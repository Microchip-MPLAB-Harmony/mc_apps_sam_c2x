/*******************************************************************************
  System Definitions

  File Name:
    mc_app.h

  Summary:
    Header file which shares global variables and function prototypes.

  Description:
    This file contains the global variables and function prototypes for a motor control project.

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

#ifndef MC_APP_H
#define MC_APP_H

#include "userparams.h"
#include "q14_generic_mcLib.h"


#define MACRO_DEBUG 
/*******************************************************************************
Public typedefs
*******************************************************************************/
typedef struct
{
    uint32_t inputVal;  /* read value of button input pin */
    uint16_t state;
    uint16_t cnt;
} button_response_t;

typedef enum
{
    NO_ALRM,
    POS_LOST_ALL,
    OVERCURRENT_ALL,
	PHASE_U_LOST_ALL,
	PHASE_V_LOST_ALL,
	PHASE_W_LOST_ALL
} alrm_t;

typedef enum
{
#ifdef FLYING_START_ENABLE    
    FLYING_START,
#endif
    STOPPED,
    ALIGNING,
    STARTING,
    CLOSINGLOOP,
    RUNNING
} run_status_t;

typedef enum
{
    NO_START_CMD,
    ALARM_STOP,
    CUR_PI_NO_START_CMD,
    CUR_PI_END,
    POS_LOST_STOP,
    PHASE_U_LOSS,
    PHASE_V_LOSS,
    PHASE_W_LOSS,            
    OC_FAULT_STOP
} stop_source_t;
/*******************************************************************************
Public variables definition
*******************************************************************************/
extern run_status_t
    motor_status;  /* status variable in motor management state machine */

extern alrm_t motor_alarm;
extern uint8_t state_run;

/* used only for displaying graphs non safety variable */
extern int16_t d_cur_a1000;
extern int16_t q_cur_a1000;
extern int16_t d_vol_v10;
extern int16_t q_vol_v10;
extern uint16_t bus_vol_v10;

extern int16_t spe_ref_sgn;

extern uint16_t rpm_ref_abs;
extern uint16_t spe_rpm_abs;
extern int16_t cur_mea[3];

extern uint16_t
  ufil,      /* u-phase absolute value filter output [internal current unit] */
  vfil,      /* v-phase absolute value filter output [internal current unit] */
  wfil,      /* w-phase absolute value filter output [internal current unit] */
  uall_cnt,  /* u-phase phase lost alarm counter */
  vall_cnt,  /* v-phase phase lost alarm counter */
  wall_cnt;  /* w-phase phase lost alarm counter */

extern uint8_t start_toggle;
extern uint8_t direction;
extern uint8_t phaseindex[3];
extern uint16_t adc_dc_bus_voltage;
extern uint16_t pot_input;
extern uint32_t trigger;
extern uint32_t state_count;
#ifdef FLYING_START_ENABLE 
extern uint16_t state_flying_start;
#endif

extern uint16_t state_stopped;
extern uint16_t state_start;
extern uint16_t state_align;
extern uint16_t state_closingloop;
extern uint16_t state_closedloop;
/*******************************************************************************
Private variables definition
*******************************************************************************/
extern pi_cntrl_t
    id_pi,
    iq_pi,
    sp_pi;

extern pi_cntrl_iv_t
    id_pi_var,
    iq_pi_var,
    sp_pi_var;


#ifdef	MACRO_DEBUG
extern uint16_t     acc_ramp;
extern uint16_t     dec_ramp;
#endif

/* visualization variables and
its dependent filters */
/* These variables are used in speed_ramp function that is outside
the control loop , if a variable is required for safety purpose
the same  should be done with a diverse storage */
extern uint16_t     syn_cnt;
extern uint16_t     vis_cnt;

extern uint16_t     syn1000ms_cnt;

extern int16_t      ext_speed_ref_rpm;
extern int16_t      
    ref_sgn,
    sgn_mem;
extern uint16_t     
    spe_ref_abs,
    ref_abs,
    espabs_fil;
extern int16_t
    vbfil,
    vdfil,
    vqfil,
    idfil,
    iqfil;

extern vec2_t
    curabm,      /* two-phases (a, b) vector of current measurement [internal current unit] */
    curabr,      /* two-phases (a, b) vector of current reference [internal current unit] */
    curdqm,      /* two-phases (d, q) vector of current measurement [internal current unit] */
    curdqr,      /* two-phases (d, q) vector of current reference [internal voltage unit] */
    outvab,      /* two-phases (a, b) vector of output voltage reference [internal voltage unit] */
    prev_outvab, /* two-phases (a, b) vector of output voltage reference of previous cycle [internal voltage unit] */
    outvdq;      /* two-phases (d, q) vector of output voltage reference [internal voltage unit] */

