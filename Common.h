#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define uint32_t unsigned int
#define bool unsigned char
#define uint16_t unsigned short
#define uint8_t unsigned char

#define LOG_INFO printf
#define LOG_CRITICAL printf

#define ENABLE  1
#define DISABLE 0

#define TRUE  1
#define FALSE 0

typedef enum
{
    FuncRet_Success = 0,
    FuncRet_Fail    = 1
}FuncRet_e;

#endif