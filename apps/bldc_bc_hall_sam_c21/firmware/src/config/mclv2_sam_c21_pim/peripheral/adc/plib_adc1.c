/*******************************************************************************
  Analog-to-Digital Converter(ADC1) PLIB

  Company
    Microchip Technology Inc.

  File Name
    plib_adc1.c

  Summary
    ADC1 PLIB Implementation File.

  Description
    This file defines the interface to the ADC peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

  Remarks:
    None.

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
/* This section lists the other files that are included in this file.
*/

#include "interrupts.h"
#include "plib_adc1.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data
// *****************************************************************************
// *****************************************************************************

#define ADC1_LINEARITY_POS  (6U)
#define ADC1_LINEARITY_Msk   (0x7UL << ADC1_LINEARITY_POS)

#define ADC1_BIASCAL_POS  (9U)
#define ADC1_BIASCAL_Msk   (0x7UL << ADC1_BIASCAL_POS)


// *****************************************************************************
// *****************************************************************************
// Section: ADC1 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Initialize ADC module */
void ADC1_Initialize( void )
{
    /* Reset ADC */
    ADC1_REGS->ADC_CTRLA = (uint8_t)ADC_CTRLA_SWRST_Msk;

    while((ADC1_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_SWRST_Msk) == ADC_SYNCBUSY_SWRST_Msk)
    {
        /* Wait for Synchronization */
    }
    /* Write linearity calibration in BIASREFBUF and bias calibration in BIASCOMP */
    uint32_t calib_low_word = (uint32_t)(*(uint64_t*)OTP5_ADDR);
    ADC1_REGS->ADC_CALIB = (uint16_t)((ADC_CALIB_BIASREFBUF((calib_low_word & ADC1_LINEARITY_Msk) >> ADC1_LINEARITY_POS)) | 
                                      (ADC_CALIB_BIASCOMP((calib_low_word & ADC1_BIASCAL_Msk) >> ADC1_BIASCAL_POS)));

    ADC1_REGS->ADC_CTRLA = (uint8_t)ADC_CTRLA_SLAVEEN_Msk;
    /* Sampling length */
    ADC1_REGS->ADC_SAMPCTRL = (uint8_t)ADC_SAMPCTRL_SAMPLEN(7UL);

    /* Reference */
    ADC1_REGS->ADC_REFCTRL = (uint8_t)ADC_REFCTRL_REFSEL_INTVCC2;

    /* Input pin */
    ADC1_REGS->ADC_INPUTCTRL = (uint16_t) ADC_POSINPUT_AIN0;

    /* Resolution & Operation Mode */
    ADC1_REGS->ADC_CTRLC = (uint16_t)(ADC_CTRLC_RESSEL_12BIT | ADC_CTRLC_WINMODE(0UL) );


    /* Clear all interrupt flags */
    ADC1_REGS->ADC_INTFLAG = (uint8_t)ADC_INTFLAG_Msk;

    while(0U != ADC1_REGS->ADC_SYNCBUSY)
    {
        /* Wait for Synchronization */
    }
}

/* Enable ADC module */
void ADC1_Enable( void )
{
    ADC1_REGS->ADC_CTRLA |= (uint8_t)ADC_CTRLA_ENABLE_Msk;
    while(0U != ADC1_REGS->ADC_SYNCBUSY)
    {
        /* Wait for Synchronization */
    }
}

/* Disable ADC module */
void ADC1_Disable( void )
{
    ADC1_REGS->ADC_CTRLA &= (uint8_t)(~ADC_CTRLA_ENABLE_Msk);
    while(0U != ADC1_REGS->ADC_SYNCBUSY)
    {
        /* Wait for Synchronization */
    }
}

/* Configure channel input */
void ADC1_ChannelSelect( ADC_POSINPUT positiveInput, ADC_NEGINPUT negativeInput )
{
    /* Configure pin scan mode and positive and negative input pins */
    ADC1_REGS->ADC_INPUTCTRL = (uint16_t) positiveInput | (uint16_t) negativeInput;

    while((ADC1_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_INPUTCTRL_Msk) == ADC_SYNCBUSY_INPUTCTRL_Msk)
    {
        /* Wait for Synchronization */
    }
}

/* Start the ADC conversion by SW */
void ADC1_ConversionStart( void )
{
    /* Start conversion */
    ADC1_REGS->ADC_SWTRIG |= (uint8_t)ADC_SWTRIG_START_Msk;

    while((ADC1_REGS->ADC_SYNCBUSY & ADC_SYNCBUSY_SWTRIG_Msk) == ADC_SYNCBUSY_SWTRIG_Msk)
    {
        /* Wait for Synchronization */
    }
}

/* Check whether auto sequence conversion is done */
bool ADC1_ConversionSequenceIsFinished(void)
{
    bool seq_status = false;
    if ((ADC1_REGS->ADC_SEQSTATUS & ADC_SEQSTATUS_SEQBUSY_Msk) != ADC_SEQSTATUS_SEQBUSY_Msk)
    {
        seq_status = true;
    }
    return seq_status;
}

/* Configure window comparison threshold values */
void ADC1_ComparisonWindowSet(uint16_t low_threshold, uint16_t high_threshold)
{
    ADC1_REGS->ADC_WINLT = low_threshold;
    ADC1_REGS->ADC_WINUT = high_threshold;
    while(0U != (ADC1_REGS->ADC_SYNCBUSY))
    {
        /* Wait for Synchronization */
    }
}

void ADC1_WindowModeSet(ADC_WINMODE mode)
{
	ADC1_REGS->ADC_CTRLC =  (ADC1_REGS->ADC_CTRLC & (uint16_t)(~ADC_CTRLC_WINMODE_Msk)) | (uint16_t)((uint32_t)mode << ADC_CTRLC_WINMODE_Pos);
    while(0U != (ADC1_REGS->ADC_SYNCBUSY))
    {
        /* Wait for Synchronization */
    }
}

/* Read the conversion result */
uint16_t ADC1_ConversionResultGet( void )
{
    return (uint16_t)ADC1_REGS->ADC_RESULT;
}

void ADC1_InterruptsClear(ADC_STATUS interruptMask)
{
    ADC1_REGS->ADC_INTFLAG = (uint8_t)interruptMask;
}

void ADC1_InterruptsEnable(ADC_STATUS interruptMask)
{
    ADC1_REGS->ADC_INTENSET = (uint8_t)interruptMask;
}

void ADC1_InterruptsDisable(ADC_STATUS interruptMask)
{
    ADC1_REGS->ADC_INTENCLR = (uint8_t)interruptMask;
}

/* Check whether result is ready */
bool ADC1_ConversionStatusGet( void )
{
    bool status;
    status =  (((ADC1_REGS->ADC_INTFLAG & ADC_INTFLAG_RESRDY_Msk) >> ADC_INTFLAG_RESRDY_Pos) != 0U);
    if (status == true)
    {
        ADC1_REGS->ADC_INTFLAG = (uint8_t)ADC_INTFLAG_RESRDY_Msk;
    }
    return status;
}