extern uint16_t
    newsysph,    /* next cycle system phase [(d, q) reference system 
                    angular position referred to alpha (u) axis] [internal angle unit] */
    position_offset,/* angular offset to be added to system phase [internal angle unit] */
    spe_ref_fil,  /* filtered speed reference absolute value [internal speed unit] */
    elespeed_abs;  /* estimated speed absolute value [internal speed unit] */

/* system phase [(d, q) reference system angular 
                position referred to alpha (u) axis] [internal angle unit] */
extern ang_sincos_t
    sysph;   

extern uint32_t        
    spe_ref_mem; /* filter memory in reference speed lp filter */

extern int32_t 
    dcurref_l;    /* amplified direct current reference */


extern uint16_t assert_active_vector;
extern int16_t  elespeed;    /* (estimated or imposed) electrical speed [internal speed unit] */


/*Debug variables; One time write variables */
#ifdef	MACRO_DEBUG
extern uint16_t
    sampling_freq,
    k_avail_vol,
    max_spe_rs,
    min_spe_rs,
    max_spe_rpm,
    min_spe_rpm,
    k_speed_l,
    k_intspe2rpm,
    stup_ticks,
    dspeedl_stup,
    max_cur,
    start_cur,
    cur_ramp_al,
    cur_ramp_ru;
#endif	


/*******************************************************************************
Private functions prototypes
*******************************************************************************/

#ifdef	MACRO_DEBUG
void macro_debug(void);
#endif	// ifdef MACRO_DEBUG

/*******************************************************************************
Public functions prototypes
*******************************************************************************/


/******************************************************************************
Function:		syn10ms
Description:	synchronization function for the main loop
Input:			nothing
Output:			1 if the synchronization period is not elapsed, 0 if it is elapsed
Revision:		1.0
******************************************************************************/
uint8_t syn10ms(void);
uint8_t syn1000ms(void);
/******************************************************************************
Function:		motorcontrol_vars_init
Description:	initialization function for some internal variables
Input:			nothing
Output:			nothing
Revision:		1.0
******************************************************************************/
void motorcontrol_vars_init(void);

/******************************************************************************
Function:		speed_ramp
Description:	reference speed ramp calculation (given speed reference, calculates
				an internal reference which reaches the external reference with a ramp)
Input:			nothing
Output:			nothing
Modifies:		internal speed reference
Note:			to be called in main loop, every 10ms 
Revision:		1.0
******************************************************************************/
void speed_ramp(void);

/******************************************************************************
Function:		current_measurement_management
Description:	conversion of the A/D conversion results in internal current values
Input:			nothing
Output:			nothing
Modifies:		internal current sample values
Revision:		1.0
******************************************************************************/
void current_measurement_management(void);

/******************************************************************************
Function:		motorcontrol
Description:	FOC implementation (one step)
Input:			nothing
Output:			nothing
Modifies:		many internal variables; it performs one step of FOC control,
				from measured values sampling (current and DC link voltage),
				till output driving signals generation; also rotor position
				estimation is performed
Revision:		1.0
******************************************************************************/
#ifdef RAM_EXECUTE
void  __ramfunc__ motorcontrol(void);
#else
void motorcontrol(void);
#endif
void motor_stop(void);

/******************************************************************************
Function:    motor_start
Description:  pwm output pins enable function
Input:      nothing
Output:      nothing (modifies some global variables)
Note:      to be called to enable pwm outputs
******************************************************************************/
void motor_start(void);

/******************************************************************************
Function:    phase_lost_check
Description:  checking if one of the three motor connections is broken
Input:      nothing (uses as input the global variables ufil, vfil, wfil,
        which are the low-pass filtered absolute values of the current
        readings.
Output:      nothing (can set the global alarm, if needed)
Note:      this function can be called in the main loop every 10ms;
        the time constant of the filters (much higher than 10ms)
        assures than no aliasing is present;
        the working principle is the following: if one of the three
        lowpass-filtered absolute values of the currents is significantly
        lesser than the median of the three, this means that one of the three
        is missing; this is true is the filter time constant is higher
        than the sinusoid period; we retain that one of the three is
        significantly lesser than the median if it is lesser than (3/4)
        of the media (0.75)
******************************************************************************/
void phase_lost_check(void);


/******************************************************************************
Function:    pos_lost_control_reset
Description:  reset of internal variables used in "pos_lost_control"
Input:      nothing
Output:      nothing
Modifies:    global variable plost_cnt
******************************************************************************/
void pos_lost_control_reset(void);


/******************************************************************************
Function:    phase_lost_filters_reset
Description:  reset of the filters used for the phase lost check and also
        of the related alarm counters
Input:      nothing (uses global variables)
Output:      nothing (modifies global variables)
Note:      to be called in the main interrupt, when the output frequency
        is lesser than the minimum required for the routine (ex. when
        aligning)
******************************************************************************/
void phase_lost_filters_reset(void);


#endif /* MC_APP_H */
