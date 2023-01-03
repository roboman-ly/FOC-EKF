/*
 * File: foc.c
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

#include "foc.h"


/* External inputs (root inport signals with default storage) */
ExtU rtU;

/* External outputs (root outports fed by signals with default storage) */
ExtY rtY;

/* Real-time model */
static RT_MODEL rtM_;
RT_MODEL *const rtM = &rtM_;
EKF_DATA_DEF ekf_data;
PI iqPI;
PI idPI;
/*foc各种参数的初始化*/
void foc_init(void)
{
    rtU.Tpwm=7200;
    rtU.ud=0;
    rtU.uq=0;
    rtU.udc=12;
    rtU.iq=0.4;    //目标iq  转矩
    rtU.id=0;
}
/*PI控制器*/
void PI_init(void)
{
    iqPI.kp=1.2;
    iqPI.ki=0.30;
    iqPI.out_max=12;
    iqPI.iout_max=10;
    idPI.kp=0.6;
    idPI.ki=0.1;
    idPI.out_max=12;
    idPI.iout_max=10;
}

void All_foc_init(void)
{
    foc_init();
    PI_init();
    Apt_Ekf_Init();
}
void PI_control(PI*sptr,real32_T error)
{
    sptr->out=sptr->kp*error+sptr->ki*sptr->sum_err;
    sptr->sum_err+=error;
    //积分限幅
    if(sptr->sum_err>=sptr->iout_max)sptr->sum_err=sptr->iout_max;
    else if (sptr->sum_err<=-sptr->iout_max) sptr->sum_err=-sptr->iout_max;
    //输出限幅
    if(sptr->out>=sptr->out_max)sptr->out=sptr->out_max;
    else if (sptr->out<=-sptr->out_max) sptr->out=-sptr->out_max;
}


