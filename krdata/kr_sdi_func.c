#include "kr_context.h"

typedef enum {
    KR_LOC_ABSOLUTE     = '0', /*���Զ�λ*/
    KR_LOC_RELATIVE     = '1'  /*��Զ�λ*/
}E_KRLocProp;

int kr_sdi_aggr_func(T_KRSDI *krsdi, T_KRContext *krcontext)
{
    int iResult = -1;
    int iAbsLoc = -1;
    int iRelLoc = -1;
    
    T_KRListNode *node = krcontext->ptRecList->tail;
    while(node)
    {
        krcontext->ptRecord = (T_KRRecord *)kr_list_value(node);
        
        iAbsLoc++; /*����λ�ü�һ*/
                
        /*���������ͳ�ƶ�λУ��*/
        if (krsdi->ptShmSDIDef->caLocationProperty[0] == KR_LOC_ABSOLUTE) {     
            if (krsdi->ptShmSDIDef->lStatisticsLocation != iAbsLoc) {
                node = node->prev;
                continue;
            }
        }
        
        /*ͳ������ԴУ��*/
        if (((T_KRTable *)krcontext->ptRecord->ptTable)->iTableId != \
            krsdi->ptShmSDIDef->lStatisticsDatasrc) {
            node = node->prev;
            continue;
        }
        
        /*������У��*/
        iResult = kr_calc_eval(krsdi->ptSDICalc, krcontext);
        if (iResult != 0) {
            KR_LOG(KR_LOGERROR, "kr_calc_eval [%ld] failed!", krsdi->lSDIId);
            return -1;
        } else if (kr_calc_type(krsdi->ptSDICalc) != KR_TYPE_BOOL) {
            KR_LOG(KR_LOGERROR, "result_type of sdi_calc must be boolean!");
            return -1;
        } else if (kr_calc_ind(krsdi->ptSDICalc) != KR_VALUE_SETED ||
                   !kr_calc_value(krsdi->ptSDICalc)->b) {
            node = node->prev;
            continue;
        }
    
        iRelLoc++; /*���λ�ü�һ*/
            
        /*���������ͳ�ƶ�λУ��*/
        if (krsdi->ptShmSDIDef->caLocationProperty[0] == KR_LOC_RELATIVE) {     
            if (krsdi->ptShmSDIDef->lStatisticsLocation != iRelLoc) {
                node = node->prev;
                continue;
            }
        }
            
        /*��ȡ������ֵ*/
        void *val = kr_get_field_value(krcontext->ptRecord, \
                          krsdi->ptShmSDIDef->lStatisticsField);
        switch(krsdi->eValueType)
        {
            case KR_TYPE_INT:
                krsdi->uValue.i = *(int *)val;
                break;
            case KR_TYPE_LONG:
                krsdi->uValue.l = *(long *)val;
                break;
            case KR_TYPE_DOUBLE:
                krsdi->uValue.d = *(double *)val;
                break;
            case KR_TYPE_STRING:
                krsdi->uValue.s = kr_strdup(val);
                break;
            default:
                KR_LOG(KR_LOGERROR, "Bad FieldType [%c]!", krsdi->eValueType);
                return -1;
        }

        /*add this record to related*/
        kr_hashtable_insert(krsdi->ptRelated, \
                krcontext->ptRecord, krcontext->ptRecord);
    
        /* This is what the difference between SDI and DDI:
         * SDI only set once, while DDI still need to traversal all the list
         */
        krsdi->eValueInd = KR_VALUE_SETED;
        break;
        
        node = node->prev;
    }
    
    return 0;
}
