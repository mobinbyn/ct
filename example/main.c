#include "system_define.h"
#include <stdio.h>
#include <string.h>

void usart_send(char* s);
void delay_ms(uint32_t ms);


int main(void)
{
	char buf[100];

	// Enable ADC1 and USART1 Clock
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN  | RCC_APB2ENR_USART1EN;
	
	GPIOA->CRH = GPIO_CRH_CNF10_1 | GPIO_CRH_CNF9_1 | GPIO_CRH_MODE9;
	
	//Set budrate 9600
	USART1->BRR = 833;
	USART1->CR1 = USART_CR1_UE | USART_CR1_TE;
	
	CT_Init();

	while(1)
	{
		double I_RMS = CT_CalcIrms(ADC_CT1_CH, 1480);
		sprintf (buf, "\nCurrent -> %.2f A\n", I_RMS);
		usart_send(buf);
		delay_ms(50);
	}
}



void usart_send(char* s)
{
	while(*s != '\0')
	{
		while((USART1->SR & USART_SR_TC) == 0);
		USART1->DR = *s++;
	}
}


void delay_ms(uint32_t ms)
{
	volatile uint32_t i;
	for(i = 0; i < ms * 8000; i++);
}


