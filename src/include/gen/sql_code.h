/*
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
 * The content of this file was generated by the Firebird project
 * using the program jrd/codes.epp
 */
/*
 * 
 * *** WARNING *** - This file is automatically generated by codes.epp - do not edit!
 * 
 */
static const SSHORT gds__sql_code [] = {
   0 /* No Error */,
-802 /*   1 arith_except                     */,
-901 /*   2 bad_dbkey                        */,
-922 /*   3 bad_db_format                    */,
-904 /*   4 bad_db_handle                    */,
-924 /*   5 bad_dpb_content                  */,
-901 /*   6 bad_dpb_form                     */,
-501 /*   7 bad_req_handle                   */,
-901 /*   8 bad_segstr_handle                */,
-901 /*   9 bad_segstr_id                    */,
-901 /*  10 bad_tpb_content                  */,
-901 /*  11 bad_tpb_form                     */,
-901 /*  12 bad_trans_handle                 */,
-902 /*  13 bug_check                        */,
-413 /*  14 convert_error                    */,
-902 /*  15 db_corrupt                       */,
-913 /*  16 deadlock                         */,
-901 /*  17 excess_trans                     */,
 100 /*  18 from_no_match                    */,
-901 /*  19 infinap                          */,
-901 /*  20 infona                           */,
-901 /*  21 infunk                           */,
-901 /*  22 integ_fail                       */,
-104 /*  23 invalid_blr                      */,
-902 /*  24 io_error                         */,
-901 /*  25 lock_conflict                    */,
-902 /*  26 metadata_corrupt                 */,
-625 /*  27 not_valid                        */,
-508 /*  28 no_cur_rec                       */,
-803 /*  29 no_dup                           */,
-901 /*  30 no_finish                        */,
-607 /*  31 no_meta_update                   */,
-551 /*  32 no_priv                          */,
-901 /*  33 no_recon                         */,
 100 /*  34 no_record                        */,
-901 /*  35 no_segstr_close                  */,
-820 /*  36 obsolete_metadata                */,
-901 /*  37 open_trans                       */,
-901 /*  38 port_len                         */,
-151 /*  39 read_only_field                  */,
-150 /*  40 read_only_rel                    */,
-817 /*  41 read_only_trans                  */,
-150 /*  42 read_only_view                   */,
-901 /*  43 req_no_trans                     */,
-901 /*  44 req_sync                         */,
-901 /*  45 req_wrong_db                     */,
 101 /*  46 segment                          */,
 100 /*  47 segstr_eof                       */,
-402 /*  48 segstr_no_op                     */,
-901 /*  49 segstr_no_read                   */,
-901 /*  50 segstr_no_trans                  */,
-817 /*  51 segstr_no_write                  */,
-901 /*  52 segstr_wrong_db                  */,
-902 /*  53 sys_request                      */,
 100 /*  54 stream_eof                       */,
-904 /*  55 unavailable                      */,
-901 /*  56 unres_rel                        */,
-901 /*  57 uns_ext                          */,
-901 /*  58 wish_list                        */,
-820 /*  59 wrong_ods                        */,
-804 /*  60 wronumarg                        */,
-904 /*  61 imp_exc                          */,
-901 /*  62 random                           */,
-901 /*  63 fatal_conflict                   */,
-902 /*  64 badblk                           */,
-902 /*  65 invpoolcl                        */,
-904 /*  66 nopoolids                        */,
-902 /*  67 relbadblk                        */,
-902 /*  68 blktoobig                        */,
-904 /*  69 bufexh                           */,
-104 /*  70 syntaxerr                        */,
-904 /*  71 bufinuse                         */,
-901 /*  72 bdbincon                         */,
-904 /*  73 reqinuse                         */,
-902 /*  74 badodsver                        */,
-219 /*  75 relnotdef                        */,
-205 /*  76 fldnotdef                        */,
-902 /*  77 dirtypage                        */,
-902 /*  78 waifortra                        */,
-902 /*  79 doubleloc                        */,
-902 /*  80 nodnotfnd                        */,
-902 /*  81 dupnodfnd                        */,
-902 /*  82 locnotmar                        */,
-689 /*  83 badpagtyp                        */,
-902 /*  84 corrupt                          */,
-902 /*  85 badpage                          */,
-902 /*  86 badindex                         */,
-901 /*  87 dbbnotzer                        */,
-901 /*  88 tranotzer                        */,
-902 /*  89 trareqmis                        */,
-902 /*  90 badhndcnt                        */,
-902 /*  91 wrotpbver                        */,
-902 /*  92 wroblrver                        */,
-902 /*  93 wrodpbver                        */,
-402 /*  94 blobnotsup                       */,
-902 /*  95 badrelation                      */,
-902 /*  96 nodetach                         */,
-902 /*  97 notremote                        */,
-901 /*  98 trainlim                         */,
-901 /*  99 notinlim                         */,
-901 /* 100 traoutsta                        */,
-923 /* 101 connect_reject                   */,
-902 /* 102 dbfile                           */,
-902 /* 103 orphan                           */,
-904 /* 104 no_lock_mgr                      */,
-104 /* 105 ctxinuse                         */,
-104 /* 106 ctxnotdef                        */,
-402 /* 107 datnotsup                        */,
-901 /* 108 badmsgnum                        */,
-104 /* 109 badparnum                        */,
-904 /* 110 virmemexh                        */,
-901 /* 111 blocking_signal                  */,
-902 /* 112 lockmanerr                       */,
-924 /* 113 journerr                         */,
-664 /* 114 keytoobig                        */,
-407 /* 115 nullsegkey                       */,
-902 /* 116 sqlerr                           */,
-820 /* 117 wrodynver                        */,
-172 /* 118 funnotdef                        */,
-171 /* 119 funmismat                        */,
-104 /* 120 bad_msg_vec                      */,
-924 /* 121 bad_detach                       */,
-901 /* 122 noargacc_read                    */,
-901 /* 123 noargacc_write                   */,
-817 /* 124 read_only                        */,
-677 /* 125 ext_err                          */,
-150 /* 126 non_updatable                    */,
-926 /* 127 no_rollback                      */,
-902 /* 128 bad_sec_info                     */,
-902 /* 129 invalid_sec_info                 */,
-901 /* 130 misc_interpreted                 */,
-904 /* 131 update_conflict                  */,
-906 /* 132 unlicensed                       */,
-904 /* 133 obj_in_use                       */,
-413 /* 134 nofilter                         */,
-904 /* 135 shadow_accessed                  */,
-104 /* 136 invalid_sdl                      */,
-406 /* 137 out_of_bounds                    */,
-171 /* 138 invalid_dimension                */,
-911 /* 139 rec_in_limbo                     */,
-904 /* 140 shadow_missing                   */,
-923 /* 141 cant_validate                    */,
-923 /* 142 cant_start_journal               */,
-204 /* 143 gennotdef                        */,
-923 /* 144 cant_start_logging               */,
-685 /* 145 bad_segstr_type                  */,
-530 /* 146 foreign_key                      */,
-820 /* 147 high_minor                       */,
-901 /* 148 tra_state                        */,
-532 /* 149 trans_invalid                    */,
-902 /* 150 buf_invalid                      */,
-902 /* 151 indexnotdefined                  */,
-902 /* 152 login                            */,
-823 /* 153 invalid_bookmark                 */,
-824 /* 154 bad_lock_level                   */,
-615 /* 155 relation_lock                    */,
-615 /* 156 record_lock                      */,
-692 /* 157 max_idx                          */,
-902 /* 158 jrn_enable                       */,
-902 /* 159 old_failure                      */,
-902 /* 160 old_in_progress                  */,
-902 /* 161 old_no_space                     */,
-902 /* 162 no_wal_no_jrn                    */,
-902 /* 163 num_old_files                    */,
-902 /* 164 wal_file_open                    */,
-901 /* 165 bad_stmt_handle                  */,
-902 /* 166 wal_failure                      */,
-230 /* 167 walw_err                         */,
-231 /* 168 logh_small                       */,
-232 /* 169 logh_inv_version                 */,
-233 /* 170 logh_open_flag                   */,
-234 /* 171 logh_open_flag2                  */,
-235 /* 172 logh_diff_dbname                 */,
-236 /* 173 logf_unexpected_eof              */,
-237 /* 174 logr_incomplete                  */,
-238 /* 175 logr_header_small                */,
-239 /* 176 logb_small                       */,
-240 /* 177 wal_illegal_attach               */,
-241 /* 178 wal_invalid_wpb                  */,
-242 /* 179 wal_err_rollover                 */,
-243 /* 180 no_wal                           */,
-615 /* 181 drop_wal                         */,
-204 /* 182 stream_not_defined               */,
-244 /* 183 wal_subsys_error                 */,
-245 /* 184 wal_subsys_corrupt               */,
-902 /* 185 no_archive                       */,
-902 /* 186 shutinprog                       */,
-615 /* 187 range_in_use                     */,
-834 /* 188 range_not_found                  */,
-204 /* 189 charset_not_found                */,
-901 /* 190 lock_timeout                     */,
-204 /* 191 prcnotdef                        */,
-170 /* 192 prcmismat                        */,
-246 /* 193 wal_bugcheck                     */,
-247 /* 194 wal_cant_expand                  */,
-204 /* 195 codnotdef                        */,
-204 /* 196 xcpnotdef                        */,
-836 /* 197 except                           */,
-837 /* 198 cache_restart                    */,
-825 /* 199 bad_lock_handle                  */,
-902 /* 200 jrn_present                      */,
-248 /* 201 wal_err_rollover2                */,
-249 /* 202 wal_err_logwrite                 */,
-250 /* 203 wal_err_jrn_comm                 */,
-251 /* 204 wal_err_expansion                */,
-252 /* 205 wal_err_setup                    */,
-253 /* 206 wal_err_ww_sync                  */,
-254 /* 207 wal_err_ww_start                 */,
-902 /* 208 shutdown                         */,
-553 /* 209 existing_priv_mod                */,
-616 /* 210 primary_key_ref                  */,
-291 /* 211 primary_key_notnull              */,
-204 /* 212 ref_cnstrnt_notfound             */,
-660 /* 213 foreign_key_notfound             */,
-292 /* 214 ref_cnstrnt_update               */,
-293 /* 215 check_cnstrnt_update             */,
-294 /* 216 check_cnstrnt_del                */,
-618 /* 217 integ_index_seg_del              */,
-618 /* 218 integ_index_seg_mod              */,
-616 /* 219 integ_index_del                  */,
-616 /* 220 integ_index_mod                  */,
-616 /* 221 check_trig_del                   */,
-617 /* 222 check_trig_update                */,
-616 /* 223 cnstrnt_fld_del                  */,
-617 /* 224 cnstrnt_fld_rename               */,
-295 /* 225 rel_cnstrnt_update               */,
-150 /* 226 constaint_on_view                */,
-296 /* 227 invld_cnstrnt_type               */,
-831 /* 228 primary_key_exists               */,
-607 /* 229 systrig_update                   */,
-552 /* 230 not_rel_owner                    */,
-204 /* 231 grant_obj_notfound               */,
-205 /* 232 grant_fld_notfound               */,
-552 /* 233 grant_nopriv                     */,
 -84 /* 234 nonsql_security_rel              */,
 -84 /* 235 nonsql_security_fld              */,
-255 /* 236 wal_cache_err                    */,
-902 /* 237 shutfail                         */,
-297 /* 238 check_constraint                 */,
-901 /* 239 bad_svc_handle                   */,
-838 /* 240 shutwarn                         */,
-901 /* 241 wrospbver                        */,
-901 /* 242 bad_spb_form                     */,
-901 /* 243 svcnotdef                        */,
-902 /* 244 no_jrn                           */,
-314 /* 245 transliteration_failed           */,
-257 /* 246 start_cm_for_wal                 */,
-258 /* 247 wal_ovflow_log_required          */,
-204 /* 248 text_subtype                     */,
-902 /* 249 dsql_error                       */,
-104 /* 250 dsql_command_err                 */,
-103 /* 251 dsql_constant_err                */,
-504 /* 252 dsql_cursor_err                  */,
-204 /* 253 dsql_datatype_err                */,
-502 /* 254 dsql_decl_err                    */,
-510 /* 255 dsql_cursor_update_err           */,
-502 /* 256 dsql_cursor_open_err             */,
-501 /* 257 dsql_cursor_close_err            */,
-206 /* 258 dsql_field_err                   */,
-104 /* 259 dsql_internal_err                */,
-204 /* 260 dsql_relation_err                */,
-204 /* 261 dsql_procedure_err               */,
-518 /* 262 dsql_request_err                 */,
-804 /* 263 dsql_sqlda_err                   */,
16384 /* 264 dsql_var_count_err               */,
-826 /* 265 dsql_stmt_handle                 */,
-804 /* 266 dsql_function_err                */,
-206 /* 267 dsql_blob_err                    */,
-204 /* 268 collation_not_found              */,
-204 /* 269 collation_not_for_charset        */,
-104 /* 270 dsql_dup_option                  */,
-104 /* 271 dsql_tran_err                    */,
-104 /* 272 dsql_invalid_array               */,
-604 /* 273 dsql_max_arr_dim_exceeded        */,
-604 /* 274 dsql_arr_range_error             */,
-204 /* 275 dsql_trigger_err                 */,
-206 /* 276 dsql_subselect_err               */,
-531 /* 277 dsql_crdb_prepare_err            */,
-157 /* 278 specify_field_err                */,
-158 /* 279 num_field_err                    */,
-806 /* 280 col_name_err                     */,
-807 /* 281 where_err                        */,
-808 /* 282 table_view_err                   */,
-809 /* 283 distinct_err                     */,
-832 /* 284 key_field_count_err              */,
-810 /* 285 subquery_err                     */,
-833 /* 286 expression_eval_err              */,
-599 /* 287 node_err                         */,
-104 /* 288 command_end_err                  */,
-901 /* 289 index_name                       */,
-901 /* 290 exception_name                   */,
-901 /* 291 field_name                       */,
-104 /* 292 token_err                        */,
-901 /* 293 union_err                        */,
-901 /* 294 dsql_construct_err               */,
-830 /* 295 field_aggregate_err              */,
-829 /* 296 field_ref_err                    */,
-208 /* 297 order_by_err                     */,
-171 /* 298 return_mode_err                  */,
-170 /* 299 extern_func_err                  */,
-204 /* 300 alias_conflict_err               */,
-204 /* 301 procedure_conflict_error         */,
-204 /* 302 relation_conflict_err            */,
-901 /* 303 dsql_domain_err                  */,
-663 /* 304 idx_seg_err                      */,
-599 /* 305 node_name_err                    */,
-901 /* 306 table_name                       */,
-901 /* 307 proc_name                        */,
-660 /* 308 idx_create_err                   */,
-259 /* 309 wal_shadow_err                   */,
-616 /* 310 dependency                       */,
-663 /* 311 idx_key_err                      */,
-597 /* 312 dsql_file_length_err             */,
-598 /* 313 dsql_shadow_number_err           */,
-104 /* 314 dsql_token_unk_err               */,
-204 /* 315 dsql_no_relation_alias           */,
-204 /* 316 indexname                        */,
-281 /* 317 no_stream_plan                   */,
-282 /* 318 stream_twice                     */,
-283 /* 319 stream_not_found                 */,
-204 /* 320 collation_requires_text          */,
-901 /* 321 dsql_domain_not_found            */,
-284 /* 322 index_unused                     */,
-282 /* 323 dsql_self_join                   */,
-596 /* 324 stream_bof                       */,
-595 /* 325 stream_crack                     */,
-601 /* 326 db_or_file_exists                */,
-401 /* 327 invalid_operator                 */,
-924 /* 328 conn_lost                        */,
-835 /* 329 bad_checksum                     */,
-689 /* 330 page_type_err                    */,
-816 /* 331 ext_readonly_err                 */,
-811 /* 332 sing_select_err                  */,
-902 /* 333 psw_attach                       */,
-902 /* 334 psw_start_trans                  */,
-827 /* 335 invalid_direction                */,
-901 /* 336 dsql_var_conflict                */,
-607 /* 337 dsql_no_blob_array               */,
-155 /* 338 dsql_base_table                  */,
-282 /* 339 duplicate_base_table             */,
-282 /* 340 view_alias                       */,
-904 /* 341 index_root_page_full             */,
-204 /* 342 dsql_blob_type_unknown           */,
-693 /* 343 req_max_clones_exceeded          */,
-637 /* 344 dsql_duplicate_spec              */,
-803 /* 345 unique_key_violation             */,
-901 /* 346 srvr_version_too_old             */,
-909 /* 347 drdb_completed_with_errs         */,
 -84 /* 348 dsql_procedure_use_err           */,
-313 /* 349 dsql_count_mismatch              */,
-685 /* 350 blob_idx_err                     */,
-685 /* 351 array_idx_err                    */,
-663 /* 352 key_field_err                    */,
-901 /* 353 no_delete                        */,
-616 /* 354 del_last_field                   */,
-901 /* 355 sort_err                         */,
-904 /* 356 sort_mem_err                     */,
-841 /* 357 version_err                      */,
-828 /* 358 inval_key_posn                   */,
-690 /* 359 no_segments_err                  */,
-600 /* 360 crrp_data_err                    */,
-691 /* 361 rec_size_err                     */,
-605 /* 362 dsql_field_ref                   */,
-904 /* 363 req_depth_exceeded               */,
-694 /* 364 no_field_access                  */,
-162 /* 365 no_dbkey                         */,
-839 /* 366 jrn_format_err                   */,
-840 /* 367 jrn_file_full                    */,
-519 /* 368 dsql_open_cursor_request         */,
-999 /* 369 ib_error                         */,
-260 /* 370 cache_redef                      */,
-239 /* 371 cache_too_small                  */,
-260 /* 372 log_redef                        */,
-239 /* 373 log_too_small                    */,
-239 /* 374 partition_too_small              */,
-261 /* 375 partition_not_supp               */,
-261 /* 376 log_length_spec                  */,
-842 /* 377 precision_err                    */,
-842 /* 378 scale_nogt                       */,
-842 /* 379 expec_short                      */,
-842 /* 380 expec_long                       */,
-842 /* 381 expec_ushort                     */,
-105 /* 382 like_escape_invalid              */,
-901 /* 383 svcnoexe                         */,
-901 /* 384 net_lookup_err                   */,
-901 /* 385 service_unknown                  */,
-901 /* 386 host_unknown                     */,
-552 /* 387 grant_nopriv_on_base             */,
-203 /* 388 dyn_fld_ambiguous                */,
-104 /* 389 dsql_agg_ref_err                 */,
-282 /* 390 complex_view                     */,
-901 /* 391 unprepared_stmt                  */,
-842 /* 392 expec_positive                   */,
-804 /* 393 dsql_sqlda_value_err             */,
-104 /* 394 invalid_array_id                 */,
-816 /* 395 extfile_uns_op                   */,
-901 /* 396 svc_in_use                       */,
-902 /* 397 err_stack_limit                  */,
-827 /* 398 invalid_key                      */,
-901 /* 399 net_init_error                   */,
-901 /* 400 loadlib_failure                  */,
-902 /* 401 network_error                    */,
-902 /* 402 net_connect_err                  */,
-902 /* 403 net_connect_listen_err           */,
-902 /* 404 net_event_connect_err            */,
-902 /* 405 net_event_listen_err             */,
-902 /* 406 net_read_err                     */,
-902 /* 407 net_write_err                    */,
-616 /* 408 integ_index_deactivate           */,
-616 /* 409 integ_deactivate_primary         */,
-104 /* 410 cse_not_supported                */,
-901 /* 411 tra_must_sweep                   */,
-902 /* 412 unsupported_network_drive        */,
-902 /* 413 io_create_err                    */,
-902 /* 414 io_open_err                      */,
-902 /* 415 io_close_err                     */,
-902 /* 416 io_read_err                      */,
-902 /* 417 io_write_err                     */,
-902 /* 418 io_delete_err                    */,
-902 /* 419 io_access_err                    */,
-901 /* 420 udf_exception                    */,
-901 /* 421 lost_db_connection               */,
-901 /* 422 no_write_user_priv               */,
-104 /* 423 token_too_long                   */,
-906 /* 424 max_att_exceeded                 */,
-902 /* 425 login_same_as_role_name          */,
-607 /* 426 reftable_requires_pk             */,
 -85 /* 427 usrname_too_long                 */,
 -85 /* 428 password_too_long                */,
 -85 /* 429 usrname_required                 */,
 -85 /* 430 password_required                */,
 -85 /* 431 bad_protocol                     */,
 -85 /* 432 dup_usrname_found                */,
 -85 /* 433 usrname_not_found                */,
 -85 /* 434 error_adding_sec_record          */,
 -85 /* 435 error_modifying_sec_record       */,
 -85 /* 436 error_deleting_sec_record        */,
 -85 /* 437 error_updating_sec_db            */,
-904 /* 438 sort_rec_size_err                */,
-204 /* 439 bad_default_value                */,
-204 /* 440 invalid_clause                   */,
-904 /* 441 too_many_handles                 */,
-904 /* 442 optimizer_blk_exc                */,
-104 /* 443 invalid_string_constant          */,
-104 /* 444 transitional_date                */,
-817 /* 445 read_only_database               */,
-817 /* 446 must_be_dialect_2_and_up         */,
-901 /* 447 blob_filter_exception            */,
-901 /* 448 exception_access_violation       */,
-901 /* 449 exception_datatype_missalignment */,
-901 /* 450 exception_array_bounds_exceeded  */,
-901 /* 451 exception_float_denormal_operand */,
-901 /* 452 exception_float_divide_by_zero   */,
-901 /* 453 exception_float_inexact_result   */,
-901 /* 454 exception_float_invalid_operand  */,
-901 /* 455 exception_float_overflow         */,
-901 /* 456 exception_float_stack_check      */,
-901 /* 457 exception_float_underflow        */,
-901 /* 458 exception_integer_divide_by_zero */,
-901 /* 459 exception_integer_overflow       */,
-901 /* 460 exception_unknown                */,
-901 /* 461 exception_stack_overflow         */,
-901 /* 462 exception_sigsegv                */,
-901 /* 463 exception_sigill                 */,
-901 /* 464 exception_sigbus                 */,
-901 /* 465 exception_sigfpe                 */,
-901 /* 466 ext_file_delete                  */,
-901 /* 467 ext_file_modify                  */,
-901 /* 468 adm_task_denied                  */,
-105 /* 469 extract_input_mismatch           */,
-551 /* 470 insufficient_svc_privileges      */,
-902 /* 471 file_in_use                      */,
-904 /* 472 service_att_err                  */,
-817 /* 473 ddl_not_allowed_by_db_sql_dial   */,
-901 /* 474 cancelled                        */,
-902 /* 475 unexp_spb_form                   */,
-104 /* 476 sql_dialect_datatype_unsupport   */,
-901 /* 477 svcnouser                        */,
-104 /* 478 depend_on_uncommitted_rel        */,
-904 /* 479 svc_name_missing                 */,
-204 /* 480 too_many_contexts                */,
-901 /* 481 datype_notsup                    */,
 501 /* 482 dialect_reset_warning            */,
-901 /* 483 dialect_not_changed              */,
-901 /* 484 database_create_failed           */,
-901 /* 485 inv_dialect_specified            */,
-901 /* 486 valid_db_dialects                */,
 300 /* 487 sqlwarn                          */,
 301 /* 488 dtype_renamed                    */,
-902 /* 489 extern_func_dir_error            */,
-833 /* 490 date_range_exceeded              */,
-901 /* 491 inv_client_dialect_specified     */,
-901 /* 492 valid_client_dialects            */,
-904 /* 493 optimizer_between_err            */,
-901 /* 494 service_not_supported            */,
-607 /* 495 generator_name                   */,
-607 /* 496 udf_name                         */,
-204 /* 497 bad_limit_param                  */,
-204 /* 498 bad_skip_param                   */,
-902 /* 499 io_32bit_exceeded_err            */,
-902 /* 500 invalid_savepoint                */,
-104 /* 501 dsql_column_pos_err              */,
-104 /* 502 dsql_agg_where_err               */,
-104 /* 503 dsql_agg_group_err               */,
-104 /* 504 dsql_agg_column_err              */,
-104 /* 505 dsql_agg_having_err              */,
-104 /* 506 dsql_agg_nested_err              */,
-904 /* 507 exec_sql_invalid_arg             */,
-904 /* 508 exec_sql_invalid_req             */,
-904 /* 509 exec_sql_invalid_var             */,
-904 /* 510 exec_sql_max_call_exceeded       */,
-902 /* 511 conf_access_denied               */,
-904 /* 512 wrong_backup_state               */,
-904 /* 513 wal_backup_err                   */,
-901 /*   1 gfix_db_name                     */,
-901 /*   2 gfix_invalid_sw                  */,
-901 /*   4 gfix_incmp_sw                    */,
-901 /*   5 gfix_replay_req                  */,
-901 /*   6 gfix_pgbuf_req                   */,
-901 /*   7 gfix_val_req                     */,
-901 /*   8 gfix_pval_req                    */,
-901 /*   9 gfix_trn_req                     */,
-901 /*  12 gfix_full_req                    */,
-901 /*  13 gfix_usrname_req                 */,
-901 /*  14 gfix_pass_req                    */,
-901 /*  15 gfix_subs_name                   */,
-901 /*  16 gfix_wal_req                     */,
-901 /*  17 gfix_sec_req                     */,
-901 /*  18 gfix_nval_req                    */,
-901 /*  19 gfix_type_shut                   */,
-901 /*  20 gfix_retry                       */,
-901 /*  23 gfix_retry_db                    */,
-901 /*  63 gfix_exceed_max                  */,
-901 /*  64 gfix_corrupt_pool                */,
-901 /*  65 gfix_mem_exhausted               */,
-901 /*  66 gfix_bad_pool                    */,
-901 /*  67 gfix_trn_not_valid               */,
-901 /*  84 gfix_unexp_eoi                   */,
-901 /*  90 gfix_recon_fail                  */,
-901 /* 108 gfix_trn_unknown                 */,
-901 /* 110 gfix_mode_req                    */,
   0 /* 111 gfix_opt_SQL_dialect             */,
-607 /*   2 dsql_dbkey_from_non_table        */,
-104 /*   3 dsql_transitional_numeric        */,
 301 /*   4 dsql_dialect_warning_expr        */,
-104 /*   5 sql_db_dialect_dtype_unsupport   */,
-817 /*   7 isc_sql_dialect_conflict_num     */,
 301 /*   8 dsql_warning_number_ambiguous    */,
 301 /*   9 dsql_warning_number_ambiguous1   */,
 301 /*  10 dsql_warn_precision_ambiguous    */,
 301 /*  11 dsql_warn_precision_ambiguous1   */,
 301 /*  12 dsql_warn_precision_ambiguous2   */,
-204 /*  13 dsql_ambiguous_field_name        */,
-607 /*  14 dsql_udf_return_pos_err          */,
-104 /*  15 dsql_invalid_label               */,
-104 /*  16 dsql_datatypes_not_comparable    */,
-901 /* 188 dyn_role_does_not_exist          */,
-901 /* 189 dyn_no_grant_admin_opt           */,
-901 /* 190 dyn_user_not_role_member         */,
-901 /* 191 dyn_delete_role_failed           */,
-901 /* 192 dyn_grant_role_to_user           */,
-901 /* 193 dyn_inv_sql_role_name            */,
-901 /* 194 dyn_dup_sql_role                 */,
-901 /* 195 dyn_kywd_spec_for_role           */,
-901 /* 196 dyn_roles_not_supported          */,
-612 /* 204 dyn_domain_name_exists           */,
-612 /* 205 dyn_field_name_exists            */,
-383 /* 206 dyn_dependency_exists            */,
-315 /* 207 dyn_dtype_invalid                */,
-829 /* 208 dyn_char_fld_too_small           */,
-829 /* 209 dyn_invalid_dtype_conversion     */,
-829 /* 210 dyn_dtype_conv_invalid           */,
-901 /* 212 dyn_zero_len_id                  */,
-901 /*   1 gbak_unknown_switch              */,
-901 /*   2 gbak_page_size_missing           */,
-901 /*   3 gbak_page_size_toobig            */,
-901 /*   4 gbak_redir_ouput_missing         */,
-901 /*   5 gbak_switches_conflict           */,
-901 /*   6 gbak_unknown_device              */,
-901 /*   7 gbak_no_protection               */,
-901 /*   8 gbak_page_size_not_allowed       */,
-901 /*   9 gbak_multi_source_dest           */,
-901 /*  10 gbak_filename_missing            */,
-901 /*  11 gbak_dup_inout_names             */,
-901 /*  12 gbak_inv_page_size               */,
-901 /*  13 gbak_db_specified                */,
-901 /*  14 gbak_db_exists                   */,
-901 /*  15 gbak_unk_device                  */,
-901 /*  20 gbak_blob_info_failed            */,
-901 /*  21 gbak_unk_blob_item               */,
-901 /*  22 gbak_get_seg_failed              */,
-901 /*  23 gbak_close_blob_failed           */,
-901 /*  24 gbak_open_blob_failed            */,
-901 /*  25 gbak_put_blr_gen_id_failed       */,
-901 /*  26 gbak_unk_type                    */,
-901 /*  27 gbak_comp_req_failed             */,
-901 /*  28 gbak_start_req_failed            */,
-901 /*  29 gbak_rec_failed                  */,
-901 /*  30 gbak_rel_req_failed              */,
-901 /*  31 gbak_db_info_failed              */,
-901 /*  32 gbak_no_db_desc                  */,
-901 /*  33 gbak_db_create_failed            */,
-901 /*  34 gbak_decomp_len_error            */,
-901 /*  35 gbak_tbl_missing                 */,
-901 /*  36 gbak_blob_col_missing            */,
-901 /*  37 gbak_create_blob_failed          */,
-901 /*  38 gbak_put_seg_failed              */,
-901 /*  39 gbak_rec_len_exp                 */,
-901 /*  40 gbak_inv_rec_len                 */,
-901 /*  41 gbak_exp_data_type               */,
-901 /*  42 gbak_gen_id_failed               */,
-901 /*  43 gbak_unk_rec_type                */,
-901 /*  44 gbak_inv_bkup_ver                */,
-901 /*  45 gbak_missing_bkup_desc           */,
-901 /*  46 gbak_string_trunc                */,
-901 /*  47 gbak_cant_rest_record            */,
-901 /*  48 gbak_send_failed                 */,
-901 /*  49 gbak_no_tbl_name                 */,
-901 /*  50 gbak_unexp_eof                   */,
-901 /*  51 gbak_db_format_too_old           */,
-901 /*  52 gbak_inv_array_dim               */,
-901 /*  55 gbak_xdr_len_expected            */,
-901 /*  65 gbak_open_bkup_error             */,
-901 /*  66 gbak_open_error                  */,
-901 /* 182 gbak_missing_block_fac           */,
-901 /* 183 gbak_inv_block_fac               */,
-901 /* 184 gbak_block_fac_specified         */,
-901 /* 188 gbak_missing_username            */,
-901 /* 189 gbak_missing_password            */,
-901 /* 200 gbak_missing_skipped_bytes       */,
-901 /* 201 gbak_inv_skipped_bytes           */,
-901 /* 213 gbak_err_restore_charset         */,
-901 /* 215 gbak_err_restore_collation       */,
-901 /* 220 gbak_read_error                  */,
-901 /* 221 gbak_write_error                 */,
-901 /* 233 gbak_db_in_use                   */,
-901 /* 238 gbak_sysmemex                    */,
-901 /* 250 gbak_restore_role_failed         */,
-901 /* 253 gbak_role_op_missing             */,
-901 /* 258 gbak_page_buffers_missing        */,
-901 /* 259 gbak_page_buffers_wrong_param    */,
-901 /* 260 gbak_page_buffers_restore        */,
-901 /* 262 gbak_inv_size                    */,
-901 /* 263 gbak_file_outof_sequence         */,
-901 /* 264 gbak_join_file_missing           */,
-901 /* 265 gbak_stdin_not_supptd            */,
-901 /* 266 gbak_stdout_not_supptd           */,
-901 /* 267 gbak_bkup_corrupt                */,
-901 /* 268 gbak_unk_db_file_spec            */,
-901 /* 269 gbak_hdr_write_failed            */,
-901 /* 270 gbak_disk_space_ex               */,
-901 /* 271 gbak_size_lt_min                 */,
-901 /* 273 gbak_svc_name_missing            */,
-901 /* 274 gbak_not_ownr                    */,
-901 /* 279 gbak_mode_req                    */,
-901 /* 281 gbak_just_data                   */,
-901 /* 282 gbak_data_only                   */,
-901 /*  15 gsec_cant_open_db                */,
-901 /*  16 gsec_switches_error              */,
-901 /*  17 gsec_no_op_spec                  */,
-901 /*  18 gsec_no_usr_name                 */,
-901 /*  19 gsec_err_add                     */,
-901 /*  20 gsec_err_modify                  */,
-901 /*  21 gsec_err_find_mod                */,
-901 /*  22 gsec_err_rec_not_found           */,
-901 /*  23 gsec_err_delete                  */,
-901 /*  24 gsec_err_find_del                */,
-901 /*  28 gsec_err_find_disp               */,
-901 /*  29 gsec_inv_param                   */,
-901 /*  30 gsec_op_specified                */,
-901 /*  31 gsec_pw_specified                */,
-901 /*  32 gsec_uid_specified               */,
-901 /*  33 gsec_gid_specified               */,
-901 /*  34 gsec_proj_specified              */,
-901 /*  35 gsec_org_specified               */,
-901 /*  36 gsec_fname_specified             */,
-901 /*  37 gsec_mname_specified             */,
-901 /*  38 gsec_lname_specified             */,
-901 /*  40 gsec_inv_switch                  */,
-901 /*  41 gsec_amb_switch                  */,
-901 /*  42 gsec_no_op_specified             */,
-901 /*  43 gsec_params_not_allowed          */,
-901 /*  44 gsec_incompat_switch             */,
-901 /*  76 gsec_inv_username                */,
-901 /*  77 gsec_inv_pw_length               */,
-901 /*  78 gsec_db_specified                */,
-901 /*  79 gsec_db_admin_specified          */,
-901 /*  80 gsec_db_admin_pw_specified       */,
-901 /*  81 gsec_sql_role_specified          */,
-901 /*   0 license_no_file                  */,
-901 /*  19 license_op_specified             */,
-901 /*  20 license_op_missing               */,
-901 /*  21 license_inv_switch               */,
-901 /*  22 license_inv_switch_combo         */,
-901 /*  23 license_inv_op_combo             */,
-901 /*  24 license_amb_switch               */,
-901 /*  25 license_inv_parameter            */,
-901 /*  26 license_param_specified          */,
-901 /*  27 license_param_req                */,
-901 /*  28 license_syntx_error              */,
-901 /*  30 license_dup_id                   */,
-901 /*  31 license_inv_id_key               */,
-901 /*  32 license_err_remove               */,
-901 /*  33 license_err_update               */,
-901 /*  34 license_err_convert              */,
-901 /*  35 license_err_unk                  */,
-901 /*  36 license_svc_err_add              */,
-901 /*  37 license_svc_err_remove           */,
-901 /*  59 license_eval_exists              */,
-901 /*   1 gstat_unknown_switch             */,
-901 /*   2 gstat_retry                      */,
-901 /*   3 gstat_wrong_ods                  */,
-901 /*   4 gstat_unexpected_eof             */,
-901 /*  29 gstat_open_err                   */,
-901 /*  30 gstat_read_err                   */,
-901 /*  31 gstat_sysmemex                   */
};
