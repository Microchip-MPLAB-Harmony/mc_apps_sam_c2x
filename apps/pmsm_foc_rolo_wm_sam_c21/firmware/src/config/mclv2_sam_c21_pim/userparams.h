/*******************************************************************************
  System Definitions

  File Name:
    userparams.h

  Summary:
    Header file which defines Motor Specific and Board Specific constants 

  Description:
    This file contains the motor and board specific constants. It also defines
 * switches which allows algorithm to be run in debug modes like Open Loop Mode,
 * Torque mode, etc. 

 *******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef USERPARAMS_H 
#define USERPARAMS_H

/*Define the Motor Type*/
#define SMALL_HURST    

/*Define the Board Type*/
#define MCLV2

/*Defining CTRL_PWM_1_1 sets the Control to PWM Frequency Ratio as 1:1
 Undefining CTRL_PWM_1_1 sets the Control to PWM Frequency Ration as 1:2
 Note: Please account for CPU bandwidth availability while setting 
 Control to PWM Frequency Ratio as 1:1. If the ISR execution is not completed within
 the same PWM cycle, it could lead to erratic motor behavior */
#undef CTRL_PWM_1_1


/* Defining Q_AXIS_STARTUP causes the open loop startup by injecting current in Q axis 
 * Undefining Q_AXIS_STARTUP cause the open loop startup by injecting current in D axis */
#undef Q_AXIS_STARTUP

/*Debug Feature: Defining OPEN_LOOP_FUNCTIONING forces the algorithm to operate in Rotor Angle Open Loop mode i.e. angle reference is generated and not estimated*/
#undef OPEN_LOOP_FUNCTIONING

/*Debug Feature: Defining TORQUE_MODE forces the algorithm to operate in Torque Mode i.e. no speed control, the potentiometer input is used as torque reference*/
#undef TORQUE_MODE

/* Defining RAM_EXECUTE executes key motor control functions from RAM and thereby allowing faster execution at the expense of data memory.
 Please note, instruction breakpoint will not be asserted if that particular instruction is being executed from RAM
*Undefining RAM_EXECUTE executes key motor control functions from Flash and thereby reducing data memory consumption at the expense of time */
#undef RAM_EXECUTE

/*Defining USE_DIVAS uses the DIVAS peripheral for division and square root operatons*/
#define USE_DIVAS



/*******************************************************************************
Macro definitions
*******************************************************************************/

#define PWM_HPER_TICKS  ( 2400U )  /* 4000 ticks, total period 8000 ticks @48MHz -> 167us */
/* 2400 ticks , total period 4800 ticks -> 100 micro seconds */

#define HALF_HPER_TICKS  (PWM_HPER_TICKS>>1)

/* motor and application related parameters */
/* Note: only one motor has to be selected! */


#ifdef  SMALL_HURST                      /* Small Hurst motor */
#define MAX_FRE_HZ      (   250 )       /* maximum frequency [Hz] - 3000RPM */
#define MIN_FRE_HZ      (    30 )       /* minimum frequency [Hz] - 360RPM  */
#define POLAR_COUPLES   (     5 )       /* number of polar couples */
#define R_STA           (   2.1 )       /* stator phase resistance [Ohm] */
#define L_SYN           (     0.00192 ) /* synchronous inductance 0.00192 [Hen] (note: >0!) */
#define MAX_CUR_AMP     (     2.0 )     /* peak maximum current [A] */
#define START_CUR_AMP   (     0.4 )     /* peak startup current [A] */
#define KP_V_A          (     5.0 )     /* current loop proportional gain [Volt/Amp] */
#define KI_V_AS         (   1467.0 )    /* current loop integral gain [Volt/(Amp*sec)] */
#define KP_AS_R         (  0.0016 )  	/* speed loop proportional gain [Amp/(rad/sec)] */
#define KI_A_R          (  0.002  )    	/* speed loop integral gain [Amp/((rad/sec)*sec)] */
#define ACC_RPM_S       (  1000.0 )     /* acceleration ramp [rpm/s] */
#define DEC_RPM_S       (   500.0 )     /* acceleration ramp [rpm/s] */
#define STUP_ACCTIME_S  (     2 )     /* startup acceleration time [sec] */
#define CUR_RISE_T      (     1 )     /* current rising time [s] during startup alignment */
#define CUR_FALL_T      (     1 )     /* direct current falling time [s] after startup */
#define MIN_WM_FRE_HZ   (    30 )  // 360 RPM /* Minimum Windmilling Frequency [Hz]*/
#define WINDMILL_CUR_AMP (     0.4 )   /* Post Windmill startup current [A] */
#define RGN_BRK_CUR_AMP (     0.4 )            /*Regenerative Braking current*/
#define WINDMILLING_TIME_SEC  (0.3) /* Windmilling Time in Seconds*/
#define PASSIVE_BRAKING_TIME_SEC       (2) /*Motor Braking Time in Seconds*/
#define TORQUE_MODE_MIN_CUR_AMP 0.08     /* Minimum Torque Mode Reference in A */
#endif  /* ifdef SMALL_HURST */



