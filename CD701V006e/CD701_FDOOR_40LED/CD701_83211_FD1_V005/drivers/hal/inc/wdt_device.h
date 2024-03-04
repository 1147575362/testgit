/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file wdt_device.h
 */

#ifndef WDT_DEVICE_H__
#define WDT_DEVICE_H__

#include <stdint.h>
#include <wdt1_sfr.h>
#include <wdta_sfr.h>


typedef void (*wdt_cb_func_t)(void);

typedef enum{
  WDT1_INTERVAL_E13 = 0U,/*0x0: 2^13 / System Clock  512us @16MHz*/
  WDT1_INTERVAL_E19,    /*0x1: 2^19 / System Clock  32768us @16MHz*/
  WDT1_INTERVAL_E22,    /*0x2: 2^22 / System Clock  262144us @16MHz*/
  WDT1_INTERVAL_E32,    /*0x3: 2^32 / System Clock  268seconds @@16MHz*/
}WDT1Interval_t;

typedef enum{
  WDT1_MODE_RESET = 0U,
  WDT1_MODE_INTERRUPT
}WDT1Mode_t;

void WDT1_Enable(WDT1Mode_t mode, WDT1Interval_t interval, wdt_cb_func_t callback);
void WDT1_Clear(void);

typedef enum{
  WDTA_INTERVAL_128MS = 0U,/*0x0: 2^11 * 62.5us ~= 128 ms*/
  WDTA_INTERVAL_256MS,     /*0x1: 2^12 * 62.5us ~= 256 ms*/
  WDTA_INTERVAL_512MS,     /*0x2: 2^13 * 62.5us ~= 512 ms*/
  WDTA_INTERVAL_1S,        /*0x3: 2^14 * 62.5us ~= 1.0 s*/
  WDTA_INTERVAL_2S,        /*0x4: 2^15 * 62.5us ~= 2.0 s*/
  WDTA_INTERVAL_4S,        /*0x5: 2^16 * 62.5us ~= 4.0 s*/
  WDTA_INTERVAL_8S,        /*0x6: 2^17 * 62.5us ~= 8.0 s*/
  WDTA_INTERVAL_16S,       /*0x7: 2^18 * 62.5us ~=16.0 s*/
}WDTAInterval_t;

typedef enum{
  WDTA_MODE_RESET = 0U,
  WDTA_MODE_INTERRUPT
}WDTAMode_t;


void WDTA_Enable(WDTAInterval_t interval);
void WDTA_Disable(void);
void WDTA_Clear(void);




#endif /* __WDT_DEVICE_H__ */

