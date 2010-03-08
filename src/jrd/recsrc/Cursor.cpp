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
 *  Copyright (c) 2009 Dmitry Yemanov <dimitr@firebirdsql.org>
 *  and all contributors signed below.
 *
 *  All Rights Reserved.
 *  Contributor(s): ______________________________________.
 */

#include "firebird.h"
#include "../jrd/common.h"
#include "../jrd/jrd.h"
#include "../jrd/req.h"
#include "../jrd/cmp_proto.h"

#include "RecordSource.h"
#include "Cursor.h"

using namespace Firebird;
using namespace Jrd;

// ---------------------
// Cursor implementation
// ---------------------

Cursor::Cursor(CompilerScratch* csb, RecordSource* rsb,
			   const VarInvariantArray* invariants, bool scrollable)
	: m_top(rsb), m_invariants(invariants), m_scrollable(scrollable)
{
	fb_assert(m_top);

	m_impure = CMP_impure(csb, sizeof(Impure));
}

void Cursor::open(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* impure = (Impure*) ((UCHAR*) request + m_impure);

	impure->irsb_active = true;
	impure->irsb_state = BOS;

	// Initialize dependent invariants
	if (m_invariants)
	{
		for (const SLONG* iter = m_invariants->begin(); iter < m_invariants->end(); iter++)
		{
			impure_value* const invariant_impure = (impure_value*) ((SCHAR*) request + *iter);
			invariant_impure->vlu_flags = 0;
		}
	}

	m_top->open(tdbb);
}

void Cursor::close(thread_db* tdbb)
{
	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (impure->irsb_active)
	{
		impure->irsb_active = false;
		m_top->close(tdbb);
	}
}

bool Cursor::fetchNext(thread_db* tdbb)
{
	if (!reschedule(tdbb))
	{
		return false;
	}

	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!impure->irsb_active)
	{
		// error: invalid cursor state
		status_exception::raise(Arg::Gds(isc_cursor_not_open));
	}

	if (impure->irsb_state == EOS)
	{
		// error: cursor is past EOF
		status_exception::raise(Arg::Gds(isc_stream_eof));
	}
	else if (impure->irsb_state == BOS)
	{
		impure->irsb_position = 0;
	}
	else
	{
		impure->irsb_position++;
	}

	if (!m_scrollable)
	{
		if (!m_top->getRecord(tdbb))
		{
			impure->irsb_state = EOS;
			return false;
		}
	}
	else
	{
		BufferedStream* const buffer = (BufferedStream*) m_top;
		buffer->locate(tdbb, impure->irsb_position);

		if (!buffer->getRecord(tdbb))
		{
			impure->irsb_state = EOS;
			return false;
		}
	}

	request->req_records_selected++;
	request->req_records_affected.bumpFetched();
	impure->irsb_state = POSITIONED;

	return true;
}

bool Cursor::fetchPrior(thread_db* tdbb)
{
	if (!m_scrollable)
	{
		// error: invalid fetch direction
		status_exception::raise(Arg::Gds(isc_invalid_fetch_option) << Arg::Str("PRIOR"));
	}

	if (!reschedule(tdbb))
	{
		return false;
	}

	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!impure->irsb_active)
	{
		// error: invalid cursor state
		status_exception::raise(Arg::Gds(isc_cursor_not_open));
	}

	BufferedStream* const buffer = (BufferedStream*) m_top;

	if (impure->irsb_state == BOS)
	{
		// error: cursor is prior BOF
		status_exception::raise(Arg::Gds(isc_stream_bof));
	}
	else if (impure->irsb_state == EOS)
	{
		impure->irsb_position = buffer->getCount(request) - 1;
	}
	else
	{
		impure->irsb_position--;
	}

	buffer->locate(tdbb, impure->irsb_position);

	if (!buffer->getRecord(tdbb))
	{
		impure->irsb_state = BOS;
		return false;
	}

	request->req_records_selected++;
	request->req_records_affected.bumpFetched();
	impure->irsb_state = POSITIONED;

	return true;
}

bool Cursor::fetchFirst(thread_db* tdbb)
{
	if (!m_scrollable)
	{
		// error: invalid fetch direction
		status_exception::raise(Arg::Gds(isc_invalid_fetch_option) << Arg::Str("FIRST"));
	}

	return fetchAbsolute(tdbb, 1);
}

bool Cursor::fetchLast(thread_db* tdbb)
{
	if (!m_scrollable)
	{
		// error: invalid fetch direction
		status_exception::raise(Arg::Gds(isc_invalid_fetch_option) << Arg::Str("LAST"));
	}

	return fetchAbsolute(tdbb, -1);
}

bool Cursor::fetchAbsolute(thread_db* tdbb, SINT64 offset)
{
	if (!m_scrollable)
	{
		// error: invalid fetch direction
		status_exception::raise(Arg::Gds(isc_invalid_fetch_option) << Arg::Str("ABSOLUTE"));
	}

	if (!reschedule(tdbb))
	{
		return false;
	}

	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!impure->irsb_active)
	{
		// error: invalid cursor state
		status_exception::raise(Arg::Gds(isc_cursor_not_open));
	}

	if (!offset)
	{
		impure->irsb_state = BOS;
		return false;
	}

	BufferedStream* const buffer = (BufferedStream*) m_top;
	impure->irsb_position = (offset > 0) ? offset - 1 : buffer->getCount(request) + offset;
	buffer->locate(tdbb, impure->irsb_position);

	if (!buffer->getRecord(tdbb))
	{
		impure->irsb_state = (offset > 0) ? EOS : BOS;
		return false;
	}

	request->req_records_selected++;
	request->req_records_affected.bumpFetched();
	impure->irsb_state = POSITIONED;

	return true;
}

bool Cursor::fetchRelative(thread_db* tdbb, SINT64 offset)
{
	if (!m_scrollable)
	{
		// error: invalid fetch direction
		status_exception::raise(Arg::Gds(isc_invalid_fetch_option) << Arg::Str("RELATIVE"));
	}

	if (!reschedule(tdbb))
	{
		return false;
	}

	jrd_req* const request = tdbb->getRequest();
	Impure* const impure = (Impure*) ((UCHAR*) request + m_impure);

	if (!impure->irsb_active)
	{
		// error: invalid cursor state
		status_exception::raise(Arg::Gds(isc_cursor_not_open));
	}

	if (!offset)
	{
		return (impure->irsb_state == POSITIONED);
	}
	else if (impure->irsb_state == BOS && offset < 0)
	{
		return false;
	}
	else if (impure->irsb_state == EOS && offset > 0)
	{
		return false;
	}

	BufferedStream* const buffer = (BufferedStream*) m_top;
	impure->irsb_position += offset;
	buffer->locate(tdbb, impure->irsb_position);

	if (!buffer->getRecord(tdbb))
	{
		impure->irsb_state = (offset > 0) ? EOS : BOS;
		return false;
	}

	request->req_records_selected++;
	request->req_records_affected.bumpFetched();
	impure->irsb_state = POSITIONED;

	return true;
}

bool Cursor::reschedule(thread_db* tdbb)
{
	if (--tdbb->tdbb_quantum < 0)
	{
		JRD_reschedule(tdbb, 0, true);
	}

	jrd_req* const request = tdbb->getRequest();

	if (request->req_flags & req_abort)
	{
		return false;
	}

	if (!request->req_transaction)
	{
		return false;
	}

	return true;
}