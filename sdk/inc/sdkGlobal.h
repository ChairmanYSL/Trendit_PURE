#ifndef AFX_SDK_GLOBAL_H
#define AFX_SDK_GLOBAL_H

#define SDK_OK                       (1)
#define SDK_EQU                      (0)        //�Ƚϴ�С���ʱ�����//use this if it's equal when compare size
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)       //��ʱ//timeout
#define SDK_ESC                      (-3)
#define SDK_PARA_ERR                 (-4)
#define SDK_FUN_NULL                 (-5)       //������Ϊ��
#define SDK_EBUSY                    (-6)       /* �豸æ */
#define SDK_EIO                      (-7)       /* �豸δ�� */
#define SDK_EDATA                    (-8)       /* ������������ݴ��� */

#include <stdio.h>
#include <string.h>
#include "sdktypedef.h"
#include "sdkDebug.h"
#include "sdkmaths.h"
#include "sdkemvbase.h"

#ifdef assert
#undef  assert
#endif

#endif