#ifdef  LONG_HURST                      /* Long Hurst motor */
#define MAX_FRE_HZ      (   250 )       /* maximum frequency [Hz] - 3000RPM */
#define MIN_FRE_HZ      (    30 )       /* minimum frequency [Hz] - 360RPM  */
#define POLAR_COUPLES   (     5 )       /* number of polar couples */
#define R_STA           (     0.285 )   /* stator phase resistance [Ohm] */
#define L_SYN           (     0.00032 ) /* synchronous inductance 0.00192 [Hen] (note: >0!) */
#define MAX_CUR_AMP     (     2.0 )     /* peak maximum current [A] */
#define START_CUR_AMP   (     0.8 )     /* peak startup current [A] */
#define KP_V_A          (     0.71 )     /* current loop proportional gain [Volt/Amp] 0.36*/
#define KI_V_AS         (   917.0 )      /* current loop integral gain [Volt/(Amp*sec)]550 */
#define KP_AS_R         (     0.003 )   /* speed loop proportional gain [Amp/(rad/sec)] */  
#define KI_A_R          (     0.002 )    /* speed loop integral gain [Amp/((rad/sec)*sec)] */
#define ACC_RPM_S       (  1000.0 )     /* acceleration ramp [rpm/s] */
#define DEC_RPM_S       (   500.0 )     /* acceleration ramp [rpm/s] */
#define STUP_ACCTIME_S  (     2.0 )     /* startup acceleration time [sec] */
#define CUR_RISE_T      (     1.5 )       /* current rising time [s] during startup alignment */
#define CUR_FALL_T      (     1 )       /* direct current falling time [s] after startup */
#define MIN_WM_FRE_HZ   (    30 )       /* Minimum Windmilling Frequency [Hz]*/
#define WINDMILL_CUR_AMP (     0.4 )    /* Post Windmill startup current [A] */
#define RGN_BRK_CUR_AMP (     0.4 )     /*Regenerative Braking current*/
#define WINDMILLING_TIME_SEC  (0.3)     /* Windmilling Time in Seconds*/
#define PASSIVE_BRAKING_TIME_SEC       (2) /*Motor Braking Time in Seconds*/
#define TORQUE_MODE_MIN_CUR_AMP 0.2     /* Minimum Torque Mode Reference in A */
#endif  /* ifdef LONG_HURST */

