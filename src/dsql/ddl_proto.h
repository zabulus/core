/*
 *	PROGRAM:	Dynamic SQL runtime support
 *	MODULE:		ddl_proto.h
 *	DESCRIPTION:	Prototype Header file for ddl_proto.h
 *
 * The contents of this file are subject to the Interbase Public
 * License Version 1.0 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy
 * of the License at http://www.Inprise.com/IPL.html
 *
 * Software distributed under the License is distributed on an
 * "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
 * or implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code was created by Inprise Corporation
 * and its predecessors. Portions created by Inprise Corporation are
 * Copyright (C) Inprise Corporation.
 *
 * All Rights Reserved.
 * Contributor(s): ______________________________________.
 */

#ifndef _DSQL_DDL_PROTO_H_
#define _DSQL_DDL_PROTO_H_

void DDL_execute(class dsql_req*);
void DDL_generate(class dsql_req*, struct dsql_nod*);
int	DDL_ids(class dsql_req*);
void DDL_put_field_dtype(class dsql_req*, class dsql_fld*, USHORT);
extern "C" void DDL_resolve_intl_type(class dsql_req*, class dsql_fld*, class str*);
void DDL_resolve_intl_type2 (struct dsql_req *, struct dsql_fld *, struct str *, BOOLEAN);

#endif /* _DSQL_DDL_PROTO_H_ */
