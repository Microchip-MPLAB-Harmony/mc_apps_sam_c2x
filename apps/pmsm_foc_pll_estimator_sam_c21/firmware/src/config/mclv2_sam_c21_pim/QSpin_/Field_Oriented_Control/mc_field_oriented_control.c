/*******************************************************************************
 Field Oriented Control ( FOC ) source file

  Company:
    - Microchip Technology Inc.

  File Name:
    - mc_torque_control.c

  Summary:
    - Field Oriented Control ( FOC ) source file

  Description:
    - This file implements functions for Field Oriented Control ( FOC )

 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2022 Microchip Technology Inc. and its subsidiaries.
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

/*******************************************************************************
Headers inclusions
*******************************************************************************/
#include "mc_field_oriented_control.h"

/*******************************************************************************
Local configuration options
*******************************************************************************/
#define ONE_BY_SQRT3   (int16_t)( 9459 )
#define TWO_BY_SQRT3   (int16_t)( 18919 )
#define OPEN_TO_CLOSE_ANGLE_STEP_SIZE   (uint16_t)(10)

/*******************************************************************************
 Private data types
*******************************************************************************/
typedef enum
{
    FocState_FlyingStart,
    FocState_Startup,
    FocState_ClosingLoop,
    FocState_CloseLoop
}tmcFoc_FocState_e;

typedef struct
{
    bool enable;
    bool initDone;
    tmcFoc_FocState_e FocState;
    tmcTypes_AlphaBeta_s iAlphaBeta;
    tmcTypes_AlphaBeta_s uAlphaBeta;
    tmcTypes_DQ_s iDQ;
    tmcTypes_DQ_s uDQ;
    uint16_t openLoopAngle;
    int16_t openLoopSpeed;
    uint16_t closeLoopAngle;
    int16_t closeLoopSpeed;
    uint16_t angleDifference;
    int16_t iQref;
    int16_t iDref;
    int16_t nRef;
    int16_t commandDirection;
    uint16_t duty[3u];
    tmcSup_Parameters_s bOpenLoopStartup;
    tmcPwm_Parameters_s bPwmModulator;
    tmcFlx_Parameters_s bFluxController;
    tmcTor_Parameters_s bTorqueController;
    tmcSpe_Parameters_s bSpeedController;
    tmcRef_Parameters_s bReferenceController;
    tmcRpe_Parameters_s bPositionEstimation;
}tmcFoc_State_s;

/*******************************************************************************
Private variables
*******************************************************************************/
static tmcFoc_State_s mcFoc_State_mds;

/*******************************************************************************
Interface  variables
*******************************************************************************/
tmcFocI_ModuleData_s mcFocI_ModuleData_gds;

/*******************************************************************************
Macro Functions
*******************************************************************************/
#define MULT_SHIFT     mcUtils_MultAndRightShiftS16

/*******************************************************************************
Private Functions
*******************************************************************************/
/*! \brief Clarke Transformation
 *
 * Details.
 * Clarke Transformation
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
__STATIC_FORCEINLINE void mcFoc_ClarkeTransformation( const tmcTypes_ABC_s * pABC,
                                                                                       tmcTypes_AlphaBeta_s * const pAlphaBeta )
{
    /** ToDO: Implement clipping */
    /** Temporary variables used to store intermediate results */
    int16_t  s16a, s16b;

    /** Calculating alpha-axis component  */
    pAlphaBeta->alpha = pABC->a;

    /** Intermediate product is calculated by (1/(sqrt(3)) * a) */
    s16a = (int16_t)MULT_SHIFT(pABC->a, ONE_BY_SQRT3, 14u );

    /** Intermediate product is calculated by (2/sqrt(3) * b) */
    s16b = (int16_t)MULT_SHIFT( pABC->b, TWO_BY_SQRT3, 14u);

    /** Calculate beta-axis component by adding the intermediate products */
    pAlphaBeta->beta = (int16_t)(s16a + s16b);
}

