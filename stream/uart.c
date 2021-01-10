
#include "MKL05Z4.h"
#include "uart.h"
#define CLOCK_SETUP 1 



void UART_Init (uint32_t baud_rate) {
	
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;							//UART0 dolaczony do zegara
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;							//Port B dolaczony do zegara
	SIM->SOPT2 |= SIM_SOPT2_UART0SRC(1);		//Zegar ~48MHz
	PORTB->PCR[1] = PORT_PCR_MUX(2);								//PTB1=TX_D
	PORTB->PCR[2] = PORT_PCR_MUX(2);								//PTB2=RX_D
	
	UART0->C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );		//Blokada nadajnika i o dbiornika
	
	//Dla duzych baud rate nalezy ustawic OSR na 15
	if (baud_rate > 12000){
		UART0->BDH = 0;
		UART0->BDL = (uint8_t)(SystemCoreClock/(16*baud_rate));		//Obliczenie odpowiedniego BDL
		UART0->C4 &= ~UART0_C4_OSR_MASK;
		UART0->C4 |= UART0_C4_OSR(15);	
	}
	//Dla malych wartosci OSR = 31
	else{
		UART0->BDH = 0;
		UART0->BDL = (uint8_t)(SystemCoreClock/(32*baud_rate));		//Obliczenie odpowiedniego BDL
		UART0->C4 &= ~UART0_C4_OSR_MASK;
		UART0->C4 |= UART0_C4_OSR(31);	
	}
	
	UART0->C5 |= UART0_C5_BOTHEDGE_MASK;	//Próbkowanie odbiornika na obydwu zboczach zegara
	UART0->C2 |= UART0_C2_RIE_MASK;		// Wlacz przerwania od odbiornika
	UART0->C2 |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK);		//Wlacz nadajnik i odbiornik
	NVIC_EnableIRQ(UART0_IRQn);
	NVIC_ClearPendingIRQ(UART0_IRQn);
}

void UART_Print(const char *str) {
	
	uint8_t str_len = 0;

  while (str[str_len] != '\0') {             
    uart_send(str[str_len]);
    ++str_len;
  }
}

void UART_Println(const char *str) {
	
	UART_Print(str);
	UART_Print("\r\n");
}

void uart_send(uint8_t data) {
	while(!(UART0->S1 & UART0_S1_TDRE_MASK));
	UART0->D = data;
}