#ifdef  LEADSHINE_EL5_M0400_1_24                      /* LEAD SHINE (EL5-M0400-1-24) */
#define MAX_FRE_HZ      (   250 )      /* maximum frequency [Hz] - 3000 RPM*/
#define MIN_FRE_HZ      (    30 )      /* minimum frequency [Hz] - 360 RPM*/
#define POLAR_COUPLES   (     5 )      /* number of polar couples */
#define R_STA           (     1.375 )  /* stator phase resistance [Ohm] */
#define L_SYN           (     0.00253 )/* synchronous inductance 0.00192 [Hen] (note: >0!) */
#define MAX_CUR_AMP     (     3.0 )    /* peak maximum current [A] */
#define START_CUR_AMP   (     0.8 )    /* peak startup current [A] */
#define KP_V_A          (     6.1 )    /* current loop proportional gain [Volt/Amp] */
#define KI_V_AS         (     469.0 )  /* current loop integral gain [Volt/(Amp*sec)] */
#define KP_AS_R         (     0.001 )  /* speed loop proportional gain [Amp/(rad/sec)] */  // 0.0016
#define KI_A_R          (     0.03 )    /* speed loop integral gain [Amp/((rad/sec)*sec)] */
#define ACC_RPM_S       (  1000.0 )     /* acceleration ramp [rpm/s] */
#define DEC_RPM_S       (   500.0 )     /* acceleration ramp [rpm/s] */
#define STUP_ACCTIME_S  (     2.0 )     /* startup acceleration time [sec] */
#define CUR_RISE_T      (     1.5 )    /* current rising time [s] during startup alignment */
#define CUR_FALL_T      (     1 )     /* direct current falling time [s] after startup */
#define MIN_WM_FRE_HZ   (    30 )       /* Minimum Windmilling Frequency [Hz] - 360RPM*/
#define WINDMILL_CUR_AMP (     0.4 )    /* Post Windmill startup current [A] */
#define RGN_BRK_CUR_AMP (     0.4 )     /*Regenerative Braking current*/
#define WINDMILLING_TIME_SEC  (0.3)     /* Windmilling Time in Seconds*/
#define PASSIVE_BRAKING_TIME_SEC       (2) /*Motor Braking Time in Seconds*/
#define TORQUE_MODE_MIN_CUR_AMP 0.2     /* Minimum Torque Mode Reference in A */
#endif  /* ifdef LEADSHINE_EL5_M0400_1_24 */
/* board related parameters */
/* Note: only one board type has to be selected! */

/* dsPICDEM MCLV-2 Board related parameters */
#ifdef MCLV2
#define CUR_SGN_REV     /* current sign is reversed!!! */
#define AD_RBA          (  465.4545 )  /*8.8A, 1A <-> 465.45 bit */ 
#define AD_RBV          (   77.57 )  /* 52.8V, 1V <->  77.57 bit */
#endif /* ifdef MCLV2 */

/* dsPICDEM MCHV-3 Board related parameters */
#ifdef MCHV3
#define CUR_SGN_REV     /* current sign is reversed!!! */
#define AD_RBA          (  124.876 )  /*32.79A, 1A <-> 124.87 bit */ 
#define AD_RBV          (   9.03 )  /* 453V, 1V <->  9 bit */
#endif  /* ifdef MCHV3 */

#define AD_FULLRANGE    ( 4096 )

#define MC_FREQ_HZ      ( 48000000.0f ) /* PWM peripheral frequency */

/* set the default start speed  in rpm */
#define START_SPEED_DEFAULT    (600U)

/* slower sampling frequency [Hz] (speed ramps and slower functions) */
#define SAMF_SLOW       (  100 )  

#define DEADT_TICKS     (   48U )		/* 48 ticks @48MHz -> 1us */



/* phase lost alarm counters limit, filters time constant */
/* time constant is (1<<11)/Fs (=682ms @3kHz) */
#define SH_PHL_FIL                     ( 11 )    
/* half second counter time */
#define PHLOST_CNT_MAX                 ( (uint16_t)SAMF_SLOW >> 1U )  
#define SH_THR_PHLOST                  ( 11 )          /* threshold amplification */
#define K_THR_PHLOST   ( 1365 )        /* (1<<11)*(4/3)*(1/2), threshold is 0.5 */