/*! \brief Park Transformation
 *
 * Details.
 * Park Transformation
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
__STATIC_FORCEINLINE void mcFoc_ParkTransformation( const tmcTypes_AlphaBeta_s * const pAlphaBeta,
                                            const int16_t sine, const int16_t cosine,
                                            tmcTypes_DQ_s * const pDQ )
{
    /** ToDO: Implement clipping */
    /** Temporary variables used to store intermediate results */
    int16_t s16a, s16b;

    /** Intermediate product is calculated by (pAlphaBeta->alpha * cosVal) */
    s16a = (int16_t)MULT_SHIFT( pAlphaBeta->alpha, cosine, 14u);

    /** Intermediate product is calculated by (pAlphaBeta->beta * sinVal) */
    s16b = (int16_t)MULT_SHIFT( pAlphaBeta->beta, sine, 14u);

    /** Calculate pId by adding the two intermediate s16a and s16b */
    pDQ->d = s16a + s16b;

    /** Intermediate product is calculated by (pAlphaBeta->alpha * sinVal) */
    s16a = (int16_t)MULT_SHIFT( pAlphaBeta->alpha, sine, 14u);

    /** Intermediate product is calculated by (pAlphaBeta->beta * cosVal) */
    s16b = (int16_t)MULT_SHIFT( pAlphaBeta->beta, cosine, 14u);

    /** Calculate pIq by subtracting the two intermediate s16a from s16b */
    pDQ->q = s16b - s16a;
}

/*! \brief Inverse Park Transformation
 *
 * Details.
 * Inverse Park Transformation
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
__STATIC_FORCEINLINE void mcFoc_InverseParkTransformation( const tmcTypes_DQ_s * const pDQ,
                                                        const int16_t sine, const int16_t cosine,
                                                        tmcTypes_AlphaBeta_s * const pAlphaBeta )
{
    /** ToDO: Implement clipping */
    /** Temporary variables used to store intermediate results */
    int16_t s16a, s16b;

    /** Intermediate product is calculated by (pAlphaBeta->alpha * cosVal) */
    s16a = (int16_t)MULT_SHIFT( pDQ->d, cosine,  14u);

    /** Intermediate product is calculated by (pAlphaBeta->beta * sinVal) */
    s16b =(int16_t)MULT_SHIFT( pDQ->q, sine, 14u);

    /** Calculate pId by adding the two intermediate s16a and s16b */
    pAlphaBeta->alpha = s16a - s16b;

    /** Intermediate product is calculated by (pAlphaBeta->alpha * sinVal) */
    s16a = (int16_t)MULT_SHIFT( pDQ->d, sine, 14u );

    /** Intermediate product is calculated by (pAlphaBeta->beta * cosVal) */
    s16b = (int16_t)MULT_SHIFT( pDQ->q, cosine, 14u );

    /** Calculate pIq by subtracting the two intermediate s16a from s16b */
    pAlphaBeta->beta = s16a + s16b;
}

/*******************************************************************************
 * Interface Functions
*******************************************************************************/
/*! \brief Initialize Field Oriented Control ( FOC ) module
 *
 * Details.
 * Initialize Field Oriented Control ( FOC ) module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFocI_FieldOrientedControlInit( tmcFocI_ModuleData_s * const pModule )
{
    /** Link state variable structure to the module */
    pModule->pStatePointer = (void *)&mcFoc_State_mds;

    /** Set parameters */
    mcFocI_ParametersSet(&pModule->dParameters);

    /** Update state variables */
    mcFoc_State_mds.commandDirection = 1;

    /** Initialize open loop start-up module */
    mcSupI_OpenLoopStartupInit( &mcFoc_State_mds.bOpenLoopStartup );

    /** Initialize reference control module */
    mcRefI_ReferenceControlInit( &mcFoc_State_mds.bReferenceController);

    /** Initialize speed control module */
    mcSpeI_SpeedControlInit( &mcFoc_State_mds.bSpeedController);

    /** Initialize torque control module */
    mcTorI_TorqueControlInit( &mcFoc_State_mds.bTorqueController);

    /** Initialize flux control module */
    mcFlxI_FluxControlInit( &mcFoc_State_mds.bFluxController);

    /** Initialize rotor position estimation  */
    mcRpeI_RotorPositionEstimInit( &mcFoc_State_mds.bPositionEstimation);

    /** Initialize PWM  module */
    mcPwmI_PulseWidthModulationInit( &mcFoc_State_mds.bPwmModulator );

}

