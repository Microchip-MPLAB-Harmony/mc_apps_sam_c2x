/**
 * @brief 
 *    Motor Control current measurement implementation file
 *
 * @Company 
 *    Microchip Technology Inc.
 *
 * @File name
 *    mc_current_calculation.c
 *
 * @Summary 
 *    Implementation file for current measurement
 *
 * @Description 
 *    This file contains the implementation of the data structures and functions  necessary for 
 *    the current measurement functionality in motor control applications.
 *
 *    It includes the initialization of the current measurement module,  calculation of current 
 *    sensor offsets, computation of phase currents, and resetting of current calculations.
 */

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2021 Microchip Technology Inc. and its subsidiaries.
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
 Header files inclusions
 *******************************************************************************/
#include "mc_current_calculation.h"

/*******************************************************************************
 Module configuration
 *******************************************************************************/
#define USE_DUAL_SHUNT

/*******************************************************************************
 Private constants
 *******************************************************************************/
#define OFFSET_SAMPLES    128u

/*******************************************************************************
 * Private data-types
 *******************************************************************************/

/**
 * @brief Structure to hold the current measurement states.
 */
typedef struct
{
    uint8_t  calibDone;         /**< Calibration done flag */
    uint16_t adcSampleCounter;  /**< ADC sample counter */
    int16_t  iaOffset;          /**< Offset for phase A current */
    int16_t  ibOffset;          /**< Offset for phase B current */
    int16_t  minOffset;         /**< Minimum offset value */
    int16_t  maxOffset;         /**< Maximum offset value */
    int16_t  convFactorValue;    /**<  Coversion factor value */
    uint16_t  convFactorShift;   /**<  Coversion factor shift */
    int32_t  iaOffsetBuffer;    /**< Buffer for phase A current offset calculation */
    int32_t  ibOffsetBuffer;    /**< Buffer for phase B current offset calculation */
} tmcCur_States_s;

/*******************************************************************************
 * Private variables
 *******************************************************************************/

/**
 * @brief Instance of the current measurement states.
 */
static tmcCur_States_s mcCur_State_mds;

/*******************************************************************************
 * Private Functions
 *******************************************************************************/

/*******************************************************************************
 * Interface variables
 *******************************************************************************/

/**
 * @brief Global instance of the module data structure for current measurement.
 */
tmcCur_ModuleData_s mcCurI_ModuleData_gds;

/*******************************************************************************
 * Interface Functions
 *******************************************************************************/

/**
 * @brief Current control initialization function.
 *
 * Initializes the current control module by setting up the required parameters 
 * and preparing the module for current calculations.
 *
 * @param[in,out] pModule Pointer to the module data structure.
 * 
 * @return None
 */
void mcCurI_CurrentCalculationInit(tmcCur_ModuleData_s * const pModule)
{

    /* Set parameters */
    mcCur_ParametersSet(&pModule->dParameters);

    /* Set parameters */
    mcCur_ParametersSet( &pModule->dParameters);

    float32_t f32a = 16.0f * pModule->dParameters.maxBoardCurrent / pModule->dParameters.baseCurrent;
    mcUtils_FloatToValueShiftPair( f32a, &mcCur_State_mds.convFactorValue, &mcCur_State_mds.convFactorShift );
}

/**
 * @brief Function to calculate the current sensor offset.
 *
 * Calculates the offset for the current sensors to ensure accurate current measurements.
 *
 * @param[in,out] pModule Pointer to the module data structure.
 * 
 * @return None
 */
tmcTypes_StdReturn_e mcCurI_CurrentOffsetCalculation(tmcCur_ModuleData_s * const pModule)
{
    tmcTypes_StdReturn_e eStatus = StdReturn_Progress;
    tmcCur_States_s * pState;
    tmcCur_Input_s * pInput;
    tmcCur_Output_s * pOutput;

    pInput = &pModule->dInput;
    pOutput = &pModule->dOutput;
    pState = &mcCur_State_mds;

    mcCur_InputPortsRead( pInput );

    if ( pState->adcSampleCounter < OFFSET_SAMPLES)
    {
        pState->iaOffsetBuffer += pInput->iaAdcInput;
        pState->ibOffsetBuffer += pInput->ibAdcInput;
        pState->adcSampleCounter++;
    }
    else
    {

        pState->iaOffset = (int16_t)( pState->iaOffsetBuffer/ (int16_t)OFFSET_SAMPLES );
        pState->ibOffset = (int16_t)( pState->ibOffsetBuffer/ (int16_t)OFFSET_SAMPLES );


        /** Set the return status as success */
        eStatus = StdReturn_Complete;

        /**Set ADC Calibration Done Flag */
        pOutput->calibDone = 1u;

    }

    mcCur_OutputPortsWrite(&pModule->dOutput);

    return eStatus;
}

/**
 * @brief Function to calculate the phase currents.
 *
 * Computes the phase currents based on the ADC input values and stores the results 
 * in the module's output structure.
 *
 * @param[in,out] pModule Pointer to the module data structure.
 * 
 * @return None
 */
#ifdef RAM_EXECUTE
void __ramfunc__ mcCurI_CurrentCalculation(tmcCur_ModuleData_s * const pModule)
#else
void mcCurI_CurrentCalculation(tmcCur_ModuleData_s * const pModule)
#endif
{
    tmcCur_States_s * pState;
    tmcCur_Input_s * pInput;
    tmcCur_Output_s * pOutput;

    pInput = &pModule->dInput;
    pOutput = &pModule->dOutput;
    pState = &mcCur_State_mds;

    mcCur_InputPortsRead( &pModule->dInput );

    /** Phase A current measurement */
    int16_t   temp  = pState->iaOffset - pInput->iaAdcInput;
    pOutput->iABC.a = (int16_t)Q_RIGHT_SHIFT(((int32_t)temp * (int32_t)pState->convFactorValue ), pState->convFactorShift );

    /** Phase B current measurement */
    temp  = pState->ibOffset - pInput->ibAdcInput;
    pOutput->iABC.b = (int16_t)Q_RIGHT_SHIFT(((int32_t)temp * (int32_t)pState->convFactorValue ), pState->convFactorShift );

    mcCur_OutputPortsWrite( &pModule->dOutput );
}

/**
 * @brief Function to reset phase current calculation.
 *
 * Resets the state of the current calculation module, clearing any stored values 
 * and preparing it for a fresh set of calculations.
 *
 * @param[in,out] pModule Pointer to the module data structure.
 * 
 * @return None
 */
void mcCurI_CurrentCalculationReset(tmcCur_ModuleData_s * const pModule)
{
    tmcCur_Output_s * pOutput;
    pOutput = &pModule->dOutput;

    /** Reset state variables */

    /** Reset output ports */
    pOutput->iABC.a = 0;
    pOutput->iABC.b = 0;
    pOutput->iABC.c = 0;

    /** Update output ports */
    mcCur_OutputPortsWrite( &pModule->dOutput );
}

