#ifndef _MOTOR_H_
#define _MOTOR_H_



//***************************************************** 
//���� PID �ṹ�� 
//***************************************************** 
typedef struct   
{   
    int SetPoint; //�趨Ŀ��  Desired Value   
    float Proportion; //��������  Proportional Const   
    float Integral; //���ֳ���  Integral Const   
    float Derivative; //΢�ֳ���  Derivative Const   
    int LastError; //Error[-1]   
    int PrevError; //Error[-2]   
} PID;

//�����ض���
#define MOTOR_FTM   FTM0
#define Forward_PWM  FTM_CH3
#define Backward_PWM  FTM_CH4

#define MOTOR_HZ    500//Hz

//flash
#define SECTOR_NUM1_SPEED  (FLASH_SECTOR_NUM-1)
#define SECTOR_NUM2_MOTOR_PID  (FLASH_SECTOR_NUM-2)


//***************************************************** 
//PID������غ� 
//***************************************************** 
#define P_DATA 0.4//0.19
#define I_DATA 0.11//0.015
#define D_DATA 0.12//0.2

extern PID sPID; 
extern PID *sptr;
extern int32 P_Integer, I_Integer, D_Integer;



void PIT0_IRQHandler(void);
void IncPIDInit(void);
int16 IncPIDCalc(int16 NextPoint);

extern void motor_init(void);

#endif /* _MOTOR_H_ */