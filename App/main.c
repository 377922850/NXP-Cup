#include "common.h"
#include "include.h"

void uart4_handler(void)
{
    char ch;
    int P_Integer, I_Integer, D_Integer;
    if(uart_query    (UART4) == 1)  
    {
        uart_getchar   (UART4, &ch); 
		if(ch=='1')
			sptr->Proportion+=0.05;
		else if(ch=='2')
			sptr->Proportion-=0.05;
		else if(ch=='4')
			sptr->Integral+=0.1;
		else if(ch=='5')
			sptr->Integral-=0.1;
		else if(ch=='7')
			sptr->Derivative+=0.1;
		else if(ch=='8')
			sptr->Derivative-=0.1;
        
       P_Integer =(int) sptr->Proportion;//��������
       I_Integer =( int) sptr->Integral;//��������
       D_Integer =( int) sptr->Derivative;//��������
       printf("\t%d.%03d",(int) sptr->Proportion,(int)((sptr->Proportion-(int) sptr->Proportion)*1000));
       printf("\t%d.%04d",( int) sptr->Integral,(int)((sptr->Integral-( int) sptr->Integral)*10000));
       printf("\t%d.%04d",( int) sptr->Derivative,(int)((sptr->Derivative-( int) sptr->Derivative)*10000));
       printf("\n");
       
    //sptr->SetPoint =100;    //Ŀ���� 100 

                          
    }
}

void main()
{
    int i;
    set_vector_handler(UART4_RX_TX_VECTORn,uart4_handler);
	uart_rx_irq_en (UART4);
    
    /************************ LCD Һ���� ��ʼ��  ***********************/
    LCD_init();
    
    flash_init();
    
    adc_init(AMP1);
    adc_init(AMP2);
    adc_init(AMP3);
    adc_init(AMP4);
    adc_init(AMP5);
    adc_init(AMP6);
    
    //led light init
    led_init(LED_MAX);
    
    //��ʼ�� ȫ�� ����
    key_init(KEY_MAX);
    
    //����ʱ5sec
    for(i=5;i>0;i--)
    {
//        Display(i,16,2);
        led_turn(LED3);
        DELAY_MS(1000);
        if(key_check(KEY_L) == KEY_DOWN)//KEY_B
        {
            break;
        }
    }
    
    //�궨
    calibrate_max_min();

    motor_init();
    
    while(1)
    {
        deal_key_event();
    }
}


