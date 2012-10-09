#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "kr_macros.h"
#include "kr_string.h"

/*****************************************************************************/
/* FUNC:   boolean kr_string_isnumeric(const char *psBuf)                    */
/* PARAMS: psBuf - ���봮 (I)                                                */
/* RETURN: 0 - ����                                                          */
/*         1 - ��                                                            */
/* DESC:   �ж��Ƿ�Ϊȫ���ִ�                                                */
/*****************************************************************************/
boolean kr_string_isnumeric(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if ((psBuf[i] < '0') || (psBuf[i] > '9')) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   boolean kr_string_isdecimal(const char *psBuf)                    */
/* PARAMS: psBuf - ���봮 (I)                                                */
/* RETURN: 0 - ����                                                          */
/*         1 - ��                                                            */
/* DESC:   �ж��Ƿ�Ϊȫ����                                                  */
/*****************************************************************************/
boolean kr_string_isdecimal(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if (((psBuf[i] <  '0') || (psBuf[i] >  '9')) &&
            (psBuf[i] != '.') && (psBuf[i] != '-') && (psBuf[i] != '+')) 
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   boolean kr_string_isalphabet(const char *psBuf)                   */
/* PARAMS: psBuf - ���봮 (I)                                                */
/* RETURN: 0 - ����                                                          */
/*         1 - ��                                                            */
/* DESC:   �ж��Ƿ�Ϊȫ��ĸ��                                                */
/*****************************************************************************/
boolean kr_string_isalphabet(const char *psBuf)
{
    int i;
    for (i = 0; i < strlen(psBuf); i++) 
    {
        if ((psBuf[i] < 'A') ||
            ((psBuf[i] > 'Z') && (psBuf[i] < 'a')) ||
            (psBuf[i] > 'z'))
        {
            return FALSE;
        }
    }
    return TRUE;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_rtrim(char *pcaBuf)                               */
/* PARAMS: pcaBuf  - ת���� (I/O)                                            */
/* RETURN: ת�������                                                        */
/* DESC:   ȥ���ұ߿ո�                                                      */
/*****************************************************************************/
char *kr_string_rtrim(char *pcaBuf)
{
    register int l;
    for(l=(int)strlen(pcaBuf); l>0 && isspace((unsigned char)pcaBuf[l-1]); l--) 
        pcaBuf[l-1]='\0';
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_ltrim(char *pcaBuf)                               */
/* PARAMS: pcaBuf  - ���봮 (I)                                              */
/* RETURN: ת�������                                                        */
/* DESC:   ȥ����߿ո�                                                      */
/*****************************************************************************/
char *kr_string_ltrim( char *pcaBuf )
{
    register char *p;
    for( p=pcaBuf; isspace((unsigned char)*p); p++ );
        if( p!=pcaBuf )  strcpy(pcaBuf, p);
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_alltrim(char *pcaBuf)                             */
/* PARAMS: pcaBuf  - ���봮 (I)                                              */
/* RETURN: ת�������                                                        */
/* DESC:   ȥ�����߿ո�                                                      */
/*****************************************************************************/
char *kr_string_alltrim( char *pcaBuf )
{
    kr_string_rtrim(pcaBuf);
    kr_string_ltrim(pcaBuf);
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_toupper(char *pcaBuf)                             */
/* PARAMS: psBuf - ת���� (I/O)                                              */
/* RETURN: ���ش�                                                            */
/*         1 - ��                                                            */
/* DESC:   ת��Ϊȫ��д                                                      */
/*****************************************************************************/
char *kr_string_toupper(char *pcaBuf)
{
    int i;
    for (i = 0; i < strlen(pcaBuf); i++)
    {
        if (((unsigned char)pcaBuf[i] >= 0x81) && 
            ((unsigned char)pcaBuf[i] <= 0xfe))
        {
            /* ����Ǻ��ֵ�ǰ����ַ��������������ֲ���ת�� */
            i++;
        }
        else
        {
            pcaBuf[i] = toupper(pcaBuf[i]);
        }
    }
    return pcaBuf;
}

/*****************************************************************************/
/* FUNC:   char *kr_string_tolower(char *pcaBuf)                             */
/* PARAMS: psBuf - ת���� (I/O)                                              */
/* RETURN: ���ش�                                                            */
/*         1 - ��                                                            */
/* DESC:   ת��ΪȫСд                                                      */
/*****************************************************************************/
char *kr_string_tolower(char *pcaBuf)
{
    int i;
    for (i = 0; i < strlen(pcaBuf); i++)
    {
        if (((unsigned char)pcaBuf[i] >= 0x81) && 
            ((unsigned char)pcaBuf[i] <= 0xfe))
        {
            /* ����Ǻ��ֵ�ǰ����ַ��������������ֲ���ת�� */
            i++;
        }
        else
        {
            pcaBuf[i] = tolower(pcaBuf[i]);
        }
    }
    return pcaBuf;
}