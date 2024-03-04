/**
 * @copyright 2017 IndieMicro.
 *
 * This file is proprietary to IndieMicro.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of IndieMicro.
 *
 * @file pmu_device.h
 */

#ifndef PMU_DEVICE_H__
#define PMU_DEVICE_H__

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <realplumPro.h>

typedef enum{
    POLARITY_NATIVE = 0,
    POLARITY_FLIP,
}BattMonitorPolarity_t;


typedef enum{
  BOR_1V5_THRS_1261mV = 0,
  BOR_1V5_THRS_1290mV,
  BOR_1V5_THRS_1320mV,
  BOR_1V5_THRS_1352mV,
  BOR_1V5_THRS_1385mV,
}Bor1V5Thres_t;


typedef enum{
  BOR_3V3_THRS_2101mV = 0,
  BOR_3V3_THRS_2161mV,
  BOR_3V3_THRS_2225mV,
  BOR_3V3_THRS_2293mV,
  BOR_3V3_THRS_2365mV,
  BOR_3V3_THRS_2442mV,
  BOR_3V3_THRS_2524mV,
  BOR_3V3_THRS_2612mV,
  BOR_3V3_THRS_2706mV,
  BOR_3V3_THRS_2807mV,
  BOR_3V3_THRS_2916mV,
  BOR_3V3_THRS_3034mV,
  BOR_3V3_THRS_3162mV,
}Bor3V3Thres_t;

/**
 * @brief The enumeration of brownout mode.
 */
typedef enum {
    PMU_BROWNOUT_RESET = 0U,
    PMU_BROWNOUT_INTERRUPT,
    PMU_BROWNOUT_NONE,
} PMU_BROWNOUT_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_DISABLE = 0U,
    WAKEUP_TIMEER_ENABLE,
} PMU_WAKEUP_TIMEER_MODE_t;

/**
 * @brief The enumeration of wake up timer mode.
 */
typedef enum {
    WAKEUP_TIMEER_INTERVAL_100us = 0U,
    WAKEUP_TIMEER_INTERVAL_200us,
    WAKEUP_TIMEER_INTERVAL_400us,
    WAKEUP_TIMEER_INTERVAL_800us,
    WAKEUP_TIMEER_INTERVAL_16ms,
    WAKEUP_TIMEER_INTERVAL_32ms,
    WAKEUP_TIMEER_INTERVAL_64ms,
    WAKEUP_TIMEER_INTERVAL_128ms,
    WAKEUP_TIMEER_INTERVAL_256ms,
    WAKEUP_TIMEER_INTERVAL_512ms,
    WAKEUP_TIMEER_INTERVAL_1024ms,
    WAKEUP_TIMEER_INTERVAL_2048ms,
    WAKEUP_TIMEER_INTERVAL_4096ms,
    WAKEUP_TIMEER_INTERVAL_8192ms,
    WAKEUP_TIMEER_INTERVAL_16384ms,
    WAKEUP_TIMEER_INTERVAL_32768ms
} PMU_WAKEUP_TIMEER_Interval_t;
void PMU_BORInit(Bor1V5Thres_t lowThreshold, Bor3V3Thres_t highThreshold);
void PMU_WakeTimerInit(PMU_WAKEUP_TIMEER_MODE_t mode, PMU_WAKEUP_TIMEER_Interval_t interval);
void PMU_EnterDeepSleepMode(void);




#endif /* __PMU_DEVICE_H__ */
