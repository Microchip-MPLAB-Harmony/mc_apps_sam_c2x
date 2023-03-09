/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/
// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
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
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "userparams.h"
#include "mc_app.h"
#include "X2CScope.h"
#include "X2CScopeCommunication.h"
#include <sys/attribs.h>
#include "q14_flying_start_mcLib.h"
#include "q14_rolo_mcLib.h"  

#ifndef CTRL_PWM_1_1
static uint8_t  adc_interrupt_counter = 0U;
#endif
static uint16_t adc_result_data[2];
uint8_t start_toggle = 0;
uint16_t adc_dc_bus_voltage;
uint16_t pot_input;
uint8_t  direction = 0;
static uint8_t direction_change_flag = 0;
uint8_t phaseindex[3] = {0,1,2};


static uint16_t calibration_sample_count = 0x0000U;
static uint16_t adc_0_offset = 0u;
static uint16_t adc_1_offset = 0u;
static uint8_t  overCurrentFaultActive = 0u;
/*initializing the fault delay counter to final value to avoid any overcurrent 
reset delay in case an OC fault is triggered upon board power up*/
static uint32_t    overCurrentFaultResetDelayCounter = OVERCURRENT_RESET_DELAY_COUNT;

static uint32_t adc_0_sum = 0u;
static uint32_t adc_1_sum = 0u;
static uintptr_t dummyforMisra;
static button_response_t button_S2_data;
static button_response_t button_S3_data;
void ADC_ISR(ADC_STATUS status,uintptr_t context);
void ADC_CALIB_ISR (ADC_STATUS status, uintptr_t context);
void OC_FAULT_ISR(uintptr_t context);
void motor_start_stop(void);
void motor_direction_toggle(void);
void buttonRespond(button_response_t * buttonResData, void (* buttonJob)(void));

// *****************************************************************************
/* MISRA C-2012 Rule 14.3, and 2.1 deviated below. Deviation record ID -  
    H3_MISRAC_2012_R_14_3_DR_1, H3_MISRAC_2012_R_2_1_DR_1 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma coverity compliance block \
(deviate:1 "MISRA C-2012 Rule 14.3" "H3_MISRAC_2012_R_14_3_DR_1" )\
(deviate:1 "MISRA C-2012 Rule 2.1" "H3_MISRAC_2012_R_2_1_DR_1" )
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{

/* Initialize all modules */
    SYS_Initialize ( NULL );
    ADC0_CallbackRegister((ADC_CALLBACK) ADC_CALIB_ISR, (uintptr_t)dummyforMisra);
    motorcontrol_vars_init();
    ADC0_Enable();
    state_run = 0;
    X2CScope_Init();
    MCCTRL_InitializeFlyingStartControl();
    TCC0_PWMStart();
    motor_stop();
    EIC_CallbackRegister ((EIC_PIN)EIC_PIN_2, (EIC_CALLBACK) OC_FAULT_ISR,(uintptr_t)dummyforMisra);

    while ( true )
    {
        
        
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        X2CScope_Communicate();
        if(0U == syn10ms())
        {
            if(overCurrentFaultActive == 0u)
            {
                /* This if loop ensures that when the motor direction is changed, 
                 * the PWM is disabled for 10mS before re-starting the state machine
                 * state to avoid current spike*/
                if(direction_change_flag == 1u) 
                {
                    if(start_toggle==1u)
                    {
                        motor_start();
                   }
                    direction_change_flag = 0;
                }
                speed_ramp(); 

                button_S2_data.inputVal = BTN_START_STOP_Get();
                buttonRespond(&button_S2_data, &motor_start_stop);
                button_S3_data.inputVal = BTN_DIR_TGL_Get();
                buttonRespond(&button_S3_data, &motor_direction_toggle);
            }
            else
            {
                overCurrentFaultResetDelayCounter++;
                //Clear the Over Current Flag after a delay defined by OVERCURRENT_RESET_DELAY_SEC
                if(overCurrentFaultResetDelayCounter >= OVERCURRENT_RESET_DELAY_COUNT)
                {
                    overCurrentFaultResetDelayCounter = 0u;
                    overCurrentFaultActive = 0u;
                    
                }
            }
        }

        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}

