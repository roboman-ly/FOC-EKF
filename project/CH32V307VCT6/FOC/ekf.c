#include "ekf.h"

#include "fast_math.h"
//#include "config.h"
float Rs;        //电阻
float Ls;        //电感
float Flux;     //磁链

//过程噪声协方差矩阵
float Q_0_0;
float Q_0_1;
float Q_0_2;
float Q_0_3;
float Q_1_0;
float Q_1_1;
float Q_1_2;
float Q_1_3;
float Q_2_0;
float Q_2_1;
float Q_2_2;
float Q_2_3;
float Q_3_0;
float Q_3_1;
float Q_3_2;
float Q_3_3;

//测量噪声协方差矩阵
float R_0_0;
float R_0_1;
float R_1_0;
float R_1_1;

//误差协方差矩阵
float P0_0_0;
float P0_0_1;
float P0_0_2;
float P0_0_3;
float P0_1_0;
float P0_1_1;
float P0_1_2;
float P0_1_3;
float P0_2_0;
float P0_2_1;
float P0_2_2;
float P0_2_3;
float P0_3_0;
float P0_3_1;
float P0_3_2;
float P0_3_3;

float Ts; //步长，算法执行周期

//输入矩阵
float B_0_0;
float B_0_1;
float B_1_0;
float B_1_1;
float B_2_0;
float B_2_1;
float B_3_0;
float B_3_1;

//输入变量
float vs_ab_0_0;
float vs_ab_1_0;
float is_ab_0_0;
float is_ab_1_0;

//输出矩阵
float H_0_0;
float H_0_1;
float H_0_2;
float H_0_3;
float H_1_0;
float H_1_1;
float H_1_2;
float H_1_3;

//系统矩阵
float F_0_0;
float F_0_1;
float F_0_2;
float F_0_3;
float F_1_0;
float F_1_1;
float F_1_2;
float F_1_3;
float F_2_0;
float F_2_1;
float F_2_2;
float F_2_3;
float F_3_0;
float F_3_1;
float F_3_2;
float F_3_3;

// 单位阵
float temp_0_0;
float temp_0_1;
float temp_0_2;
float temp_0_3;
float temp_1_0;
float temp_1_1;
float temp_1_2;
float temp_1_3;
float temp_2_0;
float temp_2_1;
float temp_2_2;
float temp_2_3;
float temp_3_0;
float temp_3_1;
float temp_3_2;
float temp_3_3;

float f1_0_0;
float f1_1_0;
float f1_2_0;
float f1_3_0;

float f2_0_0;
float f2_0_1;
float f2_0_2;
float f2_0_3;
float f2_1_0;
float f2_1_1;
float f2_1_2;
float f2_1_3;
float f2_2_0;
float f2_2_1;
float f2_2_2;
float f2_2_3;
float f2_3_0;
float f2_3_1;
float f2_3_2;
float f2_3_3;

float X_pred_0_0;
float X_pred_1_0;
float X_pred_2_0;
float X_pred_3_0;

float Y_pred_0_0;
float Y_pred_1_0;

float Y_0_0;
float Y_1_0;

float P_pred_0_0;
float P_pred_0_1;
float P_pred_0_2;
float P_pred_0_3;    
float P_pred_1_0;
float P_pred_1_1;
float P_pred_1_2;
float P_pred_1_3;  
float P_pred_2_0;
float P_pred_2_1;
float P_pred_2_2;
float P_pred_2_3;
float P_pred_3_0;
float P_pred_3_1;
float P_pred_3_2;
float P_pred_3_3;

float temp_0_0_t;
float temp_0_1_t;
float temp_1_0_t;
float temp_1_1_t;
float temp;

float K_0_0;
float K_0_1;    
float K_1_0;
float K_1_1;   
float K_2_0;
float K_2_1;   
float K_3_0;
float K_3_1;