/* Model step function */
void foc_step(void)
{
  int32_T sector;
  real32_T T1;
  real32_T T1_tmp;
  real32_T rtb_ualpha;
  real32_T rtb_ubeta;
  real32_T sin_theta;
  real32_T cos_theta;

  /* MATLAB Function: '<S1>/rePark' incorporates:
   *  Inport: '<Root>/theta'
   *  Inport: '<Root>/ud'
   *  Inport: '<Root>/uq'
   */
  /*电流闭环*/

//  /*电流采样值*/
  rtY.ia=(Curbuf[0]-2048)*3.3f/4096.0f/5.0f/0.05;
  rtY.ib=(Curbuf[1]-2048)*3.3f/4096.0f/5.0f/0.05;



  /*Clark*/
  rtY.ialpha=rtY.ia;
  rtY.ibeta=(rtY.ia+2*rtY.ib)/1.73205078F;


  /*Park*/
  sin_theta = fast_sin(rtU.theta);
  cos_theta = fast_cos(rtU.theta);
  rtY.iq = -rtY.ialpha * sin_theta + rtY.ibeta * cos_theta;
  rtY.id = rtY.ialpha * cos_theta + rtY.ibeta * sin_theta;

//
//  /*PI*/
  PI_control(&iqPI,rtU.iq-rtY.iq);
  PI_control(&idPI,rtU.id-rtY.id);
  rtU.uq=iqPI.out;
  rtU.ud=idPI.out;


//  rtU.uq=2;
//  rtU.ud=0;
  /*反Park*/
  T1 = fast_sin(rtU.theta);
  rtb_ubeta = fast_cos(rtU.theta);
  rtb_ualpha = (real32_T)rtU.ud * rtb_ubeta - (real32_T)rtU.uq * T1;
  rtb_ubeta = (real32_T)rtU.ud * T1 + (real32_T)rtU.uq * rtb_ubeta;
  rtU.ualpha=rtb_ualpha;
  rtU.ubeta=rtb_ubeta;

  /*扇区判断*/
  /* MATLAB Function: '<S1>/SVPWM MATLAB Function' */
  sector = 0;

  /* Outport: '<Root>/Tcmp1' incorporates:
   *  MATLAB Function: '<S1>/SVPWM MATLAB Function'
   */
  rtY.Tcmp1 = 0.0F;

  /* Outport: '<Root>/Tcmp2' incorporates:
   *  MATLAB Function: '<S1>/SVPWM MATLAB Function'
   */
  rtY.Tcmp2 = 0.0F;

  /* Outport: '<Root>/Tcmp3' incorporates:
   *  MATLAB Function: '<S1>/SVPWM MATLAB Function'
   */
  rtY.Tcmp3 = 0.0F;

  /* MATLAB Function: '<S1>/SVPWM MATLAB Function' incorporates:
   *  Inport: '<Root>/Tpwm'
   *  Inport: '<Root>/udc'
   */
  if (rtb_ubeta > 0.0F) {
    sector = 1;
  }

  if ((1.73205078F * rtb_ualpha - rtb_ubeta) / 2.0F > 0.0F) {
    sector += 2;
  }

  if ((-1.73205078F * rtb_ualpha - rtb_ubeta) / 2.0F > 0.0F) {
    sector += 4;
  }

  /*svpwm*/
  switch (sector) {
   case 1:
    T1_tmp = rtU.Tpwm / rtU.udc;
    T1 = (-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * T1_tmp;
    rtb_ualpha = (1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * T1_tmp;
    break;

   case 2:
    T1 = (1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc);
    rtb_ualpha = -(1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc);
    break;

   case 3:
    T1 = -((-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc));
    rtb_ualpha = 1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc;
    break;

   case 4:
    T1 = -(1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc);
    rtb_ualpha = (-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm /
      rtU.udc);
    break;

   case 5:
    T1 = 1.73205078F * rtb_ubeta * rtU.Tpwm / rtU.udc;
    rtb_ualpha = -((1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm /
      rtU.udc));
    break;

   default:
    T1 = -((1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm / rtU.udc));
    rtb_ualpha = -((-1.5F * rtb_ualpha + 0.866025388F * rtb_ubeta) * (rtU.Tpwm /
      rtU.udc));
    break;
  }

  rtb_ubeta = T1 + rtb_ualpha;
  if (rtb_ubeta > rtU.Tpwm) {
    T1 /= rtb_ubeta;
    rtb_ualpha /= T1 + rtb_ualpha;
  }

  rtb_ubeta = (rtU.Tpwm - (T1 + rtb_ualpha)) / 4.0F;
  T1 = T1 / 2.0F + rtb_ubeta;
  switch (sector) {
   case 1:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = T1;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = rtb_ubeta;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = rtb_ualpha / 2.0F + T1;
    break;

   case 2:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = rtb_ubeta;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = rtb_ualpha / 2.0F + T1;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = T1;
    break;

   case 3:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = rtb_ubeta;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = T1;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = rtb_ualpha / 2.0F + T1;
    break;

   case 4:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = rtb_ualpha / 2.0F + T1;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = T1;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = rtb_ubeta;
    break;

   case 5:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = rtb_ualpha / 2.0F + T1;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = rtb_ubeta;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = T1;
    break;

   case 6:
    /* Outport: '<Root>/Tcmp1' */
    rtY.Tcmp1 = T1;

    /* Outport: '<Root>/Tcmp2' */
    rtY.Tcmp2 = rtb_ualpha / 2.0F + T1;

    /* Outport: '<Root>/Tcmp3' */
    rtY.Tcmp3 = rtb_ubeta;
    break;
  }
  TIM_SetCompare1(TIM1,rtY.Tcmp1);
  TIM_SetCompare2(TIM1,rtY.Tcmp2);
  TIM_SetCompare3(TIM1,rtY.Tcmp3);
  /* Outport: '<Root>/sector' incorporates:
   *  MATLAB Function: '<S1>/SVPWM MATLAB Function'
   */
  rtY.sector = (real32_T)sector;

  /*EKF*/
  //ekf估计转子位置及电机参数辨识
  ekf_data.ekf_input[0] = rtU.ualpha;
  ekf_data.ekf_input[1] = rtU.ubeta;
  ekf_data.ekf_input[2] = rtY.ialpha;
  ekf_data.ekf_input[3] = rtY.ibeta;
  ekf_data.ekf_input[4] = RS_PARAMETER;               //电阻
  ekf_data.ekf_input[5] = LS_PARAMETER;               //电感
  ekf_data.ekf_input[6] = FLUX_PARAMETER;          //磁链

  Apt_Ekf_Update(&ekf_data.ekf_input[0], &ekf_data.ekf_states[0]);      //6ms/15ms = 40%

  rtU.theta = ekf_data.ekf_states[3];  //取出估计角度
  rtU.angle_speed=ekf_data.ekf_states[2];

}

