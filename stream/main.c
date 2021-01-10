#include "MKL05Z4.h"
#include "ADC.h"
#include "uart.h"
#include "pit.h"
#include "frdm_bsp.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//Handshake
uint8_t pit_ok=0;
//Zmienna tymczasowa przechowujaca dane z przetwornika
uint16_t temp=0;
//
uint8_t wynik_int = 0;
float	wynik_float;
uint8_t wynik;
int kal_error;
char rx_buf[] = {0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20};

void ADC0_IRQHandler()
{	
	temp = ADC0->R[0];	// Odczyt danej i skasowanie flagi COCO

}
void PIT_IRQHandler(){

	if (!pit_ok){
		
		wynik = (temp >> 4);
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		NVIC_ClearPendingIRQ(PIT_IRQn);

		pit_ok = 1;
		
		
	}		
}
int main (void){
	kal_error  = ADC_Init();
	PIT_Init();
	UART_Init(230400); //uwaga baud_rate > ok.12tys
	
	ADC0->SC1[0] = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(12);
	
	while(1){
		if(pit_ok){
			

			uart_send(wynik);
			pit_ok = 0;	
		}
			
	}
	
}