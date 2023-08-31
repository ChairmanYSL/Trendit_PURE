#include <sys/stat.h>
//#include <unistd.h>
#include <stdio.h>
//#include <stdlib.h>

#include <android/log.h>
#include "sdkmaths.h"
#include "devApi.h"
#include "sdktypedef.h"
#include "sdkDebug.h"
#include "extern.h"


#define  LOG_TAG    "PUREEMVCORE"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

extern bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen);

u8 config_dir[64] = {0};

int sdkGetRtc(unsigned char *pbcDest)
{
    if(NULL == pbcDest)
    {
        return SDK_PARA_ERR;
    }
    ddi_sys_get_time(pbcDest);

    if(sdkIsBcdNum(pbcDest, 6))
    {
        return SDK_OK;
    }
    else
    {
        return SDK_PARA_ERR;
    }
}

void sdkmSleep(const int siMs)
{
	usleep(siMs);
}

int sdkGetRandom(unsigned char *pheRdm, int siNum)
{
	u16 outlen;
	ddi_manage_get_random((u16)siNum, pheRdm,&outlen);
	TraceHex("ddi", "random:", pheRdm, outlen);
    return SDK_OK;
}

unsigned short sdkReadPosSn(unsigned char *pasDest)
{
	u16 outlen=64;
    if(pasDest == NULL)
    {
        return SDK_PARA_ERR;
    }

	ddi_manage_get_sn(pasDest, &outlen);
    return outlen;
}

void *sdkGetMem(unsigned int size)
{
	return ddi_k_malloc(size);
}

int sdkFreeMem(void *ap )
{
    ddi_k_free(ap);
    return 1;
}

int sdkSysSetCurAppDir(unsigned char *pasDir, int len)
{
	if(NULL == pasDir)
	{
		return SDK_PARA_ERR;
	}
	Trace("extern", "pasDir: %s\r\n", pasDir);
	Trace("extern", "input len: %d\r\n", len);
	memset(config_dir, 0, sizeof(config_dir));
	memcpy(config_dir, pasDir, len);
	return SDK_OK;
}

int sdkSysGetCurAppDir(unsigned char *pasData)
{
    if(NULL == pasData) //shijianglong 2013.01.30 15:58
    {
        return SDK_PARA_ERR;
    }
    strcpy(pasData, "/sdcard/pure/");
    return SDK_OK;
}

s32 sdkDevContactlessSendAPDU(const u8 *pheInBuf, u16 siInLen, u8 *pheOutBuf, u16 *psiOutLen)
{
    s32 ret = 0;
	u32 size = 512;
//	u32 sw = 0;

    if((NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) || siInLen < 0)
    {
        return SDK_PARA_ERR;
    }

	TraceHex("apdu", "pheInBuf:", pheInBuf, siInLen);

	ret = ddi_icc_trans_apdu(CARD_TYPE_CLCPU, pheInBuf, siInLen, pheOutBuf, psiOutLen);

	Trace("extern", "ddi_icc_trans_apdu ret = %d\r\n", ret);

    if(ret == DDI_OK)
    {
		Trace("apdu", "psiOutLen = %d\r\n", *psiOutLen);
    	TraceHex("emv", "contactless r-apdu:", pheOutBuf, *psiOutLen);
        return SDK_OK;
    }

    return SDK_ERR;
}


#define DDI_OK 0

#define SDK_MAX_PATH 40

s32 sdkInsertFile(const u8 *pasFile, const u8 *pheSrc, s32 siStart, s32 siSrclen)
{
	s32 i,ret;
	FILE *fp;

    if (NULL == pasFile || NULL == pheSrc || siStart < 0 || siSrclen < 0) {
		Trace("ddi", "Input Param invalid\r\n");
		return SDK_PARA_ERR;
    }

    if (siStart != 0)
	{
        i = sdkGetFileSize(pasFile);
        if (siStart > i && i > 0)
        {
            return SDK_PARA_ERR;
   		}
    }
//	Trace("ddi", "want open file:%s", pasFile);
	fp = fopen(pasFile, "rb+");
	Trace("ddi", "want open file: %s\r\n", pasFile);
	if((NULL == fp) && (sdkGetFileSize(pasFile) <= 0))//file doesn't exist
	{
		fp = fopen(pasFile, "wb+");
		if(fp == NULL)
		{
			Trace("ddi", "open file error\r\n");
			return SDK_ERR;
		}
		else
		{
			fclose(fp);
			fp = fopen(pasFile, "rb+");
		}
	}
	fseek(fp, 0, SEEK_END);
	i = ftell(fp);
	if (siStart > i)
	{
		fclose(fp);
		return SDK_PARA_ERR;
	}
	fseek(fp, siStart, SEEK_SET);

	ret = fwrite(pheSrc, sizeof(u8), siSrclen, fp);
	if(ret == siSrclen)
	{
		fclose(fp);
		return SDK_OK;
	}
	else
	{
		fclose(fp);
		return SDK_ERR;
	}
}

s32 sdkReadFile(const u8 *pasFile, u8 *pheDest, s32 siOffset, s32 *psiDestlen)
{
//	s32 fp=0;
	FILE *fp = NULL;
	u32 i;

    if (NULL == pasFile || NULL == pheDest || NULL == psiDestlen || siOffset < 0)
	{
        return SDK_PARA_ERR;
    }

	fp = fopen(pasFile, "rb");
	if(fp == NULL)
	{
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

    if( 0 != fseek(fp, siOffset, SEEK_SET) )
    {
        fclose(fp);
        return SDK_ERR;
    }

	i = *psiDestlen;
	*psiDestlen = fread(pheDest, sizeof(u8), i, fp);
	if(*psiDestlen != i)
	{
		fclose(fp);
		return SDK_ERR;
	}
	else
	{
		fclose(fp);
		return SDK_OK;
	}
}

s32 sdkGetFileSize(const u8 *pasFile)
{
	s32 fileSize;
//	s32 fp;
	FILE *fp=NULL;

	if (NULL == pasFile)
	{
		return SDK_PARA_ERR;
	}

	fp = fopen(pasFile, "rb");
	if(NULL == fp)
	{
		return SDK_FUN_NULL;																						//�ļ���ʧ��
	}

	fseek(fp, 0, SEEK_END);

	fileSize = ftell(fp);

	fclose(fp);

	return fileSize;
}

s32 sdkDelFile(const u8 *pasFile)
{
	s32 ret;

	if(pasFile == NULL)
	{
		return SDK_PARA_ERR;
	}

	ret = remove(pasFile);
	if(ret == 0)
	{
		return SDK_OK;
	}
	else
	{
		return SDK_ERR;
	}
}

#define DEBUG_FOR_KERNEL
//#undef DEBUG_FOR_KERNEL

void sdkDev_Printf(char *fmt, ...)
{
	va_list args;
	unsigned char printbuffer[256] = {0};

	va_start ( args, fmt );
	vsprintf ( printbuffer, fmt, args );
	va_end ( args );
	LOGD("%s",printbuffer);
}