float tempa_0_0;
float tempa_1_0;
float tempa_2_0;
float tempa_3_0;

/*参数初始化*/
void Apt_Ekf_Init(void)
{
	Rs = RS_PARAMETER;//电阻
    Ls = LS_PARAMETER;//电感
    Flux = FLUX_PARAMETER;//磁链
	
	/*过程噪声协方差矩阵*/
	Q_0_0 = 0.1f;
	Q_0_1 = 0.0f;
	Q_0_2 = 0.0f;
	Q_0_3 = 0.0f;
	Q_1_0 = 0.0f;
	Q_1_1 = 0.1f;
	Q_1_2 = 0.0f;
	Q_1_3 = 0.0f;
	Q_2_0 = 0.0f;
	Q_2_1 = 0.0f;
	Q_2_2 = 0.08f;
	Q_2_3 = 0.0f;
	Q_3_0 = 0.0f;
	Q_3_1 = 0.0f;
	Q_3_2 = 0.0f;
	Q_3_3 = 0.005f;

	/*测量噪声协方差矩阵*/
	R_0_0 = 0.2f;
	R_0_1 = 0.0f;
	R_1_0 = 0.0f;
	R_1_1 = 0.2f;
	
	Ts = 0.0001f;    // kalman update time ：0.1ms
	
	/*输出矩阵*/
	H_0_0 = 1.0f;
	H_0_1 = 0.0f;
	H_0_2 = 0.0f;
	H_0_3 = 0.0f;
	H_1_0 = 0.0f;
	H_1_1 = 1.0f;
	H_1_2 = 0.0f;
	H_1_3 = 0.0f;

	/*输入矩阵*/
	B_0_0 = 1.0f/Ls;
	B_0_1 = 0.0f;
	B_1_0 = 0.0f;
	B_1_1 = 1.0f/Ls;
	B_2_0 = 0.0f;
	B_2_1 = 0.0f;
	B_3_0 = 0.0f;
	B_3_1 = 0.0f;

    /*误差协方差矩阵（估计值与实际值）*/
	P0_0_0 = 0.0f;
	P0_0_1 = 0.0f;
	P0_0_2 = 0.0f;
	P0_0_3 = 0.0f;
	P0_1_0 = 0.0f;
	P0_1_1 = 0.0f;
	P0_1_2 = 0.0f;
	P0_1_3 = 0.0f;
	P0_2_0 = 0.0f;
	P0_2_1 = 0.0f;
	P0_2_2 = 0.0f;
	P0_2_3 = 0.0f;
	P0_3_0 = 0.0f;
	P0_3_1 = 0.0f;
	P0_3_2 = 0.0f;
	P0_3_3 = 0.0f;
	
}

