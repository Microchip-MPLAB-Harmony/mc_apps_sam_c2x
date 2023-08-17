/*******************************************************************************
  System Definitions

  File Name:
    mc_generic_library.h

  Summary:
    Header file which contains variables and function prototypes of  generic library functions.

  Description:
    This file contains variables and function prototypes of generic library functions
    which are generally used in Motor Control. Implemented in Q2.14 Fixed Point Arithmetic.
 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef MC_COMMON_FCTS
#define MC_COMMON_FCTS


/******************************************************************************
 * Headers File Inclusion
******************************************************************************/

/******************************************************************************
 * Constants
******************************************************************************/

/******************************************************************************
 * User-defined data structure
******************************************************************************/

/******************************************************************************
 * Interface variables
******************************************************************************/

/******************************************************************************
 * Interface functions
******************************************************************************/
/*! \brief Calculate and return the absolute value of an int8.
 *
 *  Details:
 *  This function computes the absolute value of the input int8 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input int8 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE int8_t UTIL_GetAbsValS8( const int8_t x)
{
     int8_t result;
     
    return result;
    
}

/*! \brief Calculate and return the absolute value of an int16.
 *
 *  Details:
 *  This function computes the absolute value of the input int16 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input int16 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE int16_t UTIL_GetAbsValS16( const int16_t x)
{
    int16_t result;


    return result;
    
}

/*! \brief Calculate and return the absolute value of an int32.
 *
 *  Details:
 *  This function computes the absolute value of the input int32 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input int32 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE int32_t UTIL_GetAbsValS32(const int32_t x)
{
    
}

/*! \brief Calculate and return the absolute value of an int64.
 *
 *  Details:
 *  This function computes the absolute value of the input int64 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input int64 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE int64_t UTIL_GetAbsValS64(const int64_t x)
{
    
}

/*! \brief Calculate and return the absolute value of a float32.
 *
 *  Details:
 *  This function computes the absolute value of the input float32 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input float32 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE float32_t UTIL_GetAbsValF32(const float32_t x)
{
    
}

/*! \brief Calculate and return the absolute value of a float64.
 *
 *  Details:
 *  This function computes the absolute value of the input float64 value 'x' and
 *  returns the result.
 *
 *  @param[in] x The input float64 value
 *
 *  @return The absolute value of 'x'
 */
__STATIC_INLINE float64_t UTIL_GetAbsValF64(const float64_t x)
{
    
}

#endif // MC_COMMON_FCTS
