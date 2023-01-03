#ifndef __EKF_H
#define __EKF_H


#define RS_PARAMETER 0.4f//电阻
#define LS_PARAMETER  0.0014f//电感
#define FLUX_PARAMETER 0.002210119f//磁链
typedef struct
{
	float ekf_input[7];     //输入参数
	float ekf_states[4];    //状态变量
	
}EKF_DATA_DEF;
extern float Ts;
void Apt_Ekf_Init(void);
void Apt_Ekf_Update(const float *u, float *xD);

#endif  /*__EKF_H*/
