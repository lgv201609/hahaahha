#include <bsp_gpio.h>

void  BSP_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
    
    //IMU ģ��IIC
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;   
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    
    //LED   
    //PG1
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_SetBits(GPIOG, GPIO_Pin_1);     //�ر�
    
    //TIM5 OC4  OC3
    //PI0  PH12
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOI, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_12;
    GPIO_Init(GPIOH, &GPIO_InitStructure);
    
    
    GPIO_PinAFConfig(GPIOI,GPIO_PinSource0, GPIO_AF_TIM5);
    GPIO_PinAFConfig(GPIOH,GPIO_PinSource12,GPIO_AF_TIM5);
    
    //USART1 
    //RX:PB7
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7, GPIO_AF_USART1); 
    
    //USART6
    //TX:PG14 RX:PG9
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_9 | GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource9, GPIO_AF_USART6); 
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource14,GPIO_AF_USART6);
    
    //CAN1
    //H:PD1 L:PD0
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_0 | GPIO_Pin_1; 
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_CAN1);
    
    //CAN2
    //H:PB13 L:PB12
    GPIO_InitStructure.GPIO_Pin    =  GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode   =  GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType  =  GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd   =  GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed  =  GPIO_Speed_100MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_CAN2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_CAN2);
    
}
