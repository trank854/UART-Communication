// CECS447Project2MCU2.c
// Starter file for CECS447 Project 2 MCU 2
// Min He
#include <stdint.h>
#include "PLL.h"
#include "UART1.h"
#include "LEDSW.h"
#include "Nokia5110.h"

// color definitions 
#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define PURPLE 0x06
#define YELLOW 0x0A
#define WHITE 0x0E
#define OFF 0x00
const	uint8_t color_wheel[] = {GREEN,RED,BLUE,YELLOW,WHITE,OFF};

// Defines what Bits control LED lights 
//#define LED (*((volatile unsigned long *)       ))

extern void EnableInterrupts(void);
extern void WaitForInterrupt(void);
extern void DisableInterrupts(void);

int main(void){
	DisableInterrupts();
  PLL_Init();
  UART1_Init();
	//Nokia5110_Init();
	LEDSW_Init(); 
	EnableInterrupts();	
	//Nokia5110_Clear();
	
  while(1){
		switch(UART1_InChar()){
			case '2': 
				Mode2();
				break;
			case '3': 
				Mode3();
				break;
			default: 
				break;
		}
  }
}
