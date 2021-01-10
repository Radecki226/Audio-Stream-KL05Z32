#include "MKL05Z4.h"
#include "uart.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char rx_buf[16];
char* tx_buf;
char temp,buf;
int buf_ok = 0;
uint8_t rx_buf_pos=0;
char temp,buf;
uint8_t rx_FULL=0;
uint8_t too_long=0;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	{
		
		temp=UART0->D;	// Odczyt wartosci z bufora odbiornika i skasowanie flagi RDRF
		if(!rx_FULL)
		{
			if(temp!='\n' && temp!='\r')
			{
				if(!too_long)	// Jesli za dlugi ciag, ignoruj reszte znaków
				{
					rx_buf[rx_buf_pos] = temp;	// Kompletuj komende
					rx_buf_pos++;
					if(rx_buf_pos==16)
						too_long=1;		// Za dlugi ciag
				}
			}
			else
			{
				if(!too_long)	// Jesli za dlugi ciag, porzuc tablice
					rx_buf[rx_buf_pos] = 0;
				rx_FULL=1;
				
			}
		}
	NVIC_EnableIRQ(UART0_IRQn);
	}
}

//funkcja odbierajaca dane z uart i wyswietalajaca je na ekranie lcd
void uart_receive(){
	int flag = 0;
	while(!flag){
		if(rx_FULL){ //jesli wiadomosc gotowa
			LCD1602_ClearAll();
			LCD1602_SetCursor(0,0);
			LCD1602_Print(rx_buf);
			rx_buf_pos=0;
			rx_FULL=0;	// Dana skonsumowana
			flag = 1;
			too_long = 0;
		}
	}
	
}
//Funkcja wysylajaca komunikat i odbierajaca odpowiedz
void uart_hc_05_msg(char* string){
	DELAY(2000)
	UART_Println(string);
	uart_receive();
	DELAY(2000)
	LCD1602_ClearAll();
}
	

int main(void){

	UART_Init(38400);		// Inicjalizacja portu szeregowego UART0
	LCD1602_Init();
	LCD1602_SetCursor(0,0);

	//Ustawienie uart na 230400
	LCD1602_Print("setting uart");
	uart_hc_05_msg("AT+UART=230400,0,0");
	uart_hc_05_msg("AT+UART?");
	//Ustawienie nazwy na TMP2
	LCD1602_Print("setting name");
	uart_hc_05_msg("AT+NAME=TMP2");
	uart_hc_05_msg("AT+NAME?");
	//Ustawienie pinu na 9999
	LCD1602_Print("setting password");
	uart_hc_05_msg("AT+PSWD=\"9999\"");
	uart_hc_05_msg("AT+PSWD?");
	//zakonczenie programu
	LCD1602_Print("job finished");
	
	
	
	}