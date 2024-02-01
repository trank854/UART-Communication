// CECS447Project2MCU1.c
// Runs on TM4C123
// Starter file for CEC447 Project 2 UART MCU1
// Min He
// March 8, 2023

// U0Rx (VCP receive) connected to PA0
// U0Tx (VCP transmit) connected to PA1

#include "PLL.h"
#include "UART0.h"
#include "UART1.h"
#include "LEDSW.h"

#define MAX_STR_LEN 20

// color definitions 
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define PURPLE 0x06sssc
#define WHITE 0x0E
#define OFF 0x00

// Defines which Bits control LEDs 
#define LED (*((volatile unsigned long *)0x40025038))

extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);
extern void DisableInterrupts(void);

int main(void){
	DisableInterrupts();
  PLL_Init();
  UART0_Init();  // for PC<->MCU1
	UART1_Init();  // for MCU1<->MCU2; Remember to uncomment this
	LEDSW_Init(); 
	EnableInterrupts();

  while(1){
		Display_Menu(); 
		switch(UART0_InChar()){
			case '1':
				Mode1();
				break;
			case '2':
				Mode2();
				break;
			case '3': 
				Mode3();
				break;
			default:
				break; 
		}
		OutCRLF0();
  }
}
