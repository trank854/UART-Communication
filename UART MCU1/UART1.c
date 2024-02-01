#include "UART1.h"
#include "tm4c123gh6pm.h"

void UART1_Init(){
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART1;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;
	
	UART1_CTL_R = 0;
	UART1_IBRD_R = 27;
	UART1_FBRD_R = 8;
	UART1_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
	
	GPIO_PORTB_AFSEL_R |= 0x03;
	GPIO_PORTB_PCTL_R |= (GPIO_PORTB_PCTL_R&0xFFFFFF00) + 0x00000011;
	GPIO_PORTB_DEN_R |= 0x03;
	GPIO_PORTB_AMSEL_R &= ~0x03;
	
	UART1_CTL_R |= UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;
}

char UART1_InChar(){
  while((UART1_FR_R&UART_FR_RXFE) != 0); // wait until the receiving FIFO is not empty
  return((unsigned char)(UART1_DR_R&0xFF));
}

char UART1_OutChar(char character){
	while((UART1_FR_R&UART_FR_TXFF) != 0);
	UART1_DR_R = character;
	return UART1_DR_R;
}

void UART1_InString(char* str, unsigned int max){
	unsigned int length = 0;
	char character;
	character = UART1_InChar();
	while(character != END){
		*str = character;
		str++;
		length++;
		character = UART1_InChar();
		}
	*str = 0;
}

void UART1_OutString(char *ptr){
	while(*ptr){
		UART1_OutChar(*ptr);
		ptr++;
	}
	UART1_OutChar(END);
}