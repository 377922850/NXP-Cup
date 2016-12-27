#ifndef _MOTOR_H_
#define _MOTOR_H_



//***************************************************** 
//���� PID �ṹ�� 
//***************************************************** 
typedef struct   
{   
    int SetPoint; //�趨Ŀ��  Desired Value   
    double Proportion; //��������  Proportional Const   
    double Integral; //���ֳ���  Integral Const   
    double Derivative; //΢�ֳ���  Derivative Const   
    int LastError; //Error[-1]   
    int PrevError; //Error[-2]   
} PID;

//�����ض���
#define MOTOR_FTM   FTM0
#define Forward_PWM  FTM_CH3
#define Backward_PWM  FTM_CH4

#define MOTOR_HZ    500//Hz
#define MOTOR_DUTY  80

//***************************************************** 
//PID������غ� 
//***************************************************** 
#define P_DATA 100 
#define I_DATA 0.6 
#define D_DATA 1 

void PIT0_IRQHandler(void);
void IncPIDInit(void);
int16 IncPIDCalc(int16 NextPoint);

extern void motor_init(void);

#endif /* _MOTOR_H_ */