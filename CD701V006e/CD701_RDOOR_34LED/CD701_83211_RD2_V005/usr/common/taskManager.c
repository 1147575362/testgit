/**
 * @copyright 2017 Indie Semiconductor.
 *
 * This file is proprietary to Indie Semiconductor.
 * All rights reserved. Reproduction or distribution, in whole
 * or in part, is forbidden except by express written permission
 * of Indie Semiconductor.
 *
 * @file taskManager.c
 * @Author: Jack.Pan
 * @E-mail:jack.pan@indiemicro.com
 * @Date: 2020/03/18
 */

#include <taskManager.h>

#include <SoftTimerTask.h>
#include <colorMixingTask.h>
#include <applicationTask.h>
#include <linslaveTask.h>
#include <pdsTask.h>
#include <linmasterTask.h>
#include <linStackTask.h>
#include <measureTask.h>
#include <colorCompensationTask.h>
#include <safetyMonitorTask.h>
#include <wdt_device.h>

static volatile uint16_t taskFlags = 0U;
static uint16_t tasksMask = 0xFFFFU;
void IdleTask(void);

typedef void (*taskHandler_t)(void);

static taskHandler_t taskHandlers[] = {
    [TASK_ID_SOFT_TIMER]        = SoftTimer_TaskHandler,/* Global Timer task */
    [TASK_ID_LIN_STACK]         = LinStack_TaskHandler, /*  LIN stack task   */
    [TASK_ID_LINS]              = LINS_TaskHandler,     /*  LIN Slave task   */
    [TASK_ID_PDS]               = PDS_TaskHandler,      /*!< Task ID of persistance data storage*/
#if LIN_MASTER_EN == 1U
    [TASK_ID_LINM]              = LINM_TaskHandler,     /*  LIN Master task */
#endif
    [TASK_ID_COLOR_MIXING]      = CLM_TaskHandler,      /*  Color mixing and control task*/
    [TASK_ID_ADC_MEASURE]       = MES_TaskHandler,      /* ADC measurement, chip temperature, Led PN volt, Battery Volt etc. */
    [TASK_ID_SAFETY_MONITOR]    = SAFM_TaskHandler,     /* safe monitor task */
    [TASK_ID_COLOR_COMPENSATION]= CCP_TaskHandler,      /*  Color compensation task      */
    [TASK_ID_APPL]              = APPL_TaskHandler,     /*  design for debug and demo    */
};

void IdleTask(void)
{
    
  
}

void TM_RunTasks(void)
{
    uint8_t taskId;
    for (taskId = 0U; taskId < (uint8_t)(sizeof(taskHandlers) / sizeof(taskHandlers[0])); taskId++){
        if ( (((uint16_t)1UL << taskId) & (taskFlags & tasksMask)) != 0U ){
            __atomic_enter()
                taskFlags &= ~((uint16_t)1UL << taskId);
            __atomic_exit()
            if (taskHandlers[taskId] != NULL){
                taskHandlers[taskId]();
            }
            break;
        }
    }
    
#if WATCH_DOG_EN == 1U
    WDTA_Clear(); /* Feeding Watch dog */
#endif
    
    if (taskFlags == 0U){
        IdleTask();
    }
}

/*
 *  Disables specified task.
 *  @param [in] taskId - task to be disabled.
 *  @return none
 */
void TM_DisableTask(TM_TaskId_t taskId)
{
    tasksMask &= ~(uint16_t)(1UL << taskId);
}

/*
 *  brief Enables specified task.
 *  @param [in] taskId - task to be enabled.
 *  @return none
 */
void TM_EnableTask(TM_TaskId_t taskId)
{
    tasksMask |= (uint16_t)(1UL << taskId);
}

/*
 *  brief runs specified task.
 *  @param [in] taskId - task to be run.
 *  @return none
 */
void TM_PostTask(TM_TaskId_t taskId)
{
    __atomic_enter()
      taskFlags |= (uint16_t)(1UL << taskId);
    __atomic_exit()
}

uint8_t TM_TaskIsPending(TM_TaskId_t taskId)
{
    uint16_t flags =  taskFlags;
    uint16_t tasks = (uint16_t)1U << taskId;
    if ( (tasks & flags)!= 0U ){
        return 1U;
    }else{
        return 0U;
    }   
}