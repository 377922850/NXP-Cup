#include "common.h"
#include "include.h"
#include "motor.h"


//***************************************************** 
//���� PID ʵ�� 
//***************************************************** 
static PID sPID;   
static PID *sptr = &sPID;
int16 PWMOUT = 0;


void motor_init(void)
{
    //motor init
    ftm_pwm_init(MOTOR_FTM, Forward_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, Backward_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    
    //PIDInit
    IncPIDInit();
    
    //encoder init
    ftm_quad_init(FTM2);									//FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
	pit_init_ms(PIT0, 5); 								//��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);		//����PIT0���жϷ�����Ϊ PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 								//ʹ��PIT0�ж�
}

/*!
 *	@brief		PIT0�жϷ�����
 *	@since		v5.0
 */
void PIT0_IRQHandler(void)
{
    int16 val;
    val = ftm_quad_get(FTM2);		   //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM2);
    
    PWMOUT += IncPIDCalc(val);
    
    if(PWMOUT>50||PWMOUT<-50) 
    {
        PWMOUT=PWMOUT>0?50:-50;
    }
    
    if(PWMOUT > 0)
    {
        ftm_pwm_duty(MOTOR_FTM,Forward_PWM,PWMOUT);		 
        ftm_pwm_duty(MOTOR_FTM,Backward_PWM,0);
    }
	else
    {
    	ftm_pwm_duty(MOTOR_FTM,Forward_PWM,0); 
        ftm_pwm_duty(MOTOR_FTM,Backward_PWM,-PWMOUT);
    }
    

    vcan_sendware((uint8_t *)&val, sizeof(val));//virtual oscilloscope

    PIT_Flag_Clear(PIT0);		//���жϱ�־λ
}

//***************************************************** 
//PID ������ʼ�� 
//***************************************************** 
void IncPIDInit(void)
{   
    sptr->LastError = 0; //Error[-1]   
    sptr->PrevError = 0; //Error[-2]   
    sptr->Proportion = P_DATA; //��������  Proportional Const   
    sptr->Integral = I_DATA; //���ֳ��� Integral Const   
    sptr->Derivative = D_DATA; //΢�ֳ���  Derivative Const   
    sptr->SetPoint =100;    //Ŀ���� 100 
} 

//***************************************************** 
//����ʽ PID �������   
//***************************************************** 
int16 IncPIDCalc(int16 NextPoint)
{   
    int16 iError, iIncpid; //��ǰ���   
    iError = sptr->SetPoint - NextPoint; //��������   
    iIncpid = sptr->Proportion * iError //E[k]��   
            - sptr->Integral * sptr->LastError //E[k��1]��   
            + sptr->Derivative * sptr->PrevError; //E[k��2]��   
    sptr->PrevError = sptr->LastError;      //�洢�������´μ���   
    sptr->LastError = iError;   
    return(iIncpid);                        //��������ֵ   
} 