/* derived and control parameters */
/* useful in duty cycle calculation */
#define HALFPER_NRM     ((uint16_t)((float32_t)PWM_HPER_TICKS * BASE_VALUE_FL / 32768.0f)) 
/* useful in voltages re-calculation */
#define HALFPER_A15     ((uint32_t)((float32_t)PWM_HPER_TICKS * 32768.0f))     
/* 60 ticks @48MHz -> 1.25us (greater than deadtimes!) */
#define DMIN_TICKS      (DEADT_TICKS + 12U)   
/* max delta duty */
#define DELMAX_TICKS    (PWM_HPER_TICKS - DMIN_TICKS)    
/* converting DELMAX_TICKS to absolute float calculation */
#define DELMAX_TICKS_FL ((float32_t)PWM_HPER_TICKS - ((float32_t)DEADT_TICKS + 12.0f))    
/* max ratio of vbus which is possible to obtain with modulation */
#define K_MODLOSSES     ((DELMAX_TICKS_FL) / (float32_t)PWM_HPER_TICKS)  
/* linear modulation */
#define K_AVAIL_VOL     ((int16_t)(K_MODLOSSES * (float32_t)ONEBYSQRT3)) 
/* overmodulation */
/*#define K_AVAIL_VOL     ((int16_t)(K_MODLOSSES * TWOTHIRDS))*/    
/* maximum voltage readable by the A/D converter */
#define MAX_VOL   ((float32_t)AD_FULLRANGE / (float32_t)AD_RBV)  
/* maximum current readable by the A/D converter */
#define MAX_AMP   (0.5f * (float32_t)AD_FULLRANGE / (float32_t)AD_RBA)
/* maximum speed considered */
#define MAX_SPE_RS      (2.0f * FLOAT_PI * (float32_t)MAX_FRE_HZ)   
/* minimum speed considered */
#define MIN_SPE_RS      (2.0f * FLOAT_PI * (float32_t)MIN_FRE_HZ)   
/*minimum windmilling speed in Rad/S */
#define MIN_WM_SPE_RS      (2.0f * FLOAT_PI * (float32_t)MIN_WM_FRE_HZ)   
/* no deep flux weakening: all the voltages considered are lesser than vbus */
#define BASE_VOLTAGE    ((float32_t)1 * MAX_VOL)      
/*      deep flux weakening: bemf level can be much higher than max vbus */
/*#define BASE_VOLTAGE    ((float32_t)(4 * MAX_VOL)) */ 
/* normal condition */
#define BASE_CURRENT    ((float32_t)1 * MAX_AMP)      
/*#define BASE_CURRENT    ((float32_t)(0.5 * MAX_AMP))     to increase current resolution */
#define BASE_SPEED      ((float32_t)MAX_SPE_RS)
/* sampling frequency [Hz] */
#ifdef CTRL_PWM_1_1
#define SAMPLING_FREQ   ((0.5f * MC_FREQ_HZ / (float32_t)PWM_HPER_TICKS))  // 0.5f if PWM and control freq same; 0.25 if Control freq is half of PWM
#else
#define SAMPLING_FREQ   ((0.25f * MC_FREQ_HZ / (float32_t)PWM_HPER_TICKS))  // 0.5f if PWM and control freq same; 0.25 if Control freq is half of PWM
#endif 
/* K Time */
#define K_TIME          SAMPLING_FREQ
/* conversion constant: voltage[internal voltage unit] = K_VOLTAGE * voltage[volt] */
#define K_VOLTAGE       (BASE_VALUE_FL / BASE_VOLTAGE)  
/* conversion constant: current[internal current unit] = K_CURRENT * current[ampere] */
#define K_CURRENT       (BASE_VALUE_FL / BASE_CURRENT)
/* conversion constant: speed[internal speed unit] = K_SPEED * speed[rad/sec] */  
#define K_SPEED         (BASE_VALUE_FL / BASE_SPEED)  
/* a second internal speed unit is needed, to obtain angles as integral of the speed */
#define K_SPEED_L       ((uint16_t)(BASE_SPEED * (32768.0f / FLOAT_PI) / K_TIME)) 
/* speed[second internal speed unit] = K_SPEED_L * speed[internal speed unit] */

/* Maximum angle step in internal unit per sample at maximum speed*/
#define MAX_ANGLE_STEP_PER_SAMPLE  ((uint16_t)((32768/FLOAT_PI)*(BASE_SPEED/SAMPLING_FREQ)))

/* Blanking for 'BEMF_ANGLE_BLANK_COUNT' samples is started after the angle roll over from 2*PI to 0 or 0 to 2*PI
 * This blanking helps in removing an possible noise which would masquerade as angle roll over
 * The blanking count is set at half the number of control samples needed for a true angle roll over at max speed
 * ( 0 -> 65535, translates to 0 -> 2*PI) */
