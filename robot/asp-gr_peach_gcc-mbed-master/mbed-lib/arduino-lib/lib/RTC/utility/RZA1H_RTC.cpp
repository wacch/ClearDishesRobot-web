/***************************************************************************
 *
 * PURPOSE
 *   RTC(Real Time Clock) function module file.
 *
 * TARGET DEVICE
 *   RX63N
 *
 * AUTHOR
 *   Renesas Electronics Corp.
 *
 *
 ***************************************************************************
 * Copyright (C) 2014 Renesas Electronics. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * See file LICENSE.txt for further informations on licensing terms.
 ***************************************************************************/
/**
 * @file  RZA1H_RTC.cpp
 * @brief RZ/A1H�}�C�R�������̎��v�@�\�iRTC�F���A���E�^�C���E�N���b�N�j���g�����߂̃��C�u�����ł��B
 *
 * RTC�N���X�͂��̃��C�u������C++�ŃJ�v�Z�������Ďg���₷���������̂ł��B
 *
 * Modified 27th May 2014 by Yuuki Okamiya from RL78duino.cpp
 * Modified 22nd June 2016 by Nozomu Fujita for GR-PEACH
 */

/***************************************************************************/
/*    Include Header Files                                                 */
/***************************************************************************/
#include "RTC.h"
#define rtc_init _rtc_init
#include "rtc_api.h"
#undef rtc_init
#include "RZA1H_RTC.h"
#include "RZA1H_RTC_private.h"
#include "wiring_private.h"
#include "rtc_iodefine.h"
#define RCR1_VAL_ON      (0x08u) // AIE = 1
#define RCR1_AF_ON       (0x01u) // AF = 1
#define ENB              (0x80u)


/***************************************************************************/
/*    Macro Definitions                                                    */
/***************************************************************************/


/***************************************************************************/
/*    Type  Definitions                                                    */
/***************************************************************************/


/***************************************************************************/
/*    Function prototypes                                                  */
/***************************************************************************/
static void INT_Excep_RTC_ALM(void);
static uint16_t HEX2BCD(int s16HEX);


/***************************************************************************/
/*    Global Variables                                                     */
/***************************************************************************/



/***************************************************************************/
/*    Local Variables                                                      */
/***************************************************************************/
static fInterruptFunc_t g_fRTCInterruptFunc = NULL;
static bool setAlarmHandler = false;
static bool setAlarmTime = false;
static uint32_t last1msecCounter = 0;

/***************************************************************************/
/*    Global Routines                                                      */
/***************************************************************************/
/** ************************************************************************
 * @addtogroup group101
 *
 * @{
 ***************************************************************************/
/** ************************************************************************
 * @defgroup group12 ���v�@�\
 *
 * @{
 ***************************************************************************/
/**
 * RTC�̃n�[�h�E�F�A�����������܂��B
 *
 * @retval 0�FRTC�̋N���Ɏ��s���܂����B
 * @retval 1�FRTC�̋N���ɐ������܂����B
 * @retval 2�FRTC�͊��ɋN�����Ă��܂��B
 *
 * @attention �Ȃ�
 ***************************************************************************/
int rtc_init()
{
    if (rtc_isenabled()) {
        return 2;
    }
    rtc_init_indirectly();
    return 1;
}


/**
 * RTC���~���܂��B
 *
 * @retval 0�FRTC�̒�~�Ɏ��s���܂����B
 * @retval 1�FRTC�̒�~�ɐ������܂����B
 *
 * @attention �Ȃ�
 ***************************************************************************/
int rtc_deinit()
{
    rtc_free();
    return 1;
}


/**
 * RTC�̎��Ԃ�ݒ肵�܂��B
 *
 * @param[in] time �ݒ肷�鎞�����i�[���ꂽ�\���̂��w�肵�܂��B
 *
 * @retval 0�F�����̐ݒ�Ɏ��s���܂����B
 * @retval 1�F�����̐ݒ�ɐ������܂����B
 *
 * @attention �����̒l��BCD�ł͂���܂���B�܂��Artc_init()�֐��������I�ɌĂяo���ARTC�����������܂��B
 ***************************************************************************/
int rtc_set_time(RTC_TIMETYPE* time)
{
    struct tm t;
    t.tm_sec = time->second;
    t.tm_min = time->min;
    t.tm_hour = time->hour;
    t.tm_mday = time->day;
    t.tm_mon = time->mon - 1;
    t.tm_year = time->year - 1900;
    t.tm_wday = time->weekday;
    t.tm_yday = 0;
    t.tm_isdst = 0;
    rtc_write(mktime(&t));
    return 1;
}


/**
 * RTC�̎��Ԃ��擾���܂��B
 *
 * @param[out] time �������i�[����\���̂��w�肵�܂��B
 *
 * @retval 0�F�����̎擾�Ɏ��s���܂����B
 * @retval 1�F�����̎擾�ɐ������܂����B
 *
 * @attention �����̒l��BCD�ł͂���܂���B
 ***************************************************************************/
int rtc_get_time(RTC_TIMETYPE* time)
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    time->year    = timeinfo->tm_year + 1900;
    time->mon     = timeinfo->tm_mon + 1;
    time->day     = timeinfo->tm_mday;
    time->hour    = timeinfo->tm_hour;
    time->min     = timeinfo->tm_min;
    time->second  = timeinfo->tm_sec;
    time->weekday = timeinfo->tm_wday;
    return 1;
}