#pragma coverity compliance end_block "MISRA C-2012 Rule 14.3"
#pragma coverity compliance end_block "MISRA C-2012 Rule 2.1"
#pragma GCC diagnostic pop
/* MISRAC 2012 deviation block end */
void OC_FAULT_ISR(uintptr_t context)
{
    motor_stop(); // Disable TCC output
    start_toggle=0u; // Stop the state machine
    overCurrentFaultActive = 1u; // Set overCurrentFault Flag
    TCC0_REGS->TCC_CTRLBSET = TCC_CTRLBSET_CMD(TCC_CTRLBSET_CMD_RETRIGGER_Val); // Clear the COUNT value
    syn_cnt = SYN_VAL10MS; // Reset the 10mS counter
    TCC0_REGS->TCC_STATUS = TCC_STATUS_FAULT0(1); // Clear Non Recoverable Fault
    LED1_OC_FAULT_Set();
       
}
/* This ISR calibrates zero crossing point for Phase U and Phase V currents*/
void ADC_CALIB_ISR (ADC_STATUS status, uintptr_t context)
{
    
    X2CScope_Update();
    calibration_sample_count++;
    if(calibration_sample_count <= 4096u)
    {
        adc_0_sum += ADC0_ConversionResultGet();    
        adc_1_sum += ADC1_ConversionResultGet();
    }
    else
    {
        adc_0_offset = (uint16_t)(adc_0_sum>>12u);
        adc_1_offset = (uint16_t)(adc_1_sum>>12u);
        ADC0_Disable();
        ADC0_CallbackRegister((ADC_CALLBACK) ADC_ISR, (uintptr_t)dummyforMisra);
        ADC0_Enable();

    }
    
    ADC0_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;
    
    
}

#ifdef CTRL_PWM_1_1
void ADC_ISR(ADC_STATUS status, uintptr_t context)
{
         adc_result_data[0] = ADC0_ConversionResultGet();
         adc_result_data[1] = ADC1_ConversionResultGet();
  
         /* Clear all interrupt flags */
         ADC0_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;

         ADC0_REGS->ADC_INTENCLR = ADC_INTFLAG_RESRDY_Msk;// Disable ADC interrupt
	
    	/* select the next channel */
	/* select the next ADC channel for conversion */
         ADC0_ChannelSelect(ADC_POSINPUT_AIN9,ADC_NEGINPUT_GND); // DC Bus Voltage to ADC0
         ADC1_ChannelSelect(ADC_POSINPUT_AIN0,ADC_NEGINPUT_GND); // Potentiometer to ADC1
		
         ADC0_REGS->ADC_SWTRIG |= ADC_SWTRIG_START_Msk; 
              
         /* store the first ADC result value */
	cur_mea[0] = ((int16_t)adc_result_data[0] - (int16_t)adc_0_offset);               
							
         /* store the first ADC result value */
         cur_mea[1] =  ((int16_t)adc_result_data[1] - (int16_t)adc_1_offset);   
         
         current_measurement_management();
		 
	/* motor control */
	motorcontrol();
		 
	while(ADC0_REGS->ADC_INTFLAG != ADC_INTFLAG_RESRDY_Msk){
        /* Wait state*/
    }
                       
         /* Read the ADC result value */
    adc_dc_bus_voltage =  ADC0_ConversionResultGet();
	pot_input = ADC1_ConversionResultGet();
  
         /* select the next ADC channel for conversion */
         ADC0_ChannelSelect(ADC_POSINPUT_AIN2,ADC_NEGINPUT_GND); // Phase U to ADC0
         ADC1_ChannelSelect(ADC_POSINPUT_AIN5,ADC_NEGINPUT_GND); // Phase V to ADC1
         ADC0_REGS->ADC_INTENSET = ADC_INTFLAG_RESRDY_Msk;// Enable ADC interrupt
         /* Clear all interrupt flags */
         ADC0_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;	
         
         X2CScope_Update();
         
         return;
}
#else
void ADC_ISR(ADC_STATUS status, uintptr_t context)
{
         adc_result_data[0] = ADC0_ConversionResultGet();
         adc_result_data[1] = ADC1_ConversionResultGet();
         
         /* Clear all interrupt flags */
         ADC0_REGS->ADC_INTFLAG = ADC_INTFLAG_Msk;
	if (0U == adc_interrupt_counter)
	{
		/* store the first ADC result value */
	         cur_mea[0] = ((int16_t)adc_result_data[0] - (int16_t)adc_0_offset);               
							
                  /* store the first ADC result value */
                  cur_mea[1] =  ((int16_t)adc_result_data[1] - (int16_t)adc_1_offset); 
         
                  current_measurement_management();
         
                  /* motor control */
		motorcontrol();
                  adc_interrupt_counter = 1;
	         /* select the next channel */
		/* select the next ADC channel for conversion */
                  ADC0_ChannelSelect(ADC_POSINPUT_AIN9,ADC_NEGINPUT_GND); // DC Bus Voltage to ADC0
                  ADC1_ChannelSelect(ADC_POSINPUT_AIN0,ADC_NEGINPUT_GND); // Potentiometer to ADC1
	}
	else if (1U == adc_interrupt_counter)
	{
                      
                  adc_interrupt_counter = 0;	    
         
                  /* Read the ADC result value */
                  adc_dc_bus_voltage =  adc_result_data[0];
		pot_input = adc_result_data[1];
  
                  /* select the next ADC channel for conversion */
                  ADC0_ChannelSelect(ADC_POSINPUT_AIN2,ADC_NEGINPUT_GND); // Phase U to ADC0
                  ADC1_ChannelSelect(ADC_POSINPUT_AIN5,ADC_NEGINPUT_GND); // Phase V to ADC1

	}
	else
	{
		
      /* undefined condition*/

	}
         X2CScope_Update();
         return;
}