#define BEMF_ANGLE_BLANK_COUNT ((uint16_t)(65535/(2*MAX_ANGLE_STEP_PER_SAMPLE)))

/* due to chosen base values, the conversion constants from the A/D result to the internal
 representation are particularly simple:
    KAD_VOL =K_VOLTAGE/AD_RBV=
            =(BASE_VALUE/BASE_VOLTAGE)/AD_RBV=
            =(BASE_VALUE/(X*MAX_VOL))/AD_RBV=
            =(BASE_VALUE/(X*AD_FULLRANGE/AD_RBV))/AD_RBV=
            =BASE_VALUE/(X*AD_FULLRANGE)
    KAD_CUR =K_CURRENT/AD_RBV=
            =(BASE_VALUE/BASE_CURRENT)/AD_RBA=
            =(BASE_VALUE/(Y*MAX_AMP))/AD_RBA=
            =(BASE_VALUE/(0.5*Y*AD_FULLRANGE/AD_RBA))/AD_RBA=
            =2*BASE_VALUE/(Y*AD_FULLRANGE)
*/
/* (BASE_VALUE / (1 * AD_FULLRANGE / AD_RBV)) / AD_RBV */
#define KAD_VOL         (  4 )  
#ifndef  CUR_SGN_REV
/* (2 * BASE_VALUE / (1 * AD_FULLRANGE / AD_RBA)) / AD_RBA */
#define KAD_CUR         (  8 )  
#else   /* ifndef CUR_SGN_REV */
/* (2 * BASE_VALUE / (1 * AD_FULLRANGE / AD_RBA)) / AD_RBA */
#define KAD_CUR         ( -8 )  
#endif  /* ifndef CUR_SGN_REV */
#define SH_KAD_CUR      (  0 )
#define SH_KAD_VOL      (  0 )

/* remeber to update the filters time constants when changing Fs */
/* rising time constant is (1<<10)/Fs (=128ms @6kHz) */
#define SH_VBMIN_INC    ( 10 )  
/* falling time is (1<<(10-5))/Fs (=2ms @6kHz) */
#define SH_VBMIN_DEC    (  4 )  
/* time constant is (1<<10)/Fs (=128ms @6kHz) */
#define SH_REFSPEED_FIL ( 10 )  
/* time constant is (1<<10)/Fs (=128ms @6kHz) */
#define SH_MEAS_FIL     ( 10 )  
/* minimum bus voltage in internal units */
#define VBUSMIN         ((int16_t)(0.2f * BASE_VALUE_FL))   
/* ratio of total available voltage reserved for d axis */
#define DVOL_MARG       ((int16_t)(0.9f * BASE_VALUE_FL))   
 /* max speed [rpm] */
#define MAX_SPE_RPM     ((uint16_t)(60.0f * (float32_t)MAX_FRE_HZ / (float32_t)POLAR_COUPLES))   
/* min speed [rpm] */
#define MIN_SPE_RPM     ((uint16_t)(60 * MIN_FRE_HZ / POLAR_COUPLES))    
/* speed[rpm] = (K_INTSPE2RPM * speed[int]) / BASE_VALUE */
#define K_INTSPE2RPM    MAX_SPE_RPM     
/* speed[int] = (speed[rpm] * BASE_VALUE) / K_INTSPE2RPM */

/* floating equivalent of MAX_SPE_RPM to avoid MISRA errors */
/* max speed [rpm] */
#define MAX_SPE_RPM_FL     ((60.0f * (float32_t)MAX_FRE_HZ / (float32_t)POLAR_COUPLES))    

