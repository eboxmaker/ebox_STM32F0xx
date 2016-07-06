/*
file   : core.cpp
author : shentq
version: V1.1
date   : 2015/7/5
date   : 2016/7/5 LQM��ֲ��STM32F0ƽ̨,ʹ��HAL��LL��
Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "common.h"
// #define systick_no_interrupt()  SysTick->CTRL &=0xfffffffd
// #define systick_interrupt()     SysTick->CTRL |=0x0002
#define systick_over_flow_value 9000//��ֵȡֵ��Χ��100-9000������ƵΪ72Mhz�������
extern "C" {

	extern uint16_t  AD_value[];


	__IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
	__IO uint32_t cpu_calculate_per_sec;//cpu��������ֵ


	void ebox_init(void)
	{
//		SysTick_Config(systick_over_flow_value);//  ÿ�� (nhz/9,000,000)s����һ���ж�
		SysTick_Config(SystemCoreClock/1000);
//		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//9Mhz��systemticks clock��
		//ͳ��cpu��������//////////////////
		cpu_calculate_per_sec = 0;
		millis_seconds = 0;
		do
		{
			cpu_calculate_per_sec++;//ͳ��cpu��������
		}
		while (millis_seconds < 100);
		cpu_calculate_per_sec = cpu_calculate_per_sec * 10;
//		////////////////////////////////
		ADC1_init();

//		NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

//		//��pb4Ĭ������ΪIO�ڣ�����jtag
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
		sys.get_chip_info();
		set_systick_user_event_per_sec(1000);
//		random_seed(AD_value[0]);//��ʼ�����������

	}

	uint32_t get_cpu_calculate_per_sec()
	{
		return cpu_calculate_per_sec;
	}
	
	uint64_t micros(void)
	{
		if ((SysTick->CTRL & (1 << 16)) && (__get_PRIMASK())) //�����ʱ�����������ж��ҷ�����systick�������Ҫ��millis_secend���в���
		{
			millis_seconds++;
		}
		return  millis_seconds * 1000 + (9000 - SysTick->VAL / 9);
	}
	
	uint64_t millis( void )
	{
		return micros() / 1000;
	}

	void delay_ms(uint64_t ms)
	{
		uint64_t end ;
		end = micros() + ms * 1000 - 3;
		while (micros() < end);
	}
	
	void delay_us(uint64_t us)
	{
		uint64_t end = micros() + us - 3;
		while (micros() < end);
	}


	callback_fun_type systick_cb_table[1] = {0};
	__IO uint16_t systick_user_event_per_sec;//��ʵ��ֵ
	__IO uint16_t _systick_user_event_per_sec;//���ڱ�millis_secondȡ����

	void set_systick_user_event_per_sec(uint16_t frq)
	{
		_systick_user_event_per_sec = 1000 / frq;
		systick_user_event_per_sec = frq;
	}

	void attach_systick_user_event(void (*callback_fun)(void))
	{
		systick_cb_table[0] = callback_fun;
	}
	
	void SysTick_Handler(void)//systick�ж�
	{
		millis_seconds++;
		if ((millis_seconds % _systick_user_event_per_sec) == 0)
		{
			if (systick_cb_table[0] != 0)
			{
				systick_cb_table[0]();
			}
		}

	}
}