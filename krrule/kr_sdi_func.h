#ifndef __KR_SDI_FUNC_H__
#define __KR_SDI_FUNC_H__

typedef int  (*KRSDIAggrFunc)(void *p1, void *p2);

typedef enum {
    KR_LOC_ABSOLUTE     = '0', /*���Զ�λ*/
    KR_LOC_RELATIVE     = '1'  /*��Զ�λ*/
}E_KRLocProp;

#endif /* __KR_SDI_FUNC_H__ */
