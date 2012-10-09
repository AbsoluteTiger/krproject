/*==============================================================*/
/* ���ģ��ʹ�ã�	c	                                        */
/* WEBģ��ʹ�ã�		                                        */
/* �ɼ�ģ��ʹ�ã�		                                        */
/*==============================================================*/
WHENEVER SQLERROR EXIT FAILURE ROLLBACK;

DELETE from KR_TBL_DATASRC_FIELD_DEF;
commit;

insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 0, 'krdb_timestamp', '���ʱ��� ', 'L', 8, '1', '0', '3,4,7,8', ' ', 0, '  ', '16');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 1, 'trans_timestamp', '����ʱ��� ', 'L', 8, '1', '0', '3,4,7,8', ' ', 0, '  ', '16');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 2, 'cust_no', '�ͻ��� ', 'S', 21, '1', '0', '3,4,7,8', ' ', 0, '  ', '16');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 3, 'trans_date', '�������� ', 'S', 9, '1', '0', '3,4,7,8', ' ', 0, '  ', '12');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 4, 'trans_time', '����ʱ�� ', 'S', 7, '1', '0', '3,4,7,8', ' ', 0, '  ', '14');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 5, 'flow_no', '������ˮ�� ', 'S',  16, '1', '0', '3,4,7,8', ' ', 38, '  ', '  ');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 6, 'trans_type', '�������� ', 'S', 3, '1', '0', '3,4,7,8', ' ', 19, '  ', '04');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 7, 'trans_amt', '���׽��(�����) ', 'D', 8, '1', '0', '1,2,3,4,5,6,9', ' ', 0, '  ', '  ');
insert into KR_TBL_DATASRC_FIELD_DEF (DATASRC_ID, FIELD_ID, FIELD_NAME, FIELD_DESC, FIELD_TYPE, FIELD_LENGTH, IS_USED_BY_FLT, IS_USED_BY_RULE, FIELD_WEB_TYPE, FIELD_TEXT_RECT, FIELD_DATADIC_RECT, FIELD_USE_RECT, FIELD_SET_RECT)
values (1, 8, 'trans_loc', '���׵ص� ', 'S', 101, '1', '0', '3,4,7,8,9', ' ', 0, '  ', '  ');
commit;