#define MAX_SPE          BASE_VALUE_INT
#define MAX_SPE_FL       BASE_VALUE_FL
/* used in position lost alarm management */
#define MAX_SPE_PLOST   ((uint16_t)(1.25f * BASE_VALUE_FL))        
#define MIN_SPE         ((uint16_t)(BASE_VALUE_FL * MIN_SPE_RS / BASE_SPEED))
#define MIN_WM_SPE      ((uint16_t)(BASE_VALUE_FL * MIN_WM_SPE_RS / BASE_SPEED)) 
/* used in position lost alarm management */
#define MIN_SPE_PLOST   ((uint16_t)(0.3f * (BASE_VALUE_FL * MIN_SPE_RS / BASE_SPEED))) 
/* position lost alarm counter limit */
#define POS_LOST_CNTMX  ( 300 ) 
/* number of sampling periods for startup acceleration */
#define STUP_TICKS      ((int16_t)(SAMPLING_FREQ * (float32_t)STUP_ACCTIME_S)) 
/* delta speed for each sampling period during startup */
#define DSPEEDL_STUP    ((uint32_t)(MIN_SPE * K_SPEED_L / STUP_TICKS))   
/* 333.32 micro second = 1, hence 3 for 1ms */
#define SYN_VAL1MS       ((uint16_t)3U) 
/* timer for 10ms routines calling */
#define SYN_VAL10MS      ((uint16_t)(SAMPLING_FREQ / (float32_t)SAMF_SLOW)) 
/* 333.32 micro second = 1, hence 300 for 100ms */
#define SYN_VAL100MS     ((uint16_t)300U) 
/* 333.32 micro second = 1, hence 3000 for 1s */
#define SYN_VAL1000MS    ((uint16_t)3000U) 

/* Internal Unit calculation for Windmilling Time*/
#define WINDMILLING_TIME_IU      ((uint16_t)(SAMPLING_FREQ * (float32_t)WINDMILLING_TIME_SEC)) 

/* Internal Unit calculation for Braking Time*/
#define BRAKING_TIME_IU      ((uint16_t)(SAMPLING_FREQ * (float32_t)PASSIVE_BRAKING_TIME_SEC)) 

/*This value provides half the value of expected angle rollovers i.e. 2*PI->0 or 0 -> 2*PI*/
#define HALF_MIN_ANGLE_ROLLOVER  ((uint16_t)(MIN_WM_FRE_HZ*WINDMILLING_TIME_SEC*0.5))

/* from the required acceleration ramps in rpm we derive the quantities 
   to add to the reference each main loop (10ms) cycle */
#define ACC_RAMP        ((int16_t)(((float32_t)ACC_RPM_S * BASE_VALUE_FL / MAX_SPE_RPM_FL) / (float32_t)SAMF_SLOW))
#define DEC_RAMP        ((int16_t)(((float32_t)DEC_RPM_S * BASE_VALUE_FL / MAX_SPE_RPM_FL) / (float32_t)SAMF_SLOW))
/* maximum current in internal current units */
#define MAX_CUR         ((int16_t)((float32_t)MAX_CUR_AMP * BASE_VALUE_FL / BASE_CURRENT))  
#define MAX_CUR_SQUARED (uint32_t)(MAX_CUR*MAX_CUR)
/* startup current in internal current units */
#define START_CUR       ((int16_t)((float32_t)START_CUR_AMP * BASE_VALUE_FL / BASE_CURRENT)) 

/*Minimum Torque mode reference current*/
#ifdef TORQUE_MODE
#define TORQUE_MODE_MIN_CUR       ((int16_t)((float32_t)TORQUE_MODE_MIN_CUR_AMP * BASE_VALUE_FL / BASE_CURRENT)) 
#endif
#define WINDMILL_START_CUR ((int16_t)((float32_t)WINDMILL_CUR_AMP * BASE_VALUE_FL / BASE_CURRENT)) 

#define REGEN_BRAKE_CURRENT ((int16_t)((float32_t)RGN_BRK_CUR_AMP * BASE_VALUE_FL / BASE_CURRENT)) 
/* amplified (to increase resolution) startup current */
#define START_CURL      ((int32_t)(START_CUR * (int32_t)BASE_VALUE_INT))      
/* startup current rising ramp (amplified) */
#define CUR_RAMP_AL     ((int32_t)(START_CURL / (int32_t)((SAMPLING_FREQ * (float32_t)CUR_RISE_T)))) 
/* direct current falling ramp (amplified) */
#define CUR_RAMP_RU     ((int32_t)(START_CURL / (int32_t)((SAMPLING_FREQ * (float32_t)CUR_FALL_T)))) 
/* Regenerative Braking Unit ramp rate in Internal unit per sample.*/
#define RGN_BRK_RAMP_IU ((int16_t) 1) 

