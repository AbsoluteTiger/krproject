#include "kr_ddi.h"
#include "kr_ddi_func.h"

int kr_ddi_aggr_func(T_KRDDI *krddi, T_KRContext *calc_param)
{
    int iResult = -1;
    
    int iAbsLoc = -1;
    int iRelLoc = -1;
    
    time_t tCurrTransTime = kr_get_transtime(calc_param->ptCurrRec);
    time_t tRecTransTime;
    
    T_KRListNode *node = calc_param->ptRecList->tail;
    while(node)
    {
        calc_param->ptRecord = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; /*����λ�ü�һ*/
        
        /*�����Ƿ����У��*/
        if ((krddi->ptShmDDIDef->caStatisticsType[0] == \
                               KR_DDI_STATISTICS_EXCLUDE) && 
            (calc_param->ptRecord == calc_param->ptCurrRec)) {
            KR_LOG(KR_LOGDEBUG, "Current Record is exclude");
            node = node->prev;continue;
        }
        
        /*ͳ������ԴУ��*/
        if (((T_KRTable *)calc_param->ptRecord->ptTable)->iTableId != \
            krddi->ptShmDDIDef->lStatisticsDatasrc) {
            KR_LOG(KR_LOGDEBUG, "current table[%d] doesn't match[%ld]!", \
                   ((T_KRTable *)calc_param->ptRecord->ptTable)->iTableId, 
                   krddi->ptShmDDIDef->lStatisticsDatasrc);
            node = node->prev;continue;
        }
        
        /*ʱ�䴰��У��*/
        tRecTransTime = kr_get_transtime(calc_param->ptRecord);
        if ((tCurrTransTime - tRecTransTime) > krddi->ptShmDDIDef->lStatisticsValue ) {
            KR_LOG(KR_LOGDEBUG, "Time Window [%ld] not match[%ld] [%ld]", 
                   krddi->ptShmDDIDef->lStatisticsValue, tCurrTransTime, tRecTransTime);
            node = node->prev;continue;
        }
        
        /*������У��*/
        iResult = kr_calc_eval(krddi->ptDDICalc, calc_param);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval[%ld] failed!", krddi->lDDIId);
	    	return -1;
        } else if (krddi->ptDDICalc->result_type != KR_CALCTYPE_BOOLEAN) {
            KR_LOG(KR_LOGERROR, "result_type of ddi_calc must be boolean!");
	    	return -1;
        } else if (krddi->ptDDICalc->result_ind != KR_VALUE_SETED ||
                   !krddi->ptDDICalc->result_value.b) {
            KR_LOG(KR_LOGDEBUG, "DDICalc [%c] [%d] Not Match", \
                    krddi->ptDDICalc->result_ind, 
                    krddi->ptDDICalc->result_value.b);
            node = node->prev;continue;
        }
    
        iRelLoc++; /*���λ�ü�һ*/
                        
        /*��ȡ������ֵ*/
        E_KRFieldType type = kr_get_field_type(calc_param->ptRecord, krddi->ptShmDDIDef->lStatisticsField);
        void *val = kr_get_field_value(calc_param->ptRecord, krddi->ptShmDDIDef->lStatisticsField);
        U_KRFieldVal stValue = {0};
        switch(type)
        {
            case KR_FIELDTYPE_INT:
                stValue.i = *(int *)val;
                break;
            case KR_FIELDTYPE_LONG:
                stValue.l = *(long *)val;
                break;
            case KR_FIELDTYPE_DOUBLE:
                stValue.d = *(double *)val;
                break;
            case KR_FIELDTYPE_STRING:
                stValue.s = (char *)val;
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", type);
                return -1;
        }
        
        switch(krddi->ptShmDDIDef->caStatisticsMethod[0])
        {
            case KR_DDI_METHOD_SUM:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->eValue.i = krddi->eValue.i + stValue.i;
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->eValue.l = krddi->eValue.l + stValue.l;
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->eValue.d = krddi->eValue.d + stValue.d;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MIN:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->eValue.i = MIN(krddi->eValue.i, stValue.i);
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->eValue.l = MIN(krddi->eValue.l, stValue.l);
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->eValue.d = MIN(krddi->eValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_MAX:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->eValue.i = MAX(krddi->eValue.i, stValue.i);
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->eValue.l = MAX(krddi->eValue.l, stValue.l);
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->eValue.d = MAX(krddi->eValue.d, stValue.d);
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_COUNT:
                switch(krddi->eValueType)
                {
                    case KR_FIELDTYPE_INT:
                        krddi->eValue.i = krddi->eValue.i + 1;
                        break;
                    case KR_FIELDTYPE_LONG:
                        krddi->eValue.l = krddi->eValue.l + 1;
                        break;
                    case KR_FIELDTYPE_DOUBLE:
                        krddi->eValue.d = krddi->eValue.d + 1;
                        break;
                    default:
                        KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", krddi->eValueType);
                        return -1;
                }
                break;
            case KR_DDI_METHOD_CON_INC:
                //TODO
                break;
            case KR_DDI_METHOD_CON_DEC:
                //TODO
                break;
            case KR_DDI_METHOD_CNT_DIS:
                //TODO
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad Method [%c]!", \
                       krddi->ptShmDDIDef->caStatisticsMethod[0]);
                return -1;    
        }
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        krddi->eValueInd = KR_VALUE_SETED;
        
        node = node->prev;
    }

    return 0;
}