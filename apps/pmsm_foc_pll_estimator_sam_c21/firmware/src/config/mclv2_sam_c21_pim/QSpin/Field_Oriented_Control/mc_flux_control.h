/*******************************************************************************
  System Definitions

  File Name:
    mc_flux_control.h

  Summary:
    Header file which contains variables and function prototypes for flux control

  Description:
    This file contains variables and function prototypes which are generally used for flux
    control

 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef MCFLX_H
#define MCFLX_H

/*******************************************************************************
 * Header inclusions
*******************************************************************************/
#include "mc_types.h"
#include "mc_pi_control.h"
#include "mc_userparams.h"
#include "mc_motor.h"

/*******************************************************************************
 Default Module configuration parameters
*******************************************************************************/

/*******************************************************************************
Type Definition
*******************************************************************************/
typedef struct
{
    float32_t Kp;
    float32_t Ki;
    float32_t dt;
    tmcMot_PMSM_s * pMotorParameters;
    void * pStatePointer;
}tmcFlx_Parameters_s;

/*******************************************************************************
 * Interface variables
*******************************************************************************/
//#define RAM_EXECUTE

/*******************************************************************************
 Static Interface Functions
*******************************************************************************/
/*! \brief Set module parameters
 *
 * Details.
 * Set module parameters
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
__STATIC_INLINE void mcFlxI_ParametersSet( tmcFlx_Parameters_s * const pParameters )
{
    pParameters->Kp = (float32_t)0.014955;
    pParameters->Ki = (float32_t)13.319;
    pParameters->dt = (float32_t)0.000063;

    pParameters->pMotorParameters = &mcMotI_PMSM_gds;

}

/*******************************************************************************
 Interface Functions
*******************************************************************************/
/*! \brief Initialize flux control module
 *
 * Details.
 * Initialize flux control module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_FluxControlInit( tmcFlx_Parameters_s * const pParameters );

/*! \brief Enable flux control module
 *
 * Details.
 * Enable flux control module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_FluxControlEnable( tmcFlx_Parameters_s * const pParameters );

/*! \brief Disable flux control module
 *
 * Details.
 * Disable flux control module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_FluxControlDisable( tmcFlx_Parameters_s * const pParameters );

/*! \brief Flux control
 *
 * Details.
 * Flux control
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
#ifdef RAM_EXECUTE
void __ramfunc__  mcFlxI_FluxControlManual( const tmcFlx_Parameters_s * const pParameters,
                                                                   const int16_t Out   );
#else
void mcFlxI_FluxControlManual(  const tmcFlx_Parameters_s * const pParameters,
                                                         const int16_t  Out );
#endif

/*! \brief Flux control
 *
 * Details.
 * Flux control
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
#ifdef RAM_EXECUTE
void __ramfunc__  mcFlxI_FluxControlAuto( const tmcFlx_Parameters_s * const pParameters,
                                          const int16_t iDref, const int16_t iDact, int16_t * const pOut   );
#else
void mcFlxI_FluxControlAuto(  const tmcFlx_Parameters_s * const pParameters,
                              const int16_t iDref, const  int16_t iDact, int16_t * const pOut );
#endif

/*! \brief Reset Flux control
 *
 * Details.
 * Reset Flux control
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return:
 */
void mcFlxI_FluxControlReset( const tmcFlx_Parameters_s * const pParameters );



/*! \brief Enable MTPA module
 *
 * Details.
 * Enable MTPA module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_MTPAEnable( tmcFlx_Parameters_s * const pParameters );

/*! \brief Disable MTPA module
 *
 * Details.
 * Disable MTPA module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_MTPADisable( tmcFlx_Parameters_s * const pParameters );

/*! \brief Disable MTPA module
 *
 * Details.
 * Disable MTPA module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_MTPAInit( tmcFlx_Parameters_s * const pParameters );

/*! \brief Disable MTPA module
 *
 * Details.
 * Disable MTPA module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
#ifdef RAM_EXECUTE
void __ramfunc__  mcFlxI_MTPA( tmcFlx_Parameters_s * const pParameters,
                   const tmcTypes_DQ_s * const pIdq, int16_t * const pIdref );
#else
void mcFlxI_MTPA( tmcFlx_Parameters_s * const pParameters,
                   const tmcTypes_DQ_s * const pIdq, int16_t * const pIdref );
#endif

/*! \brief Disable MTPA module
 *
 * Details.
 * Disable MTPA module
 *
 * @param[in]: None
 * @param[in/out]: None
 * @param[out]: None
 * @return: None
 */
void  mcFlxI_MTPAReset( tmcFlx_Parameters_s * const pParameters );


#endif // MCFLX_H

