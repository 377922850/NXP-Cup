#include "common.h"
#include "include.h"
#include "motor.h"


//***************************************************** 
//���� PID ʵ�� 
//***************************************************** 
PID sPID;
PID *sptr = &sPID;
int16 PWMOUT = 0;
int32 P_Integer = 0, I_Integer = 0, D_Integer = 0;
char ServoMid = 47;

void motor_init(void)
{
    //motor init
    ftm_pwm_init(MOTOR_FTM, Forward_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, Backward_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    
    //servo init
    ftm_pwm_init(SERVO_FTM, SERVO_CH,SERVO_HZ,ServoMid);//PWM:0.15~0.75
    DELAY_MS(100);
    
    //PIDInit
    IncPIDInit();
    
    /*********************** ������Ϣ ��ʼ��  ***********************/
    P_Integer = ((int) sptr->Proportion)*1000 + (int)((sptr->Proportion-(int) sptr->Proportion)*1000);
    I_Integer = ((int) sptr->Integral)*1000 + (int)((sptr->Integral-(int) sptr->Integral)*1000);
    D_Integer = ((int) sptr->Derivative)*1000 + (int)((sptr->Derivative-(int) sptr->Derivative)*1000);
    
    key_event_init();                                                   //������Ϣ��ʼ��
    
    //encoder init
    ftm_quad_init(FTM2);									//FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
	
    
    pit_init_ms(PIT0, 10); 								//��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);		//����PIT0���жϷ�����Ϊ PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 								//ʹ��PIT0�ж�
}

/*!
 *	@brief		PIT0�жϷ�����
 *	@since		v5.0
 */
void PIT0_IRQHandler(void)
{
    key_IRQHandler();  
    int16 val;
    val = ftm_quad_get(FTM2);		   //��ȡFTM �������� ��������(������ʾ������)
    ftm_quad_clean(FTM2);
    
    ServoAngle(Servo_PD(get_bias()));
    
    PWMOUT += IncPIDCalc(val);
    
    if(PWMOUT>99||PWMOUT<-99) 
    {
        PWMOUT=PWMOUT>0?99:-99;
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
    
   // printf("%d\n",val);

//    vcan_sendware((uint8_t *)&val, sizeof(val));//virtual oscilloscope
/*   
    printf("\t%d.%03d",(int) sptr->Proportion,(int)((sptr->Proportion-(int) sptr->Proportion)*1000));
    printf("\t%d.%04d",( int) sptr->Integral,(int)((sptr->Integral-( int) sptr->Integral)*10000));
    printf("\t%d.%04d",( int) sptr->Derivative,(int)((sptr->Derivative-( int) sptr->Derivative)*10000));
    printf("\n");
*/
    PIT_Flag_Clear(PIT0);		//���жϱ�־λ
}

//***************************************************** 
//PID ������ʼ�� 
//***************************************************** 
void IncPIDInit(void)
{   
    int i=0, a[3]={0};
    sptr->LastError = 0; //Error[-1]   
    sptr->PrevError = 0; //Error[-2]   
#if 1
    sptr->Proportion = P_DATA; //��������  Proportional Const   
    sptr->Integral = I_DATA; //���ֳ��� Integral Const   
    sptr->Derivative = D_DATA; //΢�ֳ���  Derivative Const   
    sptr->SetPoint =100;    //Ŀ���� 100
#else
    for(i=0;i<3;i++)
        a[i]=flash_read(SECTOR_NUM2_MOTOR_PID,(i*4),int32);
    sptr->Proportion = ((float)a[0])/1000; //��������  Proportional Const   
    sptr->Integral = ((float)a[1])/1000; //���ֳ��� Integral Const   
    sptr->Derivative = ((float)a[2])/1000; //΢�ֳ���  Derivative Const   
    sptr->SetPoint =flash_read(SECTOR_NUM1_SPEED,(0*4),int);    //Ŀ���� 100
    
#endif
}

//***************************************************** 
//����ʽ PID �������   
//***************************************************** 
int16 IncPIDCalc(int16 NextPoint)
{   
    int16 iError, iIncpid; //��ǰ���   
    iError = sptr->SetPoint - NextPoint; //��������   

/*    //��Uk=A*e(k)+B*e(k-1)+C*e(k-2) 
    iIncpid = sptr->Proportion * iError //E[k]��   
            - sptr->Integral * sptr->LastError //E[k��1]��   
            + sptr->Derivative * sptr->PrevError; //E[k��2]��  
*/
    
// PID original function
    iIncpid = sptr->Proportion * (iError - sptr->LastError) //E[k]��   
            + sptr->Integral * iError //E[k��1]��   
            + sptr->Derivative * (iError - 2 * sptr->LastError + sptr->PrevError); //E[k��2]��   
    sptr->PrevError = sptr->LastError;      //�洢�������´μ���   
    sptr->LastError = iError;   
    return(iIncpid);                        //��������ֵ   
} 

//����ǶȺ���
void ServoAngle(int output)
{
    int Angle = 0;
    
    Angle = ServoMid - output;
    
    if (Angle < 15)
        Angle = 15;
    else if (Angle > 75)
        Angle = 75;
    
     printf("%d\n",Angle);
    ftm_pwm_duty(SERVO_FTM, SERVO_CH,Angle);
}
