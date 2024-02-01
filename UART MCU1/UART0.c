#include "UART0.h"
#include "tm4c123gh6pm.h"

void UART0_Init(){
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_UART0;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOA;
	
	UART0_CTL_R = 0;
	UART0_IBRD_R = 27;
	UART0_FBRD_R = 8;
	UART0_LCRH_R = (UART_LCRH_WLEN_8|UART_LCRH_FEN);
	UART0_CTL_R |= UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN;
	
	GPIO_PORTA_PCTL_R |= (GPIO_PORTA_PCTL_R&0xFFFFFF00)+0x00000011;
	GPIO_PORTA_AFSEL_R |= 0x03;
	GPIO_PORTA_AMSEL_R &= ~0x03;
	GPIO_PORTA_DEN_R |= 0x03;
}

//Display the Initial Description
void Display_Menu(){
	UART0_OutString("1 - Mode 1: PC <-> MCU1");
	OutCRLF0();
	UART0_OutString("2 - Mode 2: MCU1 <-> MCU2");
	OutCRLF0();
	UART0_OutString("3 - Mode 3: PC <-> MCU1 <-> MCU2");
	OutCRLF0();
	OutCRLF0();
	UART0_OutString("Select a Service Mode: ");
}

char UART0_In(){
	while((UART0_FR_R&UART_FR_RXFE) != 0);			//Waits for a user input
	return UART0_DR_R&0xFF;											//returns character data (refer to ASCII table)
}

char UART0_InChar(){
	unsigned int length = 0;										//User can only input one character
	char character, mode;
	character = UART0_In();											//Function that makes it wait for a user input
	while(character != CR){											
		if(character == BS && length){						//Ability to backspace as long as it's not empty
			UART0_OutChar(BS);											//Outputs character to the console
			length--;
		}
		else if(character >= '!' && character <= '~' && length < 1){	//User can input numbers 0-9 but only as a single number
			UART0_OutChar(character);
			length++;
			mode = character;												//set the mode as latest valid input
		}
		character = UART0_In();										//Change input so it doesn't infinite loop the same character
	}
	return mode;
}

//Prints out character from user input to the console
char UART0_OutChar(char character){
	while((UART0_FR_R&UART_FR_TXFF) != 0);
	UART0_DR_R = character;
	return UART0_DR_R;
}

void UART0_InString(char* str, unsigned int max){
	unsigned int length = 0;
	char character;
	character = UART0_In();
	while(character != CR){
		if(character == BS && length){
			str--;
			length--;
			UART0_OutChar(BS);
		}
		else if(character >= SP && character <= '~' && length < max){
			*str = character;
			str++;
			length++;
			UART0_OutChar(character);
		}
		character = UART0_In();
	}
	*str = 0;
}

//Prints out a string; Used for making descriptions
void UART0_OutString(char *ptr){
	while(*ptr){
		UART0_OutChar(*ptr);
		ptr++;
	}
}

//Creates a new line and sets the cursor to the beginning of the line
void OutCRLF0(){
	UART0_OutChar(CR);
	UART0_OutChar(LR);
}