unsigned short rtc_get_year()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_year + 1900;
}

unsigned char rtc_get_mon()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_mon + 1;
}

unsigned char rtc_get_day()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_mday;
}

unsigned char rtc_get_hour()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_hour;
}

unsigned char rtc_get_min()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_min;
}

unsigned char rtc_get_second()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_sec;
}

unsigned char rtc_get_weekday()
{
    time_t t = rtc_read();
    struct tm* timeinfo = localtime(&t);
    return timeinfo->tm_wday;
}


/**
 * �A���[�����Ɏ��s����n���h����o�^���܂��B
 *
 * @param[in] fFunction �A���[�����Ɏ��s����n���h�����w�肵�܂��B
 *
 * @return �Ȃ�
 *
 * @attention �Ȃ�
 ***************************************************************************/
void rtc_attach_alarm_handler(void (*fFunction)(void))
{
    GIC_DisableIRQ(ARM_IRQn);
    g_fRTCInterruptFunc = fFunction;
    if (fFunction != 0) {
        InterruptHandlerRegister(ARM_IRQn, INT_Excep_RTC_ALM);
        GIC_SetPriority(ARM_IRQn, 5);
        GIC_EnableIRQ(ARM_IRQn);
        RTC.RCR1 &= ~RCR1_AF_ON;
        rtc_alarm_on();
        setAlarmHandler = true;
    } else {
        setAlarmHandler = false;
    }
}


/**
 * �A���[�����Ԃ�ݒ肵�܂��B
 *
 * @param[in] hour      �����w�肵�܂��B
 * @param[in] min       �����w�肵�܂��B
 * @param[in] week_flag �j�����w�肵�܂��B�����̗j�����w�肷��ꍇ�͘_���a�Őڑ����܂��B
 *
 * @retval 0�F�A���[���̐ݒ�Ɏ��s���܂����B
 * @retval 1�F�A���[���̐ݒ�ɐ������܂����B
 *
 * @attention �����̒l��BCD�ł͂���܂���B
 ***************************************************************************/
int rtc_set_alarm_time(int hour, int min, int week_flag)
{
    rtc_alarm_off();
    RTC.RHRAR = ENB | 0;
    RTC.RMINAR = ENB | 0;
    RTC.RSECAR = ENB | 0;
    RTC.RYRAR = 0;
    RTC.RCR3 = 0;
    RTC.RMONAR = 0;
    RTC.RDAYAR = 0;
    RTC.RWKAR = 0;

    if (hour >= 0 && hour <= 23) {
        RTC.RHRAR = ENB | HEX2BCD(hour);
    } else {
        return 0;
    }
    if (min >= 0 && min <= 59) {
        RTC.RMINAR = ENB | HEX2BCD(min);
    } else {
        return 0;
    }
    if (week_flag >= RTC_ALARM_SUNDAY && week_flag <= RTC_ALARM_SATURDAY) {
        RTC.RWKAR = ENB | week_flag;
    } else if (week_flag == RTC_ALARM_EVERYDAY) {
        RTC.RWKAR = 0;
    } else {
        return 0;
    }

    setAlarmTime = true;
    rtc_alarm_on();
    return 1;
}


/**
 * �A���[����ON�ɂ��܂��B
 *
 * @return �Ȃ�
 *
 * @attention �Ȃ�
 ***************************************************************************/
void rtc_alarm_on()
{
    if (setAlarmHandler && setAlarmTime) {
        last1msecCounter = get1msecCounter() - 1000;
        RTC.RCR1 |= RCR1_VAL_ON;
    }
}

/**
 * �A���[����OFF�ɂ��܂��B
 *
 * @return �Ȃ�
 *
 * @attention �Ȃ�
 ***************************************************************************/
void rtc_alarm_off()
{
    RTC.RCR1 &= ~RCR1_VAL_ON;
}

/// @cond
/**
 * �A���[���̊��荞�݃n���h���ł��B
 *
 * @return �Ȃ�
 *
 * @attention �Ȃ�
 ***************************************************************************/
static void INT_Excep_RTC_ALM(void){
    uint32_t current1msecCounter = get1msecCounter();
    if (current1msecCounter - last1msecCounter > 1000) {
        if (g_fRTCInterruptFunc != NULL) {
            (*g_fRTCInterruptFunc)();
        }
    }
    last1msecCounter = current1msecCounter;
    /* Clear the interrupt flag */
    do {
        RTC.RCR1 &= ~RCR1_AF_ON;
    } while (RTC.RCR1 & RCR1_AF_ON);
}

/***************************************************************************/
/*    Local Routines                                                       */
/***************************************************************************/
static uint16_t HEX2BCD(int s16HEX)
{
    uint16_t bcd = 0;
    bcd += s16HEX / 1 % 10 * 0x1;
    bcd += s16HEX / 10 % 10 * 0x10;
    bcd += s16HEX / 100 % 10 * 0x100;
    bcd += s16HEX / 1000 % 10 * 0x1000;
    return bcd;
}


/***************************************************************************/
/* End of module                                                           */
/***************************************************************************/
