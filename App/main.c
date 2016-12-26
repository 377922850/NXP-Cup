#include "common.h"
#include "include.h"

#define MOTOR_FTM   FTM0
#define Forward_PWM  FTM_CH3
#define Backward_PWM  FTM_CH4

#define MOTOR_HZ    500//Hz
#define MOTOR_DUTY  80


void PIT0_IRQHandler(void);

void main()
{

    ftm_pwm_init(MOTOR_FTM, Forward_PWM,MOTOR_HZ,20);      //��ʼ�� ��� PWM
    ftm_pwm_init(MOTOR_FTM, Backward_PWM,MOTOR_HZ,0);      //��ʼ�� ��� PWM
    
    ftm_quad_init(FTM2);									//FTM2 ���������ʼ�������õĹܽſɲ� port_cfg.h ��
	pit_init_ms(PIT0, 5); 								//��ʼ��PIT0����ʱʱ��Ϊ�� 10ms
	set_vector_handler(PIT0_VECTORn ,PIT0_IRQHandler);		//����PIT0���жϷ�����Ϊ PIT0_IRQHandler
	enable_irq (PIT0_IRQn); 								//ʹ��PIT0�ж�

    while(1)
    {
      
    }

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

    vcan_sendware((uint8_t *)&val, sizeof(val));//virtual oscilloscope


    PIT_Flag_Clear(PIT0);		//���жϱ�־λ
}