//输入u, 输出xD
void Apt_Ekf_Update(const float *u, float *xD)
{
	vs_ab_0_0 = u[0];   //ualpha
	vs_ab_1_0 = u[1];   //ubeta
	is_ab_0_0 = u[2];    //ialpha
	is_ab_1_0 = u[3];    //ibeta

	Rs = u[4];             //相电阻
	Ls = u[5];             //相电感
	Flux = u[6];           //磁链

    /*线性化后的系统矩阵A，用于公式二*/
	F_0_0 = -Rs/Ls;
	F_0_1 = 0.0f;
	F_0_2 = Flux/Ls*fast_sin(xD[3]);
	F_0_3 = Flux/Ls*xD[2]*fast_cos(xD[3]);
	F_1_0 = 0.0f;
	F_1_1 = -Rs/Ls;
	F_1_2 = -Flux/Ls*fast_cos(xD[3]);
	F_1_3 = Flux/Ls*xD[2]*fast_sin(xD[3]);
	F_2_0 = 0.0f;
	F_2_1 = 0.0f;
	F_2_2 = 0.0f;
	F_2_3 = 0.0f;
	F_3_0 = 0.0f;
	F_3_1 = 0.0f;
	F_3_2 = 1.0f;
	F_3_3 = 0.0f;
   
    /*单位阵，I阵，临时计算矩阵*/
	temp_0_0 = 1.0f;
	temp_0_1 = 0.0f;
	temp_0_2 = 0.0f;
	temp_0_3 = 0.0f;
	temp_1_0 = 0.0f;
	temp_1_1 = 1.0f;
	temp_1_2 = 0.0f;
	temp_1_3 = 0.0f;
	temp_2_0 = 0.0f;
	temp_2_1 = 0.0f;
	temp_2_2 = 1.0f;
	temp_2_3 = 0.0f;
	temp_3_0 = 0.0f;
	temp_3_1 = 0.0f;
	temp_3_2 = 0.0f;
	temp_3_3 = 1.0f;

	/*GX，非线性方程*/
	f1_0_0 = -Rs/Ls*xD[0]+Flux/Ls*xD[2]*fast_sin(xD[3]);
	f1_1_0 = -Rs/Ls*xD[1]-Flux/Ls*xD[2]*fast_cos(xD[3]);
	f1_2_0 = 0.0f;
	f1_3_0 = xD[2];
    
	/*线性离散化后的系统阵，f = I + Ts*A，用于公式二*/
	f2_0_0 = temp_0_0 + (Ts*F_0_0);
	f2_0_1 = temp_0_1 + (Ts*F_0_1);
	f2_0_2 = temp_0_2 + (Ts*F_0_2);
	f2_0_3 = temp_0_3 + (Ts*F_0_3);
	f2_1_0 = temp_1_0 + (Ts*F_1_0);
	f2_1_1 = temp_1_1 + (Ts*F_1_1);
	f2_1_2 = temp_1_2 + (Ts*F_1_2);
	f2_1_3 = temp_1_3 + (Ts*F_1_3);
	f2_2_0 = temp_2_0 + (Ts*F_2_0);
	f2_2_1 = temp_2_1 + (Ts*F_2_1);
	f2_2_2 = temp_2_2 + (Ts*F_2_2);
	f2_2_3 = temp_2_3 + (Ts*F_2_3);
	f2_3_0 = temp_3_0 + (Ts*F_3_0);
	f2_3_1 = temp_3_1 + (Ts*F_3_1);
	f2_3_2 = temp_3_2 + (Ts*F_3_2);
	f2_3_3 = temp_3_3 + (Ts*F_3_3);

  //kalman公式一， 预测方程（先验估计）：X(k|k-1)的估计 = X(k-1|k-1)的估计 + Ts*[Gx + B*U]
	X_pred_0_0 = xD[0] + Ts*(f1_0_0 + B_0_0*vs_ab_0_0 + B_0_1*vs_ab_1_0);    //iα 的估计，状态空间表达式
	X_pred_1_0 = xD[1] + Ts*(f1_1_0 + B_1_0*vs_ab_0_0 + B_1_1*vs_ab_1_0);    //iβ 的估计
	X_pred_2_0 = xD[2] + Ts*(f1_2_0 + B_2_0*vs_ab_0_0 + B_2_1*vs_ab_1_0);    //Ω 的估计
	X_pred_3_0 = xD[3] + Ts*(f1_3_0 + B_3_0*vs_ab_0_0 + B_3_1*vs_ab_1_0);     //θ 的估计
	
	//输出方程估计，估计值，用于公式4
	Y_pred_0_0 = H_0_0*X_pred_0_0 + H_0_1*X_pred_1_0 + H_0_2*X_pred_2_0 + H_0_3*X_pred_3_0;  //iα 的估计，输出方程
	Y_pred_1_0 = H_1_0*X_pred_0_0 + H_1_1*X_pred_1_0 + H_1_2*X_pred_2_0 + H_1_3*X_pred_3_0;  //iβ 的估计


	Y_0_0 = is_ab_0_0;  //iα 的测量，Clark变换的输出
	Y_1_0 = is_ab_1_0;  //iβ 的测量

    /*kalman公式二，先验误差协方差  ：P(k|k-1) = f系统矩阵*P(k-1|k-1)*f的转置 + Q矩阵，(f = I + Ts*F(k|k-1))*/
	//f*P   4*4矩阵
	P_pred_0_0 = f2_0_0*P0_0_0 + f2_0_1*P0_1_0 + f2_0_2*P0_2_0 + f2_0_3*P0_3_0;    //f阵第一行
	P_pred_0_1 = f2_0_0*P0_0_1 + f2_0_1*P0_1_1 + f2_0_2*P0_2_1 + f2_0_3*P0_3_1;
	P_pred_0_2 = f2_0_0*P0_0_2 + f2_0_1*P0_1_2 + f2_0_2*P0_2_2 + f2_0_3*P0_3_2;
	P_pred_0_3 = f2_0_0*P0_0_3 + f2_0_1*P0_1_3 + f2_0_2*P0_2_3 + f2_0_3*P0_3_3;    
	P_pred_1_0 = f2_1_0*P0_0_0 + f2_1_1*P0_1_0 + f2_1_2*P0_2_0 + f2_1_3*P0_3_0;     //f阵第二行
	P_pred_1_1 = f2_1_0*P0_0_1 + f2_1_1*P0_1_1 + f2_1_2*P0_2_1 + f2_1_3*P0_3_1;
	P_pred_1_2 = f2_1_0*P0_0_2 + f2_1_1*P0_1_2 + f2_1_2*P0_2_2 + f2_1_3*P0_3_2;
	P_pred_1_3 = f2_1_0*P0_0_3 + f2_1_1*P0_1_3 + f2_1_2*P0_2_3 + f2_1_3*P0_3_3;  
	P_pred_2_0 = f2_2_0*P0_0_0 + f2_2_1*P0_1_0 + f2_2_2*P0_2_0 + f2_2_3*P0_3_0;     //f阵第三行
	P_pred_2_1 = f2_2_0*P0_0_1 + f2_2_1*P0_1_1 + f2_2_2*P0_2_1 + f2_2_3*P0_3_1;
	P_pred_2_2 = f2_2_0*P0_0_2 + f2_2_1*P0_1_2 + f2_2_2*P0_2_2 + f2_2_3*P0_3_2;
	P_pred_2_3 = f2_2_0*P0_0_3 + f2_2_1*P0_1_3 + f2_2_2*P0_2_3 + f2_2_3*P0_3_3;
	P_pred_3_0 = f2_3_0*P0_0_0 + f2_3_1*P0_1_0 + f2_3_2*P0_2_0 + f2_3_3*P0_3_0;     //f阵第四行
	P_pred_3_1 = f2_3_0*P0_0_1 + f2_3_1*P0_1_1 + f2_3_2*P0_2_1 + f2_3_3*P0_3_1;
	P_pred_3_2 = f2_3_0*P0_0_2 + f2_3_1*P0_1_2 + f2_3_2*P0_2_2 + f2_3_3*P0_3_2;
	P_pred_3_3 = f2_3_0*P0_0_3 + f2_3_1*P0_1_3 + f2_3_2*P0_2_3 + f2_3_3*P0_3_3;
    //(f*P)*f的转置 + Q     4*4矩阵
	P_pred_0_0 = P_pred_0_0*f2_0_0 + P_pred_0_1*f2_0_1 + P_pred_0_2*f2_0_2 + P_pred_0_3*f2_0_3 + Q_0_0;
	P_pred_0_1 = P_pred_0_0*f2_1_0 + P_pred_0_1*f2_1_1 + P_pred_0_2*f2_1_2 + P_pred_0_3*f2_1_3 + Q_0_1;
	P_pred_0_2 = P_pred_0_0*f2_2_0 + P_pred_0_1*f2_2_1 + P_pred_0_2*f2_2_2 + P_pred_0_3*f2_2_3 + Q_0_2;
	P_pred_0_3 = P_pred_0_0*f2_3_0 + P_pred_0_1*f2_3_1 + P_pred_0_2*f2_3_2 + P_pred_0_3*f2_3_3 + Q_0_3;    
	P_pred_1_0 = P_pred_1_0*f2_0_0 + P_pred_1_1*f2_0_1 + P_pred_1_2*f2_0_2 + P_pred_1_3*f2_0_3 + Q_1_0;
	P_pred_1_1 = P_pred_1_0*f2_1_0 + P_pred_1_1*f2_1_1 + P_pred_1_2*f2_1_2 + P_pred_1_3*f2_1_3 + Q_1_1;
	P_pred_1_2 = P_pred_1_0*f2_2_0 + P_pred_1_1*f2_2_1 + P_pred_1_2*f2_2_2 + P_pred_1_3*f2_2_3 + Q_1_2;
	P_pred_1_3 = P_pred_1_0*f2_3_0 + P_pred_1_1*f2_3_1 + P_pred_1_2*f2_3_2 + P_pred_1_3*f2_3_3 + Q_1_3;    
	P_pred_2_0 = P_pred_2_0*f2_0_0 + P_pred_2_1*f2_0_1 + P_pred_2_2*f2_0_2 + P_pred_2_3*f2_0_3 + Q_2_0;
	P_pred_2_1 = P_pred_2_0*f2_1_0 + P_pred_2_1*f2_1_1 + P_pred_2_2*f2_1_2 + P_pred_2_3*f2_1_3 + Q_2_1;
	P_pred_2_2 = P_pred_2_0*f2_2_0 + P_pred_2_1*f2_2_1 + P_pred_2_2*f2_2_2 + P_pred_2_3*f2_2_3 + Q_2_2;
	P_pred_2_3 = P_pred_2_0*f2_3_0 + P_pred_2_1*f2_3_1 + P_pred_2_2*f2_3_2 + P_pred_2_3*f2_3_3 + Q_2_3; 
	P_pred_3_0 = P_pred_3_0*f2_0_0 + P_pred_3_1*f2_0_1 + P_pred_3_2*f2_0_2 + P_pred_3_3*f2_0_3 + Q_3_0;
	P_pred_3_1 = P_pred_3_0*f2_1_0 + P_pred_3_1*f2_1_1 + P_pred_3_2*f2_1_2 + P_pred_3_3*f2_1_3 + Q_3_1;
	P_pred_3_2 = P_pred_3_0*f2_2_0 + P_pred_3_1*f2_2_1 + P_pred_3_2*f2_2_2 + P_pred_3_3*f2_2_3 + Q_3_2;
	P_pred_3_3 = P_pred_3_0*f2_3_0 + P_pred_3_1*f2_3_1 + P_pred_3_2*f2_3_2 + P_pred_3_3*f2_3_3 + Q_3_3;

    /*kalman公式三，卡尔曼增益：K(k|k-1) = P(k|k-1)*H的转置/(H*P(k|k-1)*H的转置) + R)*/
	//H*P(k|k-1)   (2,4)*(4,4)  = (2,4)
	temp_0_0 = H_0_0*P_pred_0_0 + H_0_1*P_pred_1_0 + H_0_2*P_pred_2_0 + H_0_3*P_pred_3_0;
	temp_0_1 = H_0_0*P_pred_0_1 + H_0_1*P_pred_1_1 + H_0_2*P_pred_2_1 + H_0_3*P_pred_3_1;
	temp_0_2 = H_0_0*P_pred_0_2 + H_0_1*P_pred_1_2 + H_0_2*P_pred_2_2 + H_0_3*P_pred_3_2;
	temp_0_3 = H_0_0*P_pred_0_3 + H_0_1*P_pred_1_3 + H_0_2*P_pred_2_3 + H_0_3*P_pred_3_3;   
	temp_1_0 = H_1_0*P_pred_0_0 + H_1_1*P_pred_1_0 + H_1_2*P_pred_2_0 + H_1_3*P_pred_3_0;
	temp_1_1 = H_1_0*P_pred_0_1 + H_1_1*P_pred_1_1 + H_1_2*P_pred_2_1 + H_1_3*P_pred_3_1;
	temp_1_2 = H_1_0*P_pred_0_2 + H_1_1*P_pred_1_2 + H_1_2*P_pred_2_2 + H_1_3*P_pred_3_2;
	temp_1_3 = H_1_0*P_pred_0_3 + H_1_1*P_pred_1_3 + H_1_2*P_pred_2_3 + H_1_3*P_pred_3_3;
    //(H*P(k|k-1))*H的转置 + R  ,  (2,4)*(4,2) = (2,2)
	temp_0_0 = temp_0_0*H_0_0 + temp_0_1*H_0_1 + temp_0_2*H_0_2 + temp_0_3*H_0_3 + R_0_0;
	temp_0_1 = temp_0_0*H_1_0 + temp_0_1*H_1_1 + temp_0_2*H_1_2 + temp_0_3*H_1_3 + R_0_1;    
	temp_1_0 = temp_1_0*H_0_0 + temp_1_1*H_0_1 + temp_1_2*H_0_2 + temp_1_3*H_0_3 + R_1_0;
	temp_1_1 = temp_1_0*H_1_0 + temp_1_1*H_1_1 + temp_1_2*H_1_2 + temp_1_3*H_1_3 + R_1_1;

    //(2,2)矩阵求逆
	temp_0_0_t = temp_0_0;   //暂存估计误差，用作伴随阵
	temp_0_1_t = temp_0_1;
	temp_1_0_t = temp_1_0;
	temp_1_1_t = temp_1_1;

	temp = temp_0_0*temp_1_1 - temp_0_1*temp_1_0; //行列式
	if(temp != 0)     //行列式为分母，不能为0
	{
		//A的逆 = A的伴随/A的行列式
	   temp_0_0 = temp_1_1_t/temp;     //主对调，次反号
	   temp_0_1 = -temp_0_1_t/temp;
	   temp_1_0 = -temp_1_0_t/temp;
	   temp_1_1 = temp_0_0_t/temp;
	}
    // P(k|k-1)*H的转置，(4,4)*(4*2) = (4,2)
	K_0_0 = P_pred_0_0*H_0_0 + P_pred_0_1*H_0_1 + P_pred_0_2*H_0_2 + P_pred_0_3*H_0_3;
	K_0_1 = P_pred_0_0*H_1_0 + P_pred_0_1*H_1_1 + P_pred_0_2*H_1_2 + P_pred_0_3*H_1_3;    
	K_1_0 = P_pred_1_0*H_0_0 + P_pred_1_1*H_0_1 + P_pred_1_2*H_0_2 + P_pred_1_3*H_0_3;
	K_1_1 = P_pred_1_0*H_1_0 + P_pred_1_1*H_1_1 + P_pred_1_2*H_1_2 + P_pred_1_3*H_1_3;   
	K_2_0 = P_pred_2_0*H_0_0 + P_pred_2_1*H_0_1 + P_pred_2_2*H_0_2 + P_pred_2_3*H_0_3;
	K_2_1 = P_pred_2_0*H_1_0 + P_pred_2_1*H_1_1 + P_pred_2_2*H_1_2 + P_pred_2_3*H_1_3;   
	K_3_0 = P_pred_3_0*H_0_0 + P_pred_3_1*H_0_1 + P_pred_3_2*H_0_2 + P_pred_3_3*H_0_3;
	K_3_1 = P_pred_3_0*H_1_0 + P_pred_3_1*H_1_1 + P_pred_3_2*H_1_2 + P_pred_3_3*H_1_3;
	//P(k|k-1)*H的转置 * 逆阵 ， (4,2)*(2,2) = (4,2)
	K_0_0 = K_0_0*temp_0_0 + K_0_1*temp_1_0;  //卡尔曼增益
	K_0_1 = K_0_0*temp_0_1 + K_0_1*temp_1_1;  
	K_1_0 = K_1_0*temp_0_0 + K_1_1*temp_1_0;
	K_1_1 = K_1_0*temp_0_1 + K_1_1*temp_1_1;    
	K_2_0 = K_2_0*temp_0_0 + K_2_1*temp_1_0;
	K_2_1 = K_2_0*temp_0_1 + K_2_1*temp_1_1;   
	K_3_0 = K_3_0*temp_0_0 + K_3_1*temp_1_0;
	K_3_1 = K_3_0*temp_0_1 + K_3_1*temp_1_1;

    /*kalman公式四，修正估计值（后验估计）：X(k|k)的估计 = X(k|k-1)的估计 + K(k|k-1)增益*(量测的Y(k) - 估计的Y(k|k-1))*/
	//(4,2)*(2,1) = (4,1), 后验估计值
	tempa_0_0 = X_pred_0_0 + K_0_0*(Y_0_0 - Y_pred_0_0) + K_0_1*(Y_1_0 - Y_pred_1_0);  //ialpha
	tempa_1_0 = X_pred_1_0 + K_1_0*(Y_0_0 - Y_pred_0_0) + K_1_1*(Y_1_0 - Y_pred_1_0);   //ibeta
	tempa_2_0 = X_pred_2_0 + K_2_0*(Y_0_0 - Y_pred_0_0) + K_2_1*(Y_1_0 - Y_pred_1_0);  //omiga
	tempa_3_0 = X_pred_3_0 + K_3_0*(Y_0_0 - Y_pred_0_0) + K_3_1*(Y_1_0 - Y_pred_1_0);   //theta

	/*kalman公式五，更新误差协方差 ：P(k|k)  = (I - K(k|k-1)*H)*P(k|k-1) =  P(k|k-1) - K(k|k-1)*H*P(k|k-1)*/
	//K(k|k-1)*H ,(4,2)*(2,4) = (4,4)
	temp_0_0 = K_0_0*H_0_0 + K_0_1*H_1_0;
	temp_0_1 = K_0_0*H_0_1 + K_0_1*H_1_1;
	temp_0_2 = K_0_0*H_0_2 + K_0_1*H_1_2;
	temp_0_3 = K_0_0*H_0_3 + K_0_1*H_1_3;    
	temp_1_0 = K_1_0*H_0_0 + K_1_1*H_1_0;
	temp_1_1 = K_1_0*H_0_1 + K_1_1*H_1_1;
	temp_1_2 = K_1_0*H_0_2 + K_1_1*H_1_2;
	temp_1_3 = K_1_0*H_0_3 + K_1_1*H_1_3;    
	temp_2_0 = K_2_0*H_0_0 + K_2_1*H_1_0;
	temp_2_1 = K_2_0*H_0_1 + K_2_1*H_1_1;
	temp_2_2 = K_2_0*H_0_2 + K_2_1*H_1_2;
	temp_2_3 = K_2_0*H_0_3 + K_2_1*H_1_3;   
	temp_3_0 = K_3_0*H_0_0 + K_3_1*H_1_0;
	temp_3_1 = K_3_0*H_0_1 + K_3_1*H_1_1;
	temp_3_2 = K_3_0*H_0_2 + K_3_1*H_1_2;
	temp_3_3 = K_3_0*H_0_3 + K_3_1*H_1_3;
    //(4,4)*(4,4) = (4,4)
	P0_0_0 =   P_pred_0_0 - (temp_0_0*P_pred_0_0 + temp_0_1*P_pred_1_0 + temp_0_2*P_pred_2_0 + temp_0_3*P_pred_3_0);
	P0_0_1 =   P_pred_0_1 - (temp_0_0*P_pred_0_1 + temp_0_1*P_pred_1_1 + temp_0_2*P_pred_2_1 + temp_0_3*P_pred_3_1);
	P0_0_2 =   P_pred_0_2 - (temp_0_0*P_pred_0_2 + temp_0_1*P_pred_1_2 + temp_0_2*P_pred_2_2 + temp_0_3*P_pred_3_2);
	P0_0_3 =   P_pred_0_3 - (temp_0_0*P_pred_0_3 + temp_0_1*P_pred_1_3 + temp_0_2*P_pred_2_3 + temp_0_3*P_pred_3_3);  
	P0_1_0 =   P_pred_1_0 - (temp_1_0*P_pred_0_0 + temp_1_1*P_pred_1_0 + temp_1_2*P_pred_2_0 + temp_1_3*P_pred_3_0);
	P0_1_1 =   P_pred_1_1 - (temp_1_0*P_pred_0_1 + temp_1_1*P_pred_1_1 + temp_1_2*P_pred_2_1 + temp_1_3*P_pred_3_1);
	P0_1_2 =   P_pred_1_2 - (temp_1_0*P_pred_0_2 + temp_1_1*P_pred_1_2 + temp_1_2*P_pred_2_2 + temp_1_3*P_pred_3_2);
	P0_1_3 =   P_pred_1_3 - (temp_1_0*P_pred_0_3 + temp_1_1*P_pred_1_3 + temp_1_2*P_pred_2_3 + temp_1_3*P_pred_3_3);    
	P0_2_0 =   P_pred_2_0 - (temp_2_0*P_pred_0_0 + temp_2_1*P_pred_1_0 + temp_2_2*P_pred_2_0 + temp_2_3*P_pred_3_0);
	P0_2_1 =   P_pred_2_1 - (temp_2_0*P_pred_0_1 + temp_2_1*P_pred_1_1 + temp_2_2*P_pred_2_1 + temp_2_3*P_pred_3_1);
	P0_2_2 =   P_pred_2_2 - (temp_2_0*P_pred_0_2 + temp_2_1*P_pred_1_2 + temp_2_2*P_pred_2_2 + temp_2_3*P_pred_3_2);
	P0_2_3 =   P_pred_2_3 - (temp_2_0*P_pred_0_3 + temp_2_1*P_pred_1_3 + temp_2_2*P_pred_2_3 + temp_2_3*P_pred_3_3);  
	P0_3_0 =   P_pred_3_0 - (temp_3_0*P_pred_0_0 + temp_3_1*P_pred_1_0 + temp_3_2*P_pred_2_0 + temp_3_3*P_pred_3_0);
	P0_3_1 =   P_pred_3_1 - (temp_3_0*P_pred_0_1 + temp_3_1*P_pred_1_1 + temp_3_2*P_pred_2_1 + temp_3_3*P_pred_3_1);
	P0_3_2 =   P_pred_3_2 - (temp_3_0*P_pred_0_2 + temp_3_1*P_pred_1_2 + temp_3_2*P_pred_2_2 + temp_3_3*P_pred_3_2);
	P0_3_3 =   P_pred_3_3 - (temp_3_0*P_pred_0_3 + temp_3_1*P_pred_1_3 + temp_3_2*P_pred_2_3 + temp_3_3*P_pred_3_3);

    //输出角度限幅，2Π
	if(tempa_3_0>(6.2831853f))   
	{
		tempa_3_0 -= (6.2831853f);  
	}
	xD[0] = tempa_0_0;   //ialpha
	xD[1] = tempa_1_0;   //ibeta
	xD[2] = tempa_2_0;   //omiga
	xD[3] = tempa_3_0;   //theta
}