#endif

void motor_start_stop(void) //Calling this function, starts/stops the motor
{

     start_toggle =(uint8_t)(!(bool)start_toggle);
     
     if(start_toggle !=1U)
     {
        motor_stop();
        state_count = 1u;
        #ifdef FLYING_START_ENABLE 
        state_flying_start = 0;
        #endif

        state_stopped = 0u;
        state_align = 0u;
        state_start = 0u;
        state_closingloop = 0u;
        state_closedloop = 0u;
        angle_rollover_count = 0u;

#ifdef FLYING_START_ENABLE
        motor_status = FLYING_START;
#else
        motor_status = STOPPED;
#endif 
        elespeed = 0;
        flx_arg = 0;
        bemf_arg = 0;
        flx_arg_mem = 0;
        bemf_arg_mem = 0;

        if(direction ==0u)
        {
            spe_ref_sgn = 1;
        }
        else
        {
            spe_ref_sgn = -1;
        }
     }
     else         
     {
        LED1_OC_FAULT_Clear(); // Clear Fault LED when the motor is started.
     }
}

void motor_direction_toggle(void) //Calling this function, toggles the direction of the motor
{
    if(start_toggle !=1u)
    {
        motor_stop(); 
        direction = (uint8_t)(!(bool)direction); // toggle direction 
        LED2_DIRECTION_Toggle();
        state_count = 1u;
        #ifdef FLYING_START_ENABLE 
        state_flying_start = 0;
        #endif
        state_stopped = 0u;
        state_align = 0u;
        state_start = 0u;
        state_closingloop = 0u;
        state_closedloop = 0u;
        angle_rollover_count = 0u;

#ifdef FLYING_START_ENABLE
        motor_status = FLYING_START;
#else
        motor_status = STOPPED; 
#endif 
        direction_change_flag = 1u;
        trigger = 0;
        if(direction == 0u)
        {
            spe_ref_sgn = 1;
        }
        else
        {
            spe_ref_sgn = -1;
        }
    }
    
}

void buttonRespond(button_response_t * buttonResData, void (* buttonJob)(void))
{
    switch(buttonResData->state){
        case 0u:  /* Detect if button is pressed. */
            if(buttonResData->inputVal == 0u){
                buttonJob();
                buttonResData->cnt = 0u;
                buttonResData->state = 1u;
            }
            break;
        case 1u:  /* Stay idle for 500ms, and then return to detect. */
            buttonResData->cnt++;
            if(buttonResData->cnt >= RL_500MS_10MS){
                buttonResData->cnt = 0u;
                buttonResData->state = 0u;
            }
            break;
        default:
            /* Undefined state: Should never come here */
            break;
    }
}
/*******************************************************************************
 End of File
*/