/* current loop proportional gain [int] */
#define KP_CURPIF       ((K_VOLTAGE * (float32_t)KP_V_A / K_CURRENT))    
/* current loop integral gain [int] */
#define KI_CURPIF       ((K_VOLTAGE * (float32_t)KI_V_AS / (K_CURRENT * K_TIME))) 
/* amplification shifts in current PI calculation */
#define SH_PROC         ( 10U )           
/* amplification shifts in current PI calculation */
#define SH_INTC         (  6U )           
#define KP_CUR          ((int32_t)(KP_CURPIF * (float32_t)(((uint16_t)1 << (uint16_t)SH_PROC))))
#define KI_CUR          ((int32_t)(KI_CURPIF * (float32_t)(((uint32_t)1 << (uint32_t)(SH_INTC + SH_PROC)))))
/* speed loop proportional gain [int] */
#define KP_SPEPIF       ((K_CURRENT * (float32_t)KP_AS_R / K_SPEED))       
/* speed loop integral gain [int] */
#define KI_SPEPIF       ((K_CURRENT * (float32_t)KI_A_R / (K_SPEED * K_TIME)))  
/* amplification shifts in speed PI calculation */
#define SH_PROS         ( 10U )           
/* amplification shifts in speed PI calculation */
#define SH_INTS         (  6U )           
#define KP_SPE          ((int16_t)(KP_SPEPIF * (float32_t)(((uint16_t)1 << (uint16_t)SH_PROS))))
#define KI_SPE          ((int16_t)(KI_SPEPIF * (float32_t)(((uint32_t)1 << (uint32_t)(SH_INTS + SH_PROS)))))
/* conversion constant current[mA] = K_INTCUR2MA * current[internal current unit] */
#define K_INTCUR2MA     ((uint16_t)(BASE_VALUE_FL * 1000.0f / K_CURRENT))
/* conversion constant voltage[V/10] = K_INTVOL2DV * voltage[internal voltage unit] */ 
#define K_INTVOL2DV     ((uint16_t)(BASE_VALUE_FL * 10.0f / K_VOLTAGE))  

/* uncomment the following macro to align speed reference filter before change */
#define SPREF_FIL_ALIGN
#define RL_500MS_10MS 50u

/*Debug Feature: Defining CURPI_TUN enables current PI tuning mode in which a step current reference is generated to observe step response of the current controller*/ 
#undef CURPI_TUN 
/* step amplitude [A] */
#define CUR_STEP_AMP    ( 1.0 )     
#define CUR_STEP_VAL    ((int16_t)(CUR_STEP_AMP * BASE_VALUE / BASE_CURRENT))
/* step duration [sec] */
#define CUR_STEP_TIM    ( 0.5 )     
#define CPT_CNT_VAL     ((uint16_t)(CUR_STEP_TIM * SAMF_SLOW))

#define OVERCURRENT_RESET_DELAY_SEC     3
#define OVERCURRENT_RESET_DELAY_COUNT  (uint32_t) (OVERCURRENT_RESET_DELAY_SEC*100)  // Delay coun value calculated based 10mS unit.
/*Defining this macro enables Windmilling detect*/
#define WINDMILLING_ENABLE

    #ifdef WINDMILLING_ENABLE
        /*Defining ACTIVE_VECTOR_WINDMILLING enables Windmilling in active vector mode : This mode results in low braking torque but can have noisy angle tracking 
         * Undefining ACTIVE_VECTOR_WINDMILLING enables Windmilling in null vector mode: This mode results in high braking torque which increases with speed but has relatively noise free angle tracking */
        #define ACTIVE_VECTOR_WINDMILLING

        /*Defining WINDMILLING_CALIBRATION puts the algorithm in Windmilling state perpetually. This mode is mainly used to debug 
          angle and speed tracking in windmilling mode*/
        //#define WINDMILLING_CALIBRATION
    #endif // WINDMILLING_ENABLE



#endif // USERPARAMS_H

