#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <errno.h>

#include "krshm/kr_shm.h"

short            gnSecId = 0;
T_KRShareMem     *gptShmBuf = NULL;

#define N_SHM_HELP          (1<<0)
#define N_SHM_CREATE        (1<<1)
#define N_SHM_LOAD          (1<<2)
#define N_SHM_VIEW          (1<<3)
#define N_SHM_REMOVE        (1<<4)


static void DumpAllMemory(void);
static int LoadAllMemory(void);
static int ViewMemory(void);
static int ViewSDIMemory(char cInd);
static int ViewDDIMemory(char cInd);
static int ViewHDIMemory(char cInd);
static int ViewRuleMemory(char cInd);
static void usage(void);
static int kr_get_shmkey(void);
static int kr_get_detectmode(void);


/** 
 *  @brief ��թ��⹲���ڴ����main����
 *  @param[in] argc ��������
 *  @param[in] argv ����ָ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
int main(int argc,char* argv[])
{
    int             ch;
    int             iInputFlag=0;
    int             iShmKey=0;
    
    /// ��ȡ����ָʾ����
    if (argc < 2) {
        usage();
        return -1;
    }

    while ((ch = getopt(argc, argv, "?CRLV")) != -1) 
    {
        switch (ch) 
        {
            case '?':
                iInputFlag |= N_SHM_HELP;
                break;
            case 'C':
                iInputFlag |= N_SHM_CREATE;
                break;
            case 'R':
                iInputFlag |= N_SHM_REMOVE;
                break;
            case 'L':
                iInputFlag |= N_SHM_LOAD;
                break;    
            case 'V':
                iInputFlag |= N_SHM_VIEW;
                break;
            default:
                usage();
                return -1;
        }
    }
 
    /// �鿴����
    if (N_SHM_HELP&iInputFlag) {
        usage();
    }
    
    iShmKey=kr_get_shmkey();
    if (iShmKey == -1) {
        printf("kr_get_shmkey error!\n");
        return -1;
    }
    
    /// ���������ڴ�
    if (N_SHM_CREATE&iInputFlag) { 
		printf("N_SHM_CREATE\n");
		gptShmBuf=kr_shm_create(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Initial Share Memory failed!\n");
            return -1;
        }
        
        if (LoadAllMemory() != 0) {
            printf("Load Share Memory failed!\n");
            kr_shm_destroy(iShmKey);
            return -1;
        }
        
        kr_shm_detach(gptShmBuf);
    }
    
    /// ���ع����ڴ�
    if (N_SHM_LOAD&iInputFlag) {
        printf("N_SHM_LOAD\n");
        gptShmBuf=kr_shm_attach(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Attach Share Memory failed!\n");
            return -1;
        }
        
        if (LoadAllMemory() != 0) {
            printf("Load Share Memory failed!\n");
            return -1;
        }

        kr_shm_detach(gptShmBuf);
    }
    
    /// �鿴�����ڴ�
    if (N_SHM_VIEW&iInputFlag) {
		printf("N_SHM_VIEW\n");
        gptShmBuf=kr_shm_attach(iShmKey);
        if (gptShmBuf == NULL) {
            printf("Attach Share Memory failed!\n");
            return -1;
        }
        
        if (ViewMemory() != 0) {
            printf("View Share Memory failed!\n");
            return -1;
        }

        kr_shm_detach(gptShmBuf);
    }

    /// ɾ�������ڴ�
    if (N_SHM_REMOVE&iInputFlag) {
		printf("N_SHM_REMOVE\n");
        printf("Are you sure to remove cum share memory?(Y/N)\n");
        ch=getchar();
        if(ch != 'Y' && ch != 'y') {
            printf("You choose [%c],return!\n", ch);
            return -1;
        }

        if (kr_shm_destroy(iShmKey) != 0) {
            printf("Remove share memory failed!\n");
            return -1;
        }
        printf("Share Memory Removed!\n");
    }
        
    return 0;
}


static int kr_get_shmkey(void)
{
    int     iShmKey;
    
    if (kr_config_setfile(getenv("KR_CONFIG_FILE")) != 0) 
    {
        printf("kr_config_setfile [%s] failed!", getenv("KR_CONFIG_FILE"));
        return -1;
    }

    if (kr_config_getint("SYSTEM", "IPCKEY", &iShmKey) != 0) 
    {
        printf("kr_config_getint IPCKEY failed!");
        return -1;
    }

    return iShmKey;
}

static int kr_get_detectmode(void)
{
    short     nDetectMode;
    
    if (kr_config_setfile(getenv("KR_CONFIG_FILE")) != 0) 
    {
        printf("kr_config_setfile [%s] failed!", getenv("KR_CONFIG_FILE"));
        return -1;
    }

    if (kr_config_getshort("SYSTEM", "DETECTMODE", &nDetectMode) != 0) 
    {
        printf("kr_config_getint DETECTMODE failed!");
        return -1;
    }

    return nDetectMode;
}

/** 
 *  @brief ���������ڴ���������
 *  @param[in]  ��
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static void DumpAllMemory(void)
{
    FILE *fp = NULL;
    char caDateTime[14+1] = {0};
    char caShmDumpFile[1024] = {0};
    
    kr_time_system(caDateTime);
    snprintf(caShmDumpFile, sizeof(caShmDumpFile), "krshm.%s.dump", caDateTime);
    if ((fp = fopen(caShmDumpFile, "a+")) == NULL) {
        printf("cant't open dump file!\n");
        return ;
    }
        
    /** dump all share memory */
    kr_shm_dump(gptShmBuf, fp);
    
    fclose(fp);
}

