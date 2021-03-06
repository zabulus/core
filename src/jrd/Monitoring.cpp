/*
 *  The contents of this file are subject to the Initial
 *  Developer's Public License Version 1.0 (the "License");
 *  you may not use this file except in compliance with the
 *  License. You may obtain a copy of the License at
 *  http://www.ibphoenix.com/main.nfs?a=ibphoenix&page=ibp_idpl.
 *
 *  Software distributed under the License is distributed AS IS,
 *  WITHOUT WARRANTY OF ANY KIND, either express or implied.
 *  See the License for the specific language governing rights
 *  and limitations under the License.
 *
 *  The Original Code was created by Dmitry Yemanov
 *  for the Firebird Open Source RDBMS project.
 *
 *  Copyright (c) 2006 Dmitry Yemanov <dimitr@users.sf.net>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "ids.h"

#include "../common/classes/auto.h"
#include "../common/classes/locks.h"
#include "../common/classes/fb_string.h"

#include "../common/gdsassert.h"
#include "../jrd/jrd.h"
#include "../jrd/cch.h"
#include "../jrd/ini.h"
#include "../jrd/nbak.h"
#include "../jrd/req.h"
#include "../jrd/tra.h"
#include "../jrd/blb_proto.h"
#include "../common/isc_proto.h"
#include "../common/isc_f_proto.h"
#include "../common/isc_s_proto.h"
#include "../jrd/lck_proto.h"
#include "../jrd/met_proto.h"
#include "../jrd/mov_proto.h"
#include "../jrd/opt_proto.h"
#include "../jrd/pag_proto.h"
#include "../jrd/CryptoManager.h"

#include "../jrd/Relation.h"
#include "../jrd/RecordBuffer.h"
#include "../jrd/Monitoring.h"
#include "../jrd/Function.h"

#ifdef WIN_NT
#include <process.h>
#define getpid _getpid
#endif

const char* const SCRATCH = "fb_monitor_";

using namespace Firebird;
using namespace Jrd;


const Format* MonitoringTableScan::getFormat(thread_db* tdbb, jrd_rel* relation) const
{
	MonitoringSnapshot* const snapshot = MonitoringSnapshot::create(tdbb);
	return snapshot->getData(relation)->getFormat();
}


bool MonitoringTableScan::retrieveRecord(thread_db* tdbb, jrd_rel* relation,
										 FB_UINT64 position, Record* record) const
{
	MonitoringSnapshot* const snapshot = MonitoringSnapshot::create(tdbb);
	return snapshot->getData(relation)->fetch(position, record);
}


// MonitoringData class

MonitoringData::MonitoringData(const Database* dbb)
{
	string name;
	name.printf(MONITOR_FILE, dbb->getUniqueFileId().c_str());

	Arg::StatusVector statusVector;
	try
	{
		shared_memory.reset(FB_NEW(*dbb->dbb_permanent)
			SharedMemory<MonitoringHeader>(name.c_str(), DEFAULT_SIZE, this));
	}
	catch (const Exception& ex)
	{
		iscLogException("MonitoringData: Cannot initialize the shared memory region", ex);
		throw;
	}

	fb_assert(shared_memory->getHeader()->mhb_header_version == MemoryHeader::HEADER_VERSION);
	fb_assert(shared_memory->getHeader()->mhb_version == MONITOR_VERSION);
}


MonitoringData::~MonitoringData()
{
	Guard guard(this);

	if (shared_memory->getHeader()->used == sizeof(Header))
		shared_memory->removeMapFile();
}


void MonitoringData::acquire()
{
	shared_memory->mutexLock();

	if (shared_memory->getHeader()->allocated > shared_memory->sh_mem_length_mapped)
	{
#ifdef HAVE_OBJECT_MAP
		Arg::StatusVector statusVector;
		shared_memory->remapFile(statusVector, shared_memory->getHeader()->allocated, false);
		if (!shared_memory->remapFile(statusVector, shared_memory->getHeader()->allocated, false))
		{
			status_exception::raise(statusVector);
		}
#else
		status_exception::raise(Arg::Gds(isc_montabexh));
#endif
	}
}


void MonitoringData::release()
{
	shared_memory->mutexUnlock();
}


void MonitoringData::read(SLONG att_id, TempSpace& temp)
{
	offset_t position = 0;

	// First, find and copy data for the given session

	for (ULONG offset = alignOffset(sizeof(Header)); offset < shared_memory->getHeader()->used;)
	{
		UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = alignOffset(sizeof(Element) + element->length);

		if (element->attId == att_id)
		{
			fb_assert(shared_memory->getHeader()->used >= offset + length);
			temp.write(position, ptr + sizeof(Element), element->length);
			position += element->length;
		}

		offset += length;
	}

	// Second, copy data of other sessions

	for (ULONG offset = alignOffset(sizeof(Header)); offset < shared_memory->getHeader()->used;)
	{
		UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = alignOffset(sizeof(Element) + element->length);

		if (element->attId != att_id)
		{
			fb_assert(shared_memory->getHeader()->used >= offset + length);
			temp.write(position, ptr + sizeof(Element), element->length);
			position += element->length;
		}

		offset += length;
	}
}


ULONG MonitoringData::setup(SLONG att_id)
{
	ensureSpace(sizeof(Element));

	// Prepare for writing new data at the tail

	const ULONG offset = shared_memory->getHeader()->used;
	UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
	Element* const element = (Element*) ptr;
	element->attId = att_id;
	element->length = 0;
	shared_memory->getHeader()->used += alignOffset(sizeof(Element));
	return offset;
}


void MonitoringData::write(ULONG offset, ULONG length, const void* buffer)
{
	ensureSpace(length);

	// Write data item at the tail

	UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
	Element* const element = (Element*) ptr;
	memcpy(ptr + sizeof(Element) + element->length, buffer, length);
	ULONG previous = alignOffset(sizeof(Element) + element->length);
	element->length += length;
	ULONG current = alignOffset(sizeof(Element) + element->length);
	shared_memory->getHeader()->used += (current - previous);
}


void MonitoringData::cleanup(SLONG att_id)
{
	// Remove information about the given session

	for (ULONG offset = alignOffset(sizeof(Header)); offset < shared_memory->getHeader()->used;)
	{
		UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = alignOffset(sizeof(Element) + element->length);

		if (element->attId == att_id)
		{
			fb_assert(shared_memory->getHeader()->used >= offset + length);
			memmove(ptr, ptr + length, shared_memory->getHeader()->used - offset - length);
			shared_memory->getHeader()->used -= length;
		}
		else
		{
			offset += length;
		}
	}
}


void MonitoringData::enumerate(SessionList& sessions)
{
	// Return IDs for all known sessions

	for (ULONG offset = alignOffset(sizeof(Header)); offset < shared_memory->getHeader()->used;)
	{
		UCHAR* const ptr = (UCHAR*) shared_memory->getHeader() + offset;
		const Element* const element = (Element*) ptr;
		const ULONG length = alignOffset(sizeof(Element) + element->length);
		offset += length;

		sessions.add(element->attId);
	}
}


void MonitoringData::ensureSpace(ULONG length)
{
	ULONG newSize = shared_memory->getHeader()->used + length;

	if (newSize > shared_memory->getHeader()->allocated)
	{
		newSize = FB_ALIGN(newSize, DEFAULT_SIZE);

#ifdef HAVE_OBJECT_MAP
		Arg::StatusVector statusVector;
		if (!shared_memory->remapFile(statusVector, newSize, true))
		{
			status_exception::raise(statusVector);
		}
		shared_memory->getHeader()->allocated = shared_memory->sh_mem_length_mapped;
#else
		status_exception::raise(Arg::Gds(isc_montabexh));
#endif
	}
}


void MonitoringData::mutexBug(int osErrorCode, const char* s)
{
	string msg;
	msg.printf("MONITOR: mutex %s error, status = %d", s, osErrorCode);
	fb_utils::logAndDie(msg.c_str());
}


bool MonitoringData::initialize(SharedMemoryBase* sm, bool initialize)
{
	if (initialize)
	{
		MonitoringHeader* header = reinterpret_cast<MonitoringHeader*>(sm->sh_mem_header);

		// Initialize the shared data header
		header->init(SharedMemoryBase::SRAM_DATABASE_SNAPSHOT, MONITOR_VERSION);

		header->used = alignOffset(sizeof(Header));
		header->allocated = sm->sh_mem_length_mapped;
	}

	return true;
}


ULONG MonitoringData::alignOffset(ULONG unaligned)
{
	return (ULONG) Firebird::MEM_ALIGN(unaligned);
}


// MonitoringSnapshot class


MonitoringSnapshot* MonitoringSnapshot::create(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	jrd_tra* transaction = tdbb->getTransaction();
	fb_assert(transaction);

	if (!transaction->tra_mon_snapshot)
	{
		// Create a database snapshot and store it
		// in the transaction block
		MemoryPool& pool = *transaction->tra_pool;
		transaction->tra_mon_snapshot = FB_NEW(pool) MonitoringSnapshot(tdbb, pool);
	}

	return transaction->tra_mon_snapshot;
}


MonitoringSnapshot::MonitoringSnapshot(thread_db* tdbb, MemoryPool& pool)
	: SnapshotData(pool)
{
	SET_TDBB(tdbb);

	PAG_header(tdbb, true);

	Database* const dbb = tdbb->getDatabase();
	fb_assert(dbb);

	Attachment* const attachment = tdbb->getAttachment();
	fb_assert(attachment);

	const SLONG self_att_id = attachment->att_attachment_id;

	// Initialize record buffers
	RecordBuffer* const dbb_buffer = allocBuffer(tdbb, pool, rel_mon_database);
	RecordBuffer* const att_buffer = allocBuffer(tdbb, pool, rel_mon_attachments);
	RecordBuffer* const tra_buffer = allocBuffer(tdbb, pool, rel_mon_transactions);
	RecordBuffer* const stmt_buffer = allocBuffer(tdbb, pool, rel_mon_statements);
	RecordBuffer* const call_buffer = allocBuffer(tdbb, pool, rel_mon_calls);
	RecordBuffer* const io_stat_buffer = allocBuffer(tdbb, pool, rel_mon_io_stats);
	RecordBuffer* const rec_stat_buffer = allocBuffer(tdbb, pool, rel_mon_rec_stats);
	RecordBuffer* const ctx_var_buffer = allocBuffer(tdbb, pool, rel_mon_ctx_vars);
	RecordBuffer* const mem_usage_buffer = allocBuffer(tdbb, pool, rel_mon_mem_usage);
	RecordBuffer* const tab_stat_buffer = allocBuffer(tdbb, pool, rel_mon_tab_stats);

	// Dump our own data and downgrade the lock, if required

	Monitoring::dumpAttachment(tdbb, attachment, false);

	if (!(attachment->att_flags & ATT_monitor_done))
	{
		LCK_convert(tdbb, attachment->att_monitor_lock, LCK_SR, LCK_NO_WAIT);
		attachment->att_flags |= ATT_monitor_done;
	}

	// Enumerate active sessions

	MonitoringData::SessionList sessions(pool);

	Lock temp_lock(tdbb, sizeof(SLONG), LCK_monitor), *lock = &temp_lock;

	{ // scope for the guard

		MonitoringData::Guard guard(dbb->dbb_monitoring_data);
		dbb->dbb_monitoring_data->enumerate(sessions);
	}

	// Signal other sessions to dump their state

	{ // scope for the temporary status

		ThreadStatusGuard temp_status(tdbb);

		for (SLONG* iter = sessions.begin(); iter != sessions.end(); iter++)
		{
			if (*iter != self_att_id)
			{
				lock->lck_key.lck_long = *iter;

				if (LCK_lock(tdbb, lock, LCK_SR, LCK_WAIT))
					LCK_release(tdbb, lock);
			}
		}
	}

	// Read the dump into a temporary space. While being there,
	// also check for dead sessions and garbage collect them.

	TempSpace temp_space(pool, SCRATCH);

	{ // scope for the guard

		MonitoringData::Guard guard(dbb->dbb_monitoring_data);

		ThreadStatusGuard temp_status(tdbb);
		lock->lck_type = LCK_attachment;

		for (SLONG* iter = sessions.begin(); iter != sessions.end(); iter++)
		{
			if (*iter != self_att_id)
			{
				lock->lck_key.lck_long = *iter;

				if (LCK_lock(tdbb, lock, LCK_EX, LCK_NO_WAIT))
				{
					LCK_release(tdbb, lock);
					dbb->dbb_monitoring_data->cleanup(*iter);
				}
			}
		}

		dbb->dbb_monitoring_data->read(self_att_id, temp_space);
	}

	string databaseName(dbb->dbb_database_name.c_str());
	ISC_systemToUtf8(databaseName);

	const string& userName = attachment->att_user->usr_user_name;
	const bool locksmith = attachment->locksmith();

	// Parse the dump

	MonitoringData::Reader reader(pool, temp_space);

	RecordBuffer* buffer = NULL;
	Record* record = NULL;

	bool dbb_processed = false, fields_processed = false;
	bool dbb_allowed = false, att_allowed = false;

	SnapshotData::DumpRecord dumpRecord(pool);

	while (reader.getRecord(dumpRecord))
	{
		const int rid = dumpRecord.getRelationId();

		switch (rid)
		{
		case rel_mon_database:
			buffer = dbb_buffer;
			break;
		case rel_mon_attachments:
			buffer = att_buffer;
			break;
		case rel_mon_transactions:
			buffer = tra_buffer;
			break;
		case rel_mon_statements:
			buffer = stmt_buffer;
			break;
		case rel_mon_calls:
			buffer = call_buffer;
			break;
		case rel_mon_io_stats:
			buffer = io_stat_buffer;
			break;
		case rel_mon_rec_stats:
			buffer = rec_stat_buffer;
			break;
		case rel_mon_ctx_vars:
			buffer = ctx_var_buffer;
			break;
		case rel_mon_mem_usage:
			buffer = mem_usage_buffer;
			break;
		case rel_mon_tab_stats:
			buffer = tab_stat_buffer;
			break;
		default:
			fb_assert(false);
		}

		if (buffer)
		{
			record = buffer->getTempRecord();
			record->nullify();
		}
		else
		{
			record = NULL;
		}

		SnapshotData::DumpField dumpField;
		while (dumpRecord.getField(dumpField))
		{
			const USHORT fid = dumpField.id;
			const FB_SIZE_T length = dumpField.length;
			const char* source = (const char*) dumpField.data;

			// All the strings that may require transliteration (i.e. the target charset is not NONE)
			// are known to be in the metadata charset or ASCII (which is binary compatible).
			const int charset = ttype_metadata;

			// special case for MON$DATABASE
			if (rid == rel_mon_database)
			{
				if (fid == f_mon_db_name)
					dbb_allowed = !databaseName.compare(source, length);

				if (record && dbb_allowed && !dbb_processed)
				{
					putField(tdbb, record, dumpField, charset);
					fields_processed = true;
				}

				att_allowed = (dbb_allowed && !dbb_processed);
			}
			// special case for MON$ATTACHMENTS
			else if (rid == rel_mon_attachments)
			{
				if (fid == f_mon_att_user)
					att_allowed = locksmith || !userName.compare(source, length);

				if (record && dbb_allowed && att_allowed)
				{
					putField(tdbb, record, dumpField, charset);
					fields_processed = true;
					dbb_processed = true;
				}
			}
			// generic logic that covers all other relations
			else if (record && dbb_allowed && att_allowed)
			{
				putField(tdbb, record, dumpField, charset);
				fields_processed = true;
				dbb_processed = true;
			}
		}

		if (fields_processed)
		{
			buffer->store(record);
			fields_processed = false;
		}
	}
}


void SnapshotData::clearSnapshot()
{
	for (FB_SIZE_T i = 0; i < m_snapshot.getCount(); i++)
		delete m_snapshot[i].data;

	m_snapshot.clear();
}


RecordBuffer* SnapshotData::getData(const jrd_rel* relation) const
{
	fb_assert(relation);

	return getData(relation->rel_id);
}


RecordBuffer* SnapshotData::getData(int id) const
{
	for (FB_SIZE_T i = 0; i < m_snapshot.getCount(); i++)
	{
		if (m_snapshot[i].rel_id == id)
			return m_snapshot[i].data;
	}

	return NULL;
}


RecordBuffer* SnapshotData::allocBuffer(thread_db* tdbb, MemoryPool& pool, int rel_id)
{
	jrd_rel* const relation = MET_lookup_relation_id(tdbb, rel_id, false);
	fb_assert(relation);
	MET_scan_relation(tdbb, relation);
	fb_assert(relation->isVirtual());

	const Format* const format = MET_current(tdbb, relation);
	fb_assert(format);

	RecordBuffer* const buffer = FB_NEW(pool) RecordBuffer(pool, format);
	const RelationData data = {relation->rel_id, buffer};
	m_snapshot.add(data);

	return buffer;
}


void SnapshotData::putField(thread_db* tdbb, Record* record, const DumpField& field, int charset)
{
	fb_assert(record);

	const Format* const format = record->rec_format;
	fb_assert(format);

	dsc to_desc;

	if (field.id < format->fmt_count)
		to_desc = format->fmt_desc[field.id];

	if (to_desc.isUnknown())
		return;

	to_desc.dsc_address += (IPTR) record->rec_data;

	if (field.type == VALUE_GLOBAL_ID)
	{
		// special case: translate 64-bit global ID into 32-bit local ID
		fb_assert(field.length == sizeof(SINT64));
		SINT64 global_id;
		memcpy(&global_id, field.data, field.length);

		SLONG local_id;
		if (!m_map.get(global_id, local_id))
		{
			local_id = ++m_counter;
			m_map.put(global_id, local_id);
		}

		dsc from_desc;
		from_desc.makeLong(0, &local_id);
		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else if (field.type == VALUE_TABLE_ID)
	{
		// special case: translate relation ID into name
		fb_assert(field.length == sizeof(SLONG));
		SLONG rel_id;
		memcpy(&rel_id, field.data, field.length);

		const jrd_rel* const relation = MET_lookup_relation_id(tdbb, rel_id, false);
		if (!relation || relation->rel_name.isEmpty())
			return;

		const MetaName& name = relation->rel_name;
		dsc from_desc;
		from_desc.makeText(name.length(), CS_METADATA, (UCHAR*) name.c_str());
		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else if (field.type == VALUE_INTEGER)
	{
		fb_assert(field.length == sizeof(SINT64));
		SINT64 value;
		memcpy(&value, field.data, field.length);

		dsc from_desc;
		from_desc.makeInt64(0, &value);
		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else if (field.type == VALUE_TIMESTAMP)
	{
		fb_assert(field.length == sizeof(ISC_TIMESTAMP));
		ISC_TIMESTAMP value;
		memcpy(&value, field.data, field.length);

		dsc from_desc;
		from_desc.makeTimestamp(&value);
		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else if (field.type == VALUE_STRING)
	{
		dsc from_desc;
		MoveBuffer buffer;

		if (charset == CS_NONE && to_desc.getCharSet() == CS_METADATA)
		{
			// ASF: If an attachment using NONE charset has a string using non-ASCII characters,
			// nobody will be able to select them in a system field. So we change these characters to
			// question marks here - CORE-2602.

			UCHAR* p = buffer.getBuffer(field.length);
			const UCHAR* s = (const UCHAR*) field.data;

			for (const UCHAR* end = buffer.end(); p < end; ++p, ++s)
				*p = (*s > 0x7F ? '?' : *s);

			from_desc.makeText(field.length, CS_ASCII, buffer.begin());
		}
		else
			from_desc.makeText(field.length, charset, (UCHAR*) field.data);

		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else if (field.type == VALUE_BOOLEAN)
	{
		fb_assert(field.length == sizeof(UCHAR));
		UCHAR value;
		memcpy(&value, field.data, field.length);
		dsc from_desc;
		from_desc.makeBoolean(&value);
		MOV_move(tdbb, &from_desc, &to_desc);
	}
	else
	{
		fb_assert(false);
	}

	// hvlad: detach just created temporary blob from request to bound its
	// lifetime to transaction. This is necessary as this blob belongs to
	// the MON$ table and must be accessible until transaction ends.
	if (to_desc.isBlob())
	{
		bid* blob_id = reinterpret_cast<bid*>(to_desc.dsc_address);
		jrd_tra* tran = tdbb->getTransaction();

		if (!tran->tra_blobs->locate(blob_id->bid_temp_id()))
			fb_assert(false);

		BlobIndex& blobIdx = tran->tra_blobs->current();
		fb_assert(!blobIdx.bli_materialized);

		if (blobIdx.bli_request)
		{
			if (!blobIdx.bli_request->req_blobs.locate(blobIdx.bli_temp_id))
				fb_assert(false);

			blobIdx.bli_request->req_blobs.fastRemove();
			blobIdx.bli_request = NULL;
		}
	}

	record->clearNull(field.id);
}


// Monitoring class


SINT64 Monitoring::getGlobalId(int value)
{
	return ((SINT64) getpid() << BITS_PER_LONG) + value;
}


void Monitoring::putDatabase(SnapshotData::DumpRecord& record, const Database* database,
							 MonitoringData::Writer& writer, int stat_id, int backup_state)
{
	fb_assert(database);

	record.reset(rel_mon_database);

	PathName databaseName(database->dbb_database_name);
	ISC_systemToUtf8(databaseName);

	// database name or alias (MUST BE ALWAYS THE FIRST ITEM PASSED!)
	record.storeString(f_mon_db_name, databaseName);
	// page size
	record.storeInteger(f_mon_db_page_size, database->dbb_page_size);
	// major ODS version
	record.storeInteger(f_mon_db_ods_major, database->dbb_ods_version);
	// minor ODS version
	record.storeInteger(f_mon_db_ods_minor, database->dbb_minor_version);
	// oldest interesting transaction
	record.storeInteger(f_mon_db_oit, database->dbb_oldest_transaction);
	// oldest active transaction
	record.storeInteger(f_mon_db_oat, database->dbb_oldest_active);
	// oldest snapshot transaction
	record.storeInteger(f_mon_db_ost, database->dbb_oldest_snapshot);
	// next transaction
	record.storeInteger(f_mon_db_nt, database->dbb_next_transaction);
	// number of page buffers
	record.storeInteger(f_mon_db_page_bufs, database->dbb_bcb->bcb_count);

	int temp;

	// SQL dialect
	temp = (database->dbb_flags & DBB_DB_SQL_dialect_3) ? 3 : 1;
	record.storeInteger(f_mon_db_dialect, temp);

	// shutdown mode
	if (database->dbb_ast_flags & DBB_shutdown_full)
		temp = shut_mode_full;
	else if (database->dbb_ast_flags & DBB_shutdown_single)
		temp = shut_mode_single;
	else if (database->dbb_ast_flags & DBB_shutdown)
		temp = shut_mode_multi;
	else
		temp = shut_mode_online;
	record.storeInteger(f_mon_db_shut_mode, temp);

	// sweep interval
	record.storeInteger(f_mon_db_sweep_int, database->dbb_sweep_interval);
	// read only flag
	temp = database->readOnly() ? 1 : 0;
	record.storeInteger(f_mon_db_read_only, temp);
	// forced writes flag
	temp = (database->dbb_flags & DBB_force_write) ? 1 : 0;
	record.storeInteger(f_mon_db_forced_writes, temp);
	// reserve space flag
	temp = (database->dbb_flags & DBB_no_reserve) ? 0 : 1;
	record.storeInteger(f_mon_db_res_space, temp);
	// creation date
	record.storeTimestamp(f_mon_db_created, database->dbb_creation_date);
	// database size
	record.storeInteger(f_mon_db_pages, PageSpace::actAlloc(database));
	// database backup state
	record.storeInteger(f_mon_db_backup_state, backup_state);

	// crypt thread status
	if (database->dbb_crypto_manager)
		record.storeInteger(f_mon_db_crypt_page, database->dbb_crypto_manager->getCurrentPage());

	// database owner
	record.storeString(f_mon_db_owner, database->dbb_owner);

	// statistics
	record.storeGlobalId(f_mon_db_stat_id, getGlobalId(stat_id));
	writer.putRecord(record);

	if (database->dbb_flags & DBB_shared)
	{
		putStatistics(record, database->dbb_stats, writer, stat_id, stat_database);
		putMemoryUsage(record, database->dbb_memory_stats, writer, stat_id, stat_database);
	}
	else
	{
		RuntimeStatistics zero_rt_stats;
		MemoryStats zero_mem_stats;
		putStatistics(record, zero_rt_stats, writer, stat_id, stat_database);
		putMemoryUsage(record, zero_mem_stats, writer, stat_id, stat_database);
	}
}


void Monitoring::putAttachment(SnapshotData::DumpRecord& record, const Jrd::Attachment* attachment,
							   MonitoringData::Writer& writer, int stat_id)
{
	fb_assert(attachment && attachment->att_user);

	record.reset(rel_mon_attachments);

	int temp = mon_state_idle;

	for (const jrd_tra* transaction_itr = attachment->att_transactions;
		 transaction_itr; transaction_itr = transaction_itr->tra_next)
	{
		if (transaction_itr->tra_requests)
		{
			temp = mon_state_active;
			break;
		}
	}

	PathName attName(attachment->att_filename);
	ISC_systemToUtf8(attName);

	// user (MUST BE ALWAYS THE FIRST ITEM PASSED!)
	record.storeString(f_mon_att_user, attachment->att_user->usr_user_name);
	// attachment id
	record.storeInteger(f_mon_att_id, attachment->att_attachment_id);
	// process id
	record.storeInteger(f_mon_att_server_pid, getpid());
	// state
	record.storeInteger(f_mon_att_state, temp);
	// attachment name
	record.storeString(f_mon_att_name, attName);
	// role
	record.storeString(f_mon_att_role, attachment->att_user->usr_sql_role_name);
	// remote protocol
	record.storeString(f_mon_att_remote_proto, attachment->att_network_protocol);
	// remote address
	record.storeString(f_mon_att_remote_addr, attachment->att_remote_address);
	// remote process id
	if (attachment->att_remote_pid)
	{
		record.storeInteger(f_mon_att_remote_pid, attachment->att_remote_pid);
	}
	// remote process name
	record.storeString(f_mon_att_remote_process, attachment->att_remote_process);
	// charset
	record.storeInteger(f_mon_att_charset_id, attachment->att_charset);
	// timestamp
	record.storeTimestamp(f_mon_att_timestamp, attachment->att_timestamp);
	// garbage collection flag
	temp = (attachment->att_flags & ATT_no_cleanup) ? 0 : 1;
	record.storeInteger(f_mon_att_gc, temp);
	// client library version
	record.storeString(f_mon_att_client_version, attachment->att_client_version);
	// remote protocol version
	record.storeString(f_mon_att_remote_version, attachment->att_remote_protocol);
	// remote host name
	record.storeString(f_mon_att_remote_host, attachment->att_remote_host);
	// OS user name
	record.storeString(f_mon_att_remote_os_user, attachment->att_remote_os_user);
	// authentication method
	record.storeString(f_mon_att_auth_method, attachment->att_user->usr_auth_method);
	// statistics
	record.storeGlobalId(f_mon_att_stat_id, getGlobalId(stat_id));
	// system flag
	temp = (attachment->att_flags & ATT_system) ? 1 : 0;
	record.storeInteger(f_mon_att_sys_flag, temp);

	writer.putRecord(record);

	if (attachment->att_database->dbb_flags & DBB_shared)
	{
		putStatistics(record, attachment->att_stats, writer, stat_id, stat_attachment);
		putMemoryUsage(record, attachment->att_memory_stats, writer, stat_id, stat_attachment);
	}
	else
	{
		putStatistics(record, attachment->att_database->dbb_stats, writer, stat_id, stat_attachment);
		putMemoryUsage(record, attachment->att_database->dbb_memory_stats, writer, stat_id, stat_attachment);
	}

	// context vars
	putContextVars(record, attachment->att_context_vars, writer, attachment->att_attachment_id, true);
}


void Monitoring::putTransaction(SnapshotData::DumpRecord& record, const jrd_tra* transaction,
								MonitoringData::Writer& writer, int stat_id)
{
	fb_assert(transaction);

	record.reset(rel_mon_transactions);

	int temp;

	// transaction id
	record.storeInteger(f_mon_tra_id, transaction->tra_number);
	// attachment id
	record.storeInteger(f_mon_tra_att_id, transaction->tra_attachment->att_attachment_id);
	// state
	temp = transaction->tra_requests ? mon_state_active : mon_state_idle;
	record.storeInteger(f_mon_tra_state, temp);
	// timestamp
	record.storeTimestamp(f_mon_tra_timestamp, transaction->tra_timestamp);
	// top transaction
	record.storeInteger(f_mon_tra_top, transaction->tra_top);
	// oldest transaction
	record.storeInteger(f_mon_tra_oit, transaction->tra_oldest);
	// oldest active transaction
	record.storeInteger(f_mon_tra_oat, transaction->tra_oldest_active);
	// isolation mode
	if (transaction->tra_flags & TRA_degree3)
	{
		temp = iso_mode_consistency;
	}
	else if (transaction->tra_flags & TRA_read_committed)
	{
		temp = (transaction->tra_flags & TRA_rec_version) ?
			iso_mode_rc_version : iso_mode_rc_no_version;
	}
	else
	{
		temp = iso_mode_concurrency;
	}
	record.storeInteger(f_mon_tra_iso_mode, temp);
	// lock timeout
	record.storeInteger(f_mon_tra_lock_timeout, transaction->tra_lock_timeout);
	// read only flag
	temp = (transaction->tra_flags & TRA_readonly) ? 1 : 0;
	record.storeInteger(f_mon_tra_read_only, temp);
	// autocommit flag
	temp = (transaction->tra_flags & TRA_autocommit) ? 1 : 0;
	record.storeInteger(f_mon_tra_auto_commit, temp);
	// auto undo flag
	temp = (transaction->tra_flags & TRA_no_auto_undo) ? 0 : 1;
	record.storeInteger(f_mon_tra_auto_undo, temp);

	// statistics
	record.storeGlobalId(f_mon_tra_stat_id, getGlobalId(stat_id));
	writer.putRecord(record);
	putStatistics(record, transaction->tra_stats, writer, stat_id, stat_transaction);
	putMemoryUsage(record, transaction->tra_memory_stats, writer, stat_id, stat_transaction);

	// context vars
	putContextVars(record, transaction->tra_context_vars, writer, transaction->tra_number, false);
}


void Monitoring::putRequest(SnapshotData::DumpRecord& record, const jrd_req* request,
							MonitoringData::Writer& writer, int stat_id, const string& plan)
{
	fb_assert(request);

	record.reset(rel_mon_statements);

	// request id
	record.storeInteger(f_mon_stmt_id, request->req_id);
	// attachment id
	if (request->req_attachment)
	{
		record.storeInteger(f_mon_stmt_att_id, request->req_attachment->att_attachment_id);
	}
	// state, transaction ID, timestamp
	if (request->req_flags & req_active)
	{
		const bool is_stalled = (request->req_flags & req_stall);
		record.storeInteger(f_mon_stmt_state, is_stalled ? mon_state_stalled : mon_state_active);
		if (request->req_transaction)
		{
			record.storeInteger(f_mon_stmt_tra_id, request->req_transaction->tra_number);
		}
		record.storeTimestamp(f_mon_stmt_timestamp, request->req_timestamp);
	}
	else
	{
		record.storeInteger(f_mon_stmt_state, mon_state_idle);
	}

	const JrdStatement* const statement = request->getStatement();

	// sql text
	if (statement->sqlText)
		record.storeString(f_mon_stmt_sql_text, *statement->sqlText);

	// explained plan
	if (plan.hasData())
		record.storeString(f_mon_stmt_expl_plan, plan);

	// statistics
	record.storeGlobalId(f_mon_stmt_stat_id, getGlobalId(stat_id));
	writer.putRecord(record);
	putStatistics(record, request->req_stats, writer, stat_id, stat_statement);
	putMemoryUsage(record, request->req_memory_stats, writer, stat_id, stat_statement);
}


void Monitoring::putCall(SnapshotData::DumpRecord& record, const jrd_req* request,
						 MonitoringData::Writer& writer, int stat_id)
{
	fb_assert(request);

	const jrd_req* initialRequest = request->req_caller;
	while (initialRequest->req_caller)
	{
		initialRequest = initialRequest->req_caller;
	}
	fb_assert(initialRequest);

	record.reset(rel_mon_calls);

	// call id
	record.storeInteger(f_mon_call_id, request->req_id);
	// statement id
	record.storeInteger(f_mon_call_stmt_id, initialRequest->req_id);
	// caller id
	if (initialRequest != request->req_caller)
	{
		record.storeInteger(f_mon_call_caller_id, request->req_caller->req_id);
	}

	const JrdStatement* statement = request->getStatement();
	const Routine* routine = statement->getRoutine();

	// object name/type
	if (routine)
	{
		if (routine->getName().package.hasData())
			record.storeString(f_mon_call_pkg_name, routine->getName().package);

		record.storeString(f_mon_call_name, routine->getName().identifier);
		record.storeInteger(f_mon_call_type, routine->getObjectType());
	}
	else if (!statement->triggerName.isEmpty())
	{
		record.storeString(f_mon_call_name, statement->triggerName);
		record.storeInteger(f_mon_call_type, obj_trigger);
	}
	else
	{
		// we should never be here...
		fb_assert(false);
	}

	// timestamp
	record.storeTimestamp(f_mon_call_timestamp, request->req_timestamp);
	// source line/column
	if (request->req_src_line)
	{
		record.storeInteger(f_mon_call_src_line, request->req_src_line);
		record.storeInteger(f_mon_call_src_column, request->req_src_column);
	}

	// statistics
	record.storeGlobalId(f_mon_call_stat_id, getGlobalId(stat_id));
	writer.putRecord(record);
	putStatistics(record, request->req_stats, writer, stat_id, stat_call);
	putMemoryUsage(record, request->req_memory_stats, writer, stat_id, stat_call);
}

void Monitoring::putStatistics(SnapshotData::DumpRecord& record, const RuntimeStatistics& statistics,
							   MonitoringData::Writer& writer, int stat_id, int stat_group)
{
	// statistics id
	const SINT64 id = getGlobalId(stat_id);

	// physical I/O statistics
	record.reset(rel_mon_io_stats);
	record.storeGlobalId(f_mon_io_stat_id, id);
	record.storeInteger(f_mon_io_stat_group, stat_group);
	record.storeInteger(f_mon_io_page_reads, statistics.getValue(RuntimeStatistics::PAGE_READS));
	record.storeInteger(f_mon_io_page_writes, statistics.getValue(RuntimeStatistics::PAGE_WRITES));
	record.storeInteger(f_mon_io_page_fetches, statistics.getValue(RuntimeStatistics::PAGE_FETCHES));
	record.storeInteger(f_mon_io_page_marks, statistics.getValue(RuntimeStatistics::PAGE_MARKS));
	writer.putRecord(record);

	// logical I/O statistics (global)
	record.reset(rel_mon_rec_stats);
	record.storeGlobalId(f_mon_rec_stat_id, id);
	record.storeInteger(f_mon_rec_stat_group, stat_group);
	record.storeInteger(f_mon_rec_seq_reads, statistics.getValue(RuntimeStatistics::RECORD_SEQ_READS));
	record.storeInteger(f_mon_rec_idx_reads, statistics.getValue(RuntimeStatistics::RECORD_IDX_READS));
	record.storeInteger(f_mon_rec_inserts, statistics.getValue(RuntimeStatistics::RECORD_INSERTS));
	record.storeInteger(f_mon_rec_updates, statistics.getValue(RuntimeStatistics::RECORD_UPDATES));
	record.storeInteger(f_mon_rec_deletes, statistics.getValue(RuntimeStatistics::RECORD_DELETES));
	record.storeInteger(f_mon_rec_backouts, statistics.getValue(RuntimeStatistics::RECORD_BACKOUTS));
	record.storeInteger(f_mon_rec_purges, statistics.getValue(RuntimeStatistics::RECORD_PURGES));
	record.storeInteger(f_mon_rec_expunges, statistics.getValue(RuntimeStatistics::RECORD_EXPUNGES));
	record.storeInteger(f_mon_rec_locks, statistics.getValue(RuntimeStatistics::RECORD_LOCKS));
	record.storeInteger(f_mon_rec_waits, statistics.getValue(RuntimeStatistics::RECORD_WAITS));
	record.storeInteger(f_mon_rec_conflicts, statistics.getValue(RuntimeStatistics::RECORD_CONFLICTS));
	record.storeInteger(f_mon_rec_bkver_reads, statistics.getValue(RuntimeStatistics::RECORD_BACKVERSION_READS));
	record.storeInteger(f_mon_rec_frg_reads, statistics.getValue(RuntimeStatistics::RECORD_FRAGMENT_READS));
	record.storeInteger(f_mon_rec_rpt_reads, statistics.getValue(RuntimeStatistics::RECORD_RPT_READS));
	writer.putRecord(record);

	// logical I/O statistics (table wise)

	for (RuntimeStatistics::Iterator iter = statistics.begin(); iter != statistics.end(); ++iter)
	{
		const SINT64 rec_stat_id = getGlobalId(fb_utils::genUniqueId());

		record.reset(rel_mon_tab_stats);
		record.storeGlobalId(f_mon_tab_stat_id, id);
		record.storeInteger(f_mon_tab_stat_group, stat_group);
		record.storeTableId(f_mon_tab_name, (*iter).getRelationId());
		record.storeGlobalId(f_mon_tab_rec_stat_id, rec_stat_id);
		writer.putRecord(record);

		record.reset(rel_mon_rec_stats);
		record.storeGlobalId(f_mon_rec_stat_id, rec_stat_id);
		record.storeInteger(f_mon_rec_stat_group, stat_group);
		record.storeInteger(f_mon_rec_seq_reads, (*iter).getCounter(RuntimeStatistics::RECORD_SEQ_READS));
		record.storeInteger(f_mon_rec_idx_reads, (*iter).getCounter(RuntimeStatistics::RECORD_IDX_READS));
		record.storeInteger(f_mon_rec_inserts, (*iter).getCounter(RuntimeStatistics::RECORD_INSERTS));
		record.storeInteger(f_mon_rec_updates, (*iter).getCounter(RuntimeStatistics::RECORD_UPDATES));
		record.storeInteger(f_mon_rec_deletes, (*iter).getCounter(RuntimeStatistics::RECORD_DELETES));
		record.storeInteger(f_mon_rec_backouts, (*iter).getCounter(RuntimeStatistics::RECORD_BACKOUTS));
		record.storeInteger(f_mon_rec_purges, (*iter).getCounter(RuntimeStatistics::RECORD_PURGES));
		record.storeInteger(f_mon_rec_expunges, (*iter).getCounter(RuntimeStatistics::RECORD_EXPUNGES));
		record.storeInteger(f_mon_rec_locks, (*iter).getCounter(RuntimeStatistics::RECORD_LOCKS));
		record.storeInteger(f_mon_rec_waits, (*iter).getCounter(RuntimeStatistics::RECORD_WAITS));
		record.storeInteger(f_mon_rec_conflicts, (*iter).getCounter(RuntimeStatistics::RECORD_CONFLICTS));
		record.storeInteger(f_mon_rec_bkver_reads, (*iter).getCounter(RuntimeStatistics::RECORD_BACKVERSION_READS));
		record.storeInteger(f_mon_rec_frg_reads, (*iter).getCounter(RuntimeStatistics::RECORD_FRAGMENT_READS));
		record.storeInteger(f_mon_rec_rpt_reads, (*iter).getCounter(RuntimeStatistics::RECORD_RPT_READS));
		writer.putRecord(record);
	}
}

void Monitoring::putContextVars(SnapshotData::DumpRecord& record, const StringMap& variables,
								MonitoringData::Writer& writer, int object_id, bool is_attachment)
{
	StringMap::ConstAccessor accessor(&variables);

	for (bool found = accessor.getFirst(); found; found = accessor.getNext())
	{
		record.reset(rel_mon_ctx_vars);

		if (is_attachment)
			record.storeInteger(f_mon_ctx_var_att_id, object_id);
		else
			record.storeInteger(f_mon_ctx_var_tra_id, object_id);

		record.storeString(f_mon_ctx_var_name, accessor.current()->first);
		record.storeString(f_mon_ctx_var_value, accessor.current()->second);

		writer.putRecord(record);
	}
}

void Monitoring::putMemoryUsage(SnapshotData::DumpRecord& record, const MemoryStats& stats,
								MonitoringData::Writer& writer, int stat_id, int stat_group)
{
	// statistics id
	const SINT64 id = getGlobalId(stat_id);

	// memory usage
	record.reset(rel_mon_mem_usage);
	record.storeGlobalId(f_mon_mem_stat_id, id);
	record.storeInteger(f_mon_mem_stat_group, stat_group);
	record.storeInteger(f_mon_mem_cur_used, stats.getCurrentUsage());
	record.storeInteger(f_mon_mem_cur_alloc, stats.getCurrentMapping());
	record.storeInteger(f_mon_mem_max_used, stats.getMaximumUsage());
	record.storeInteger(f_mon_mem_max_alloc, stats.getMaximumMapping());

	writer.putRecord(record);
}


void Monitoring::checkState(thread_db* tdbb)
{
	SET_TDBB(tdbb);

	Jrd::Attachment* const attachment = tdbb->getAttachment();

	if (attachment->att_flags & ATT_monitor_done)
	{
		// Enable signal handler for the monitoring stuff
		attachment->att_flags &= ~ATT_monitor_done;
		LCK_convert(tdbb, attachment->att_monitor_lock, LCK_EX, LCK_WAIT);
	}
}


void Monitoring::dumpAttachment(thread_db* tdbb, const Attachment* attachment, bool ast)
{
	if (!attachment->att_user)
		return;

	Database* const dbb = tdbb->getDatabase();
	MemoryPool& pool = *dbb->dbb_permanent;

	const SLONG att_id = attachment->att_attachment_id;

	// Determine the backup state
	int backup_state = backup_state_unknown;

	if (!ast)
	{
		BackupManager* const bm = dbb->dbb_backup_manager;

		if (bm && !bm->isShutDown())
		{
			BackupManager::StateReadGuard holder(tdbb);

			switch (bm->getState())
			{
			case Ods::hdr_nbak_normal:
				backup_state = backup_state_normal;
				break;
			case Ods::hdr_nbak_stalled:
				backup_state = backup_state_stalled;
				break;
			case Ods::hdr_nbak_merge:
				backup_state = backup_state_merge;
				break;
			}
		}
	}

	if (!dbb->dbb_monitoring_data)
		dbb->dbb_monitoring_data = FB_NEW(pool) MonitoringData(dbb);

	MonitoringData::Guard guard(dbb->dbb_monitoring_data);
	dbb->dbb_monitoring_data->cleanup(att_id);

	MonitoringData::Writer writer(dbb->dbb_monitoring_data, att_id);
	SnapshotData::DumpRecord record(pool);

	if (!ast)
		putDatabase(record, dbb, writer, fb_utils::genUniqueId(), backup_state);

	putAttachment(record, attachment, writer, fb_utils::genUniqueId());

	jrd_tra* transaction = NULL;

	// Transaction information

	for (transaction = attachment->att_transactions; transaction;
		 transaction = transaction->tra_next)
	{
		putTransaction(record, transaction, writer, fb_utils::genUniqueId());
	}

	// Call stack information

	for (transaction = attachment->att_transactions; transaction;
		 transaction = transaction->tra_next)
	{
		for (jrd_req* request = transaction->tra_requests;
			request && (request->req_flags & req_active) && (request->req_transaction == transaction);
			request = request->req_caller)
		{
			request->adjustCallerStats();

			if (!(request->getStatement()->flags &
					(JrdStatement::FLAG_INTERNAL | JrdStatement::FLAG_SYS_TRIGGER)) &&
				request->req_caller)
			{
				putCall(record, request, writer, fb_utils::genUniqueId());
			}
		}
	}

	// Request information

	for (const jrd_req* const* i = attachment->att_requests.begin();
		 i != attachment->att_requests.end();
		 ++i)
	{
		const jrd_req* const request = *i;

		if (!(request->getStatement()->flags &
				(JrdStatement::FLAG_INTERNAL | JrdStatement::FLAG_SYS_TRIGGER)))
		{
			const string plan = OPT_get_plan(tdbb, request, true);
			putRequest(record, request, writer, fb_utils::genUniqueId(), plan);
		}
	}
}


void Monitoring::publishAttachment(thread_db* tdbb)
{
	Database* const dbb = tdbb->getDatabase();
	Attachment* const attachment = tdbb->getAttachment();

	if (!dbb->dbb_monitoring_data)
		dbb->dbb_monitoring_data = FB_NEW(*dbb->dbb_permanent) MonitoringData(dbb);

	MonitoringData::Guard guard(dbb->dbb_monitoring_data);
	dbb->dbb_monitoring_data->setup(attachment->att_attachment_id);
}

void Monitoring::cleanupAttachment(thread_db* tdbb)
{
	Database* const dbb = tdbb->getDatabase();
	Attachment* const attachment = tdbb->getAttachment();

	if (dbb->dbb_monitoring_data)
	{
		MonitoringData::Guard guard(dbb->dbb_monitoring_data);
		dbb->dbb_monitoring_data->cleanup(attachment->att_attachment_id);
	}
}
