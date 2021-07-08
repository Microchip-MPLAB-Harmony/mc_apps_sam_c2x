/*******************************************************************************
 Motor Control FlyingStart interface file

  Company:
    Microchip Technology Inc.

  File Name:
    q14_flying_start_mcLib.h

  Summary:
    Motor control FlyingStart interface

  Description:
    This file contains the data structures and function prototypes of motor control flying start technique.
 *******************************************************************************/
/* ************************************************************************** */

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2020 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END 

#ifndef _Q14_FLYING_START_MCLIB_H    /* Guard against multiple inclusion */
#define _Q14_FLYING_START_MCLIB_H

#include <stdint.h>
#include "config/mclv2_sam_c21_pim/userparams.h"
#include "q14_generic_mcLib.h"
#include "mc_app.h"
#include "config/mclv2_sam_c21_pim/peripheral/tcc/plib_tcc0.h"
typedef enum
{
    MC_FLYING_START_IN_PROGRESS,
    MC_FLYING_START_DETECTED,
    MC_FLYING_START_NOT_DETECTED
}tMC_FLYING_START_STATUS_E;

typedef enum
{
    MC_FLYING_START_DETECT,
    MC_FLYING_START_DECIDE,
    MC_FLYING_START_CLOSING_LOOP,        
    MC_FLYING_START_PASSIVE_BRAKE,
    MC_FLYING_START_COMPLETED
}tMCCTRL_FLYING_START_CONTROL_STATE_E;

typedef struct
{
    uint32_t                                flying_startTimeCounter;
    uint32_t                                flying_startTimeCount;
    uint16_t                                minFlyingStartSpeed;
    uint32_t                                passiveBrakeTimeCount;
    uint32_t                                passiveBrakeTimeCounter;
    uint16_t                                angleRolloverCountThreshold;
    int16_t                                 flying_startStartCurrent;
    int16_t                                 regenBrakeCurrent;
    int16_t                                 regenBrakeCurrentStep;
    tMCCTRL_FLYING_START_CONTROL_STATE_E     mcFlyingStartState;
}tMCCTRL_FLYING_START_STATE_SIGNALS_S;

extern tMCCTRL_FLYING_START_STATE_SIGNALS_S  gMCCTRL_FlyingStartStateSignals;

tMC_FLYING_START_STATUS_E MCCTRL_FlyingStartControl(void);

/*****************************************************************************/ 
/* Function name: MCCTRL_InitilaizeFlyingStartControl                        */
/* Function parameters: None                                                 */
/* Function return: None                                                     */
/* Description: Initialize FlyingStart parameters and state                  */
/*****************************************************************************/

static __inline__ void MCCTRL_InitializeFlyingStartControl( void )
{
    /* Initialize FlyingStart parameters */
    gMCCTRL_FlyingStartStateSignals.flying_startTimeCounter = 0u;
    gMCCTRL_FlyingStartStateSignals.passiveBrakeTimeCounter = 0u;
    gMCCTRL_FlyingStartStateSignals.passiveBrakeTimeCount = BRAKING_TIME_IU;
    gMCCTRL_FlyingStartStateSignals.flying_startTimeCount = FLYING_START_TIME_IU;
    gMCCTRL_FlyingStartStateSignals.minFlyingStartSpeed = MIN_FLYING_START_SPE;
    gMCCTRL_FlyingStartStateSignals.flying_startStartCurrent = FLYING_START_CUR;
    gMCCTRL_FlyingStartStateSignals.regenBrakeCurrent = REGEN_BRAKE_CURRENT;
    gMCCTRL_FlyingStartStateSignals.regenBrakeCurrentStep = RGN_BRK_RAMP_IU;
    gMCCTRL_FlyingStartStateSignals.mcFlyingStartState = MC_FLYING_START_DETECT;
    gMCCTRL_FlyingStartStateSignals.angleRolloverCountThreshold = HALF_MIN_ANGLE_ROLLOVER;
}

/*****************************************************************************/
/* Function name: MCCTRL_ResetFlyingStartControl                             */
/* Function parameters: None                                                 */
/* Function return: None                                                     */
/* Description: Reset FlyingStart parameters                                 */
/*****************************************************************************/
static __inline__ void MCCTRL_ResetFlyingStartControl( void )
{
    /* Reset FlyingStart state */
    gMCCTRL_FlyingStartStateSignals.flying_startTimeCounter = 0u;
    gMCCTRL_FlyingStartStateSignals.passiveBrakeTimeCounter = 0u;
    trigger = 0;
    gMCCTRL_FlyingStartStateSignals.mcFlyingStartState = MC_FLYING_START_DETECT;
}

#endif