/*! \brief Enable Field Oriented Control ( FOC ) module
 *
 * Details.
 * Enable Field Oriented Control ( FOC ) module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFocI_FieldOrientedControlEnable( tmcFocI_ModuleData_s * const pParameters )
{
    /** Get the linked state variable */
    tmcFoc_State_s * pState;
    pState = (tmcFoc_State_s *)pParameters->pStatePointer;

    if( ( NULL == pState ) || ( !pState->initDone ))
    {
        /** Initialize parameters */
        mcFocI_FieldOrientedControlInit(pParameters);
    }
    else
    {
        /** For MISRA Compliance */
    }

    /** Enable open loop start-up module */
    mcSupI_OpenLoopStartupEnable( &mcFoc_State_mds.bOpenLoopStartup );

    /** Enable reference control module */
    mcRefI_ReferenceControlEnable( &mcFoc_State_mds.bReferenceController);

    /** Enable speed control module */
    mcSpeI_SpeedControlEnable( &mcFoc_State_mds.bSpeedController);

    /** Enable torque control module */
    mcTorI_TorqueControlEnable( &mcFoc_State_mds.bTorqueController);

    /** Enable flux control module */
    mcFlxI_FluxControlEnable( &mcFoc_State_mds.bFluxController);

    /** Enable rotor position estimation  */
    mcRpeI_RotorPositionEstimEnable( &mcFoc_State_mds.bPositionEstimation);

    /** Enable PWM  module */
    mcPwmI_PulseWidthModulationEnable( &mcFoc_State_mds.bPwmModulator );

    /** Set FOC state machine */
    mcFoc_State_mds.FocState = FocState_Startup;

    /** Set enable flag as true */
    pState->enable = true;
}

/*! \brief Disable Field Oriented Control ( FOC ) module
 *
 * Details.
 * Disable Field Oriented Control ( FOC ) module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFocI_FieldOrientedControlDisable( tmcFocI_ModuleData_s * const pParameters )
{
    /** Get the linked state variable */
    tmcFoc_State_s * pState;
    pState = (tmcFoc_State_s *)pParameters->pStatePointer;

    if( NULL != pState)
    {
        /** Reset state variables  */
        mcFocI_FieldOrientedControlReset(pParameters);
    }
    else
    {
        /** For MISRA Compliance */
    }

    /** Disable open loop start-up module */
    mcSupI_OpenLoopStartupDisable( &mcFoc_State_mds.bOpenLoopStartup );

    /** Disable reference control module */
    mcRefI_ReferenceControlDisable( &mcFoc_State_mds.bReferenceController);

    /** Disable speed control module */
    mcSpeI_SpeedControlDisable( &mcFoc_State_mds.bSpeedController);

    /** Disable torque control module */
    mcTorI_TorqueControlDisable( &mcFoc_State_mds.bTorqueController);

    /** Disable flux control module */
    mcFlxI_FluxControlDisable( &mcFoc_State_mds.bFluxController);

    /** Disable rotor position estimation  */
    mcRpeI_RotorPositionEstimDisable( &mcFoc_State_mds.bPositionEstimation);

    /** Disable PWM  module */
    mcPwmI_PulseWidthModulationDisable( &mcFoc_State_mds.bPwmModulator );

    /** Set enable flag as true */
    pState->enable = false;

}

