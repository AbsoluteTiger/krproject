prompt PL/SQL Developer import file
prompt Created on 2012��5��23�� by 111111
set feedback off
set define off
prompt Disabling triggers for KR_TBL_SDI_DEF...
alter table KR_TBL_SDI_DEF disable all triggers;
prompt Truncating KR_TBL_SDI_DEF...
truncate table KR_TBL_SDI_DEF;
prompt Loading KR_TBL_SDI_DEF...
insert into KR_TBL_SDI_DEF (SDI_ID, SDI_NAME, SDI_DESC, SDI_TYPE, SDI_VALUE_TYPE, SDI_AGGR_FUNC, STATISTICS_DATASRC, STATISTICS_INDEX, STATISTICS_FIELD, STATISTICS_LOCATION, LOCATION_PROPERTY, SDI_FILTER_STRING, SDI_STATUS, REC_CRET_DTTM, LST_UPD_DTTM, LST_UPD_USER_ID)
values (1, 'last_trans_time', '�ϱʽ��׽�����50���ҽ��׵ص���������Ľ���ʱ��', '0', 'S', '                                                  ', 1, 1, 4, 1, 'R', '(F_7>50)&&(F_8##[^TRANS_LOCATION*]);', '0', '20120523141050   ', '20120523141050   ', 'Tiger');
commit;
prompt 1 records loaded
prompt Enabling triggers for KR_TBL_SDI_DEF...
alter table KR_TBL_SDI_DEF enable all triggers;
set feedback on
set define on
prompt Done.
