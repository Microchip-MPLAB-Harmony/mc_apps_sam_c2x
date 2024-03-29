/*******************************************************************************
  Divide Square Root Accelerator (DIVAS) PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_divas.c

  Summary:
    DIVAS PLIB Implementation File

  Description:
    This file defines the interface to the DIVAS peripheral library. This
    library provides access to and control of the associated peripheral
    instance.

*******************************************************************************/

/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
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


#include "plib_divas.h"
#include "device.h"

#define DIVAS_CRITICAL_ENTER()                        \
    do {                                               \
        volatile uint32_t globalInterruptState;        \
        globalInterruptState = __get_PRIMASK();        \
        __disable_irq()
#define DIVAS_CRITICAL_LEAVE()                        \
        __set_PRIMASK(globalInterruptState);           \
    }                                                  \
    while (false)


/* Return 32 bit result, the result only. */
static inline uint32_t divas_result32(void)
{
    return DIVAS_REGS->DIVAS_RESULT;
}

/* Return 64 bit result, the result and remainder. */
static inline uint64_t divas_result64(void)
{
    uint64_t res = (uint64_t)DIVAS_REGS->DIVAS_REM;
    res <<= 32U;
    res |= (uint64_t)(DIVAS_REGS->DIVAS_RESULT);
    return res;
}

static inline void divas_div(const uint8_t sign, const uint32_t dividend, const uint32_t divisor)
{
    DIVAS_REGS->DIVAS_CTRLA = (uint8_t)(((uint32_t)DIVAS_REGS->DIVAS_CTRLA & ~DIVAS_CTRLA_SIGNED_Msk) | (uint32_t)sign);
    DIVAS_REGS->DIVAS_DIVIDEND = dividend;
    DIVAS_REGS->DIVAS_DIVISOR  = divisor;
    while((DIVAS_REGS->DIVAS_STATUS & DIVAS_STATUS_BUSY_Msk) == DIVAS_STATUS_BUSY_Msk)
    {
        /* Wait for the square root to complete */
    }
}

uint32_t DIVAS_SquareRoot(uint32_t number)
{
    uint32_t res = 0U;

    DIVAS_CRITICAL_ENTER();
    DIVAS_REGS->DIVAS_SQRNUM = number;
    while((DIVAS_REGS->DIVAS_STATUS & DIVAS_STATUS_BUSY_Msk) == DIVAS_STATUS_BUSY_Msk)
    {
        /* Wait for the square root to complete */
    }
    res = DIVAS_REGS->DIVAS_RESULT;
    DIVAS_CRITICAL_LEAVE();
    return res;
}

/* 32-bit Signed division, return quotient */
int32_t DIVAS_DivSigned(int32_t numerator, int32_t denominator)
{
    int32_t res;
    DIVAS_CRITICAL_ENTER();
    divas_div(1U, (uint32_t)numerator, (uint32_t)denominator);
    res = (int32_t)divas_result32();
    DIVAS_CRITICAL_LEAVE();
    return res;
}

/* 32-bit Unsigned division, return quotient */
uint32_t DIVAS_DivUnsigned(uint32_t numerator, uint32_t denominator)
{
    uint32_t res;
    DIVAS_CRITICAL_ENTER();
    divas_div(0U, numerator, denominator);
    res = divas_result32();
    DIVAS_CRITICAL_LEAVE();
    return res;
}

/* 32-bit Signed division, return quotient and remainder as 64-bit number */
uint64_t DIVAS_DivmodSigned(int32_t numerator, int32_t denominator)
{
    uint64_t res;
    DIVAS_CRITICAL_ENTER();
    divas_div(1U, (uint32_t)numerator, (uint32_t)denominator);
    res = divas_result64();
    DIVAS_CRITICAL_LEAVE();
    return res;
}

/* 32-bit Unsigned division, return quotient and remainder as 64-bit result */
uint64_t DIVAS_DivmodUnsigned(uint32_t numerator, uint32_t denominator)
{
    uint64_t res;
    DIVAS_CRITICAL_ENTER();
    divas_div(0U, numerator, denominator);
    res = divas_result64();
    DIVAS_CRITICAL_LEAVE();
    return res;
}