/*! \brief Field Oriented Control ( FOC )
 *
 * Details.
 * Field Oriented Control ( FOC )
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
#ifdef RAM_EXECUTE
void __ramfunc__  mcFocI_FieldOrientedControlFast( tmcFocI_ModuleData_s * const pModule )
#else
void mcFocI_FieldOrientedControlFast( tmcFocI_ModuleData_s * const pModule )
#endif
{
    /** Intermediate variables */
    int16_t iQref = 0;
    int16_t iDref = 0;

    int16_t sine = 0;
    int16_t cosine = 0;

    /** Get the linked state variable */
    tmcFoc_State_s * pState;
    pState = (tmcFoc_State_s *)pModule->pStatePointer;

    /** Read FOC inputs  */
    mcFocI_InputsRead( &pModule->dInput );

    /** Clarke transformation */
    mcFoc_ClarkeTransformation( &pModule->dInput.iABC, &pState->iAlphaBeta);

    /** Rotor position estimation */
    mcRpeI_RotorPositionEstim(&pState->bPositionEstimation, &pState->iAlphaBeta,
                                                  &pState->uAlphaBeta, &pState->closeLoopAngle, &pState->closeLoopSpeed );

    switch(pState->FocState )
    {
        case FocState_Startup:
        {
            tmcTypes_StdReturn_e startupStatus;
            startupStatus = mcSupI_OpenLoopStartup( &pState->bOpenLoopStartup, pState->commandDirection, &iQref, &iDref, &pState->openLoopAngle, &pState->openLoopSpeed );

            if( StdReturn_Complete == startupStatus )
            {
                /** Set speed controller state */
                mcSpeI_SpeedControlManual( &pState->bSpeedController, iQref );

                /** Calculate open loop and close loop angle difference */
                pState->angleDifference = pState->closeLoopAngle - pState->openLoopAngle;

                /** Set FOC state machine to ClosingLoop */
                pState->FocState = FocState_ClosingLoop;
                
                pState->nRef = pState->openLoopSpeed;
            }

            /** Sine-cosine calculation */
            mcUtils_SineCosineCalculation( pState->openLoopAngle, &sine, &cosine );

            break;
        }

        case FocState_ClosingLoop:
        {
            uint16_t transitionAngle = 0u;

            transitionAngle = pState->closeLoopAngle - pState->angleDifference;

            /** Sine-cosine calculation */
            mcUtils_SineCosineCalculation( transitionAngle, &sine, &cosine );

            /** Linear ramp */
            UTILS_LinearRamp(&pState->angleDifference, OPEN_TO_CLOSE_ANGLE_STEP_SIZE, 0u);

            /** Switch the FOC state to close loop if angle difference is zero */
            if( pState->angleDifference == 0u )
            {
                /** Set FOC state machine to CloseLoop */
                pState->FocState = FocState_CloseLoop;
            }

            /** Execute speed controller */
            mcSpeI_SpeedControlAuto(&pState->bSpeedController, pState->nRef,
                                pState->closeLoopSpeed, &iQref );
            break;
        }
        case FocState_CloseLoop:
        {
            /** Sine-cosine calculation */
            mcUtils_SineCosineCalculation( pState->closeLoopAngle, &sine, &cosine );

            /** Reference Control */
            mcRefI_ReferenceControl( &mcFoc_State_mds.bReferenceController, pModule->dInput.reference, &pState->nRef );


            /** Execute speed controller */
            mcSpeI_SpeedControlAuto(&pState->bSpeedController, pState->nRef,
                                pState->closeLoopSpeed, &iQref );
            break;
        }
        default:
        {
            /** For MISRA Compliance */
        }
        break;
    }

    /** Park Transformation */
    mcFoc_ParkTransformation( &pState->iAlphaBeta, sine, cosine, &pState->iDQ );

    /** Execute flux control */
    mcFlxI_FluxControlAuto( &pState->bFluxController, iDref, pState->iDQ.d, &pState->uDQ.d );

    /** ToDO: Apply circle limit for Q-axis reference current clamping  */

    /** Execute torque control */
    mcTorI_TorqueControlAuto( &pState->bTorqueController, iQref, pState->iDQ.q, &pState->uDQ.q );

    /** Inverse Park transformation */
    mcFoc_InverseParkTransformation( &pState->uDQ, sine, cosine, &pState->uAlphaBeta );

    /** Space vector modulation */
    mcPwmI_PulseWidthModulation(&pState->bPwmModulator, &pState->uAlphaBeta, mcPwmI_Duty_gau16 );

}


/*! \brief Field Oriented Control ( FOC )
 *
 * Details.
 * Field Oriented Control ( FOC )
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
#ifdef RAM_EXECUTE
void __ramfunc__  mcFocI_FieldOrientedControlSlow( const tmcFocI_ModuleData_s * const pParameters )
#else
void mcFocI_FieldOrientedControlSlow( const tmcFocI_ModuleData_s * const pParameters )
#endif
{

}

/*! \brief Change motor direction 
 *
 * Details.
 * Change motor direction 
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void mcFocI_MotorDirectionChange(const tmcFocI_ModuleData_s * const pModule )
{
     /** Get the linked state variable */
    tmcFoc_State_s * pState;
    pState = (tmcFoc_State_s *)pModule->pStatePointer;
    
    /** */
    pState->commandDirection *= -1;
    
}

/*! \brief Reset Field Oriented Control ( FOC )
 *
 * Details.
 * Reset Field Oriented Control ( FOC )
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return:
 */
void mcFocI_FieldOrientedControlReset( const tmcFocI_ModuleData_s * const pParameters )
{
    /** Reset open loop startup state variables  */
}
