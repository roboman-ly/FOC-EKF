/*
 * File: foc.h
 *
 * Code generated for Simulink model 'foc'.
 *
 * Model version                  : 1.39
 * Simulink Coder version         : 9.5 (R2021a) 14-Nov-2020
 * C/C++ source code generated on : Tue Jan 11 15:22:36 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. RAM efficiency
 * Validation result: Not run
 */

#ifndef RTW_HEADER_foc_h_
#define RTW_HEADER_foc_h_
#include <math.h>
#ifndef foc_COMMON_INCLUDES_
#define foc_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "stdint.h"
#include "fast_math.h"
#include "adc.h"
#include "ekf.h"
#endif                                 /* foc_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
#define rtmGetErrorStatus(rtm)         ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
#define rtmSetErrorStatus(rtm, val)    ((rtm)->errorStatus = (val))
#endif



#define pi 3.1415926f

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* External inputs (root inport signals with default storage) */
typedef struct {
  real32_T ud;                           /* '<Root>/ud' */
  real32_T uq;                           /* '<Root>/uq' */
  real32_T theta;                      /* '<Root>/theta' */
  real32_T angle_speed;
  real32_T udc;                        /* '<Root>/udc' */
  real32_T Tpwm; //arr                   /* '<Root>/Tpwm' */
  real32_T iq;
  real32_T id;
  real32_T ialpha;
  real32_T ibeta;
  real32_T ualpha;
  real32_T ubeta;
} ExtU;

/* External outputs (root outports fed by signals with default storage) */
typedef struct {
  real32_T Tcmp1;                      /* '<Root>/Tcmp1' */
  real32_T Tcmp2;                      /* '<Root>/Tcmp2' */
  real32_T Tcmp3;                      /* '<Root>/Tcmp3' */
  real32_T sector;                     /* '<Root>/sector' */
  real32_T iq;
  real32_T id;
  real32_T ia;
  real32_T ib;
  real32_T ialpha;
  real32_T ibeta;
} ExtY;

typedef struct {
    real32_T kp;
    real32_T ki;
    real32_T out_max;
    real32_T iout_max;
    real32_T out;
    real32_T sum_err;
} PI;
typedef struct {
    real32_T es_f;
}smo;
/* Real-time Model Data Structure */
struct tag_RTM {
  const char_T * volatile errorStatus;
};

/* External inputs (root inport signals with default storage) */
extern ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
extern ExtY rtY;




/* Model entry point functions */

void foc_step(void);
void All_foc_init(void);
/* Real-time Model object */
extern RT_MODEL *const rtM;



/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('re_park_alpha/foc')    - opens subsystem re_park_alpha/foc
 * hilite_system('re_park_alpha/foc/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 're_park_alpha'
 * '<S1>'   : 're_park_alpha/foc'
 * '<S2>'   : 're_park_alpha/foc/SVPWM MATLAB Function'
 * '<S3>'   : 're_park_alpha/foc/rePark'
 */
#endif                                 /* RTW_HEADER_foc_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