/** 
 *  @brief ���ع����ڴ���������
 *  @param[in]  ��
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int LoadAllMemory(void)
{
    /// ���ӽ������ݿ�
    if (dbsDbConnect( ) != 0) {
        printf("connect to db failed!\n");
        return -1;
    }
    
    /** load all share memory */
    gnSecId = kr_shm_load(gptShmBuf);
    if (gnSecId == -1) {
        printf("kr_shm_load failed!\n");
        return -1;
    }
    
    /// �Ͽ����ݿ����� 
    dbsDbDisconnect();
    
    return 0;
}

/** 
 *  @brief ��ѡ��Ĳ鿴�����ڴ������
 *  @param[in]  ��
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int ViewMemory(void)
{
    char ch = '\0';
    char ind = '\0';
        
    printf("Choose Memory Block To View: \n");
    printf("S--[S]DI memory block\n");
    printf("D--[D]DI memory block\n");
    printf("H--[H]DI memory block\n");
    printf("R--[R]ule memory block\n");
    printf("else to quit!\n");

    do
    {        
        ch = getchar();
        if(ch == 'S' || ch == 's') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewSDIMemory(ind);
        } else if(ch == 'D' || ch == 'd') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewDDIMemory(ind);
        } else if(ch == 'H' || ch == 'h') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewHDIMemory(ind);
        } else if(ch == 'R' || ch == 'r') {
            printf("Choose [A]ctive Or [S]tandby: \n");
            do
            {
                ind = getchar();
            }while(ind == '\0' || ind == '\n' || ind == '\t');
            ViewRuleMemory(ind);
        } else {
            printf("Quit:[%c]!\n", ch);
            break;
        }    
    }while(ch == '\0' || ch == '\n' || ch == '\t');
    
    return 0;
}


/** 
 *  @brief �鿴��̬����������
 *  @param[in]  cInd - ������ָʾ����
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int ViewSDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
        
    if (cInd == 'A' || cInd == 'a') {
        printf("[SDI] [Active] Memory Block:\n");
        DumpShmSDI(&gptShmBuf->stShmSDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[SDI] [Standby] Memory Block:\n");
        DumpShmSDI(&gptShmBuf->stShmSDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief �鿴��̬ͳ��������
 *  @param[in]  cInd - ������ָʾ����
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int ViewDDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
    
    if (cInd == 'A' || cInd == 'a') {
        printf("[DDI] [Active] Memory Block:\n");
        DumpShmDDI(&gptShmBuf->stShmDDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[DDI] [Standby] Memory Block:\n");
        DumpShmDDI(&gptShmBuf->stShmDDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief �鿴��ʷͳ��������
 *  @param[in]  cInd - ������ָʾ����
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int ViewHDIMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);
    
    if (cInd == 'A' || cInd == 'a') {
        printf("[HDI] [Active] Memory Block:\n");
        DumpShmHDI(&gptShmBuf->stShmHDI[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[HDI] [Standby] Memory Block:\n");
        DumpShmHDI(&gptShmBuf->stShmHDI[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}


/** 
 *  @brief �鿴����������
 *  @param[in]  cInd - ������ָʾ����
 *  @param[out] ��
 *  @return ����������
 *  @retval   0: �ɹ�
 *  @retval  -1: ʧ��
 */
static int ViewRuleMemory(char cInd)
{
    short       nCurrSecId;
    short       nBackSecId;

    nCurrSecId = gptShmBuf->nSecId;
    nBackSecId = (gptShmBuf->nSecId+1)%N_MAX_SEC;
    
    printf("sizeofshm:[%d], nDetectMode=[%d], nCurrSecId=[%d], nBackSecId=[%d]\n", \
            sizeof(*gptShmBuf), gptShmBuf->nDetectMode, nCurrSecId, nBackSecId);

    if (cInd == 'A' || cInd == 'a') {
        printf("[RULE] [Active] Memory Block:\n");
        DumpShmRule(&gptShmBuf->stShmRule[nCurrSecId], stdout);
    } else if (cInd == 'S' || cInd == 's') {
        printf("[RULE] [Standby] Memory Block:\n");
        DumpShmRule(&gptShmBuf->stShmRule[nBackSecId], stdout);
    } else {
        printf("Unrecognized Choice:[%c]\n", cInd);
        return -1;
    }
    
    return 0;
}




/** 
 *  @brief �÷�����
 *  @param[in]  ��
 *  @param[out] ��
 *  @return ��
 */
static void usage(void)
{
    printf("usage: afdipc [-? -C -R -L -V]");
    printf("\n"
"             -? help\n"
"             -C Create Share Memory\n"
"             -L Load Share Memory\n"
"             -R Remove Share Memory\n"
"             -V View Share Memory\n");
}
