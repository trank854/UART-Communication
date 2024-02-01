#include "LEDSW.h"
#include "UART0.h"
#include "UART1.h"
#include "tm4c123gh6pm.h"

#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define PURPLE 0x06
#define WHITE 0x0E
#define OFF 0x00

#define MAX_STR_LEN 30

#define LED (*((volatile unsigned long *)0x40025038))

char Color = 'd';
char mode;

void LEDSW_Init(void){
	unsigned volatile long delay;
	
	//Port Initialization
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;			//Enable PortF Clock
	delay = SYSCTL_RCGC2_R;										//Stabilize Clock
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;				//Unlock PF0
	GPIO_PORTF_CR_R |= 0x1F;
	GPIO_PORTF_DIR_R |= 0x0E;									//Output PF1-3
	GPIO_PORTF_PUR_R |= 0x11;									//Pull-up Resistor PF0 & PF4
	GPIO_PORTF_DEN_R |= 0x1F;									//Enable PF0-4
	
	//Interrupt Initialization
	GPIO_PORTF_IS_R &= ~0x11;									//Edge-Triggered
	GPIO_PORTF_IBE_R &= ~0x11;								//Not Both Edges
	GPIO_PORTF_IEV_R |= 0x11;									//Positive Edge
	GPIO_PORTF_ICR_R = 0x11;									//Clear 0 & 4
	NVIC_PRI7_R |= (NVIC_PRI7_R&0xFF00FFFF) | 0x00200000; //bits 23-21; priority 1
	NVIC_EN0_R |= 0x40000000;									//IRQ 30
}

void Mode1(){
	
	//Description
	OutCRLF0();
	OutCRLF0();
	UART0_OutString("R - Red");
	OutCRLF0();
	UART0_OutString("G - Green");
	OutCRLF0();
	UART0_OutString("B - Blue");
	OutCRLF0();
	UART0_OutString("P - Purple");
	OutCRLF0();
	UART0_OutString("W - White");
	OutCRLF0();
	UART0_OutString("D - Dark");
	OutCRLF0();
	OutCRLF0();
	UART0_OutString("Enter a Color for the LED: ");
	
	
	//LED Color dependent on user input
	Color = UART0_InChar();
	OutCRLF0();
	if(Color == 'r' || Color == 'R'){
		LED = RED;
		UART0_OutString("Red LED is on");
	}
	else if(Color == 'g' || Color == 'G'){
		LED = GREEN;
		UART0_OutString("Green LED is on");
	}
	else if(Color == 'b' || Color == 'B'){
		LED = BLUE;
		UART0_OutString("Blue LED is on");
	}
	else if(Color == 'p' || Color == 'P'){
		LED = PURPLE;
		UART0_OutString("Red and Blue LEDs are on");
	}
	else if(Color == 'w' || Color == 'W'){
		LED = WHITE;
		UART0_OutString("All LEDs are on");
	}
	else if(Color == 'd' || Color == 'D'){
		LED = OFF;
		UART0_OutString("All LEDs are off");
	}
	
	//Start at the beginning of the new line
	OutCRLF0();
	OutCRLF0();
}

void Mode2(){
	//r > g > b > p > w > d
	mode = '2';
	UART1_OutChar(mode);
	LED = OFF;
	GPIO_PORTF_IM_R |= 0x11;
	Color = UART1_InChar();
	//Copy from MCU2
	if(Color == 'r'){
	}
	else if(Color == 'g'){
	}
	else if(Color == 'b'){
	}
	else if(Color == 'p'){
	}
	else if (Color == 'w'){
	}
	else if (Color == 'd'){
	}
}

void Mode3(){
	char mode = '3';
	char string[MAX_STR_LEN];
	char string2[MAX_STR_LEN];
	
	UART1_OutChar(mode);
	
	LED = GREEN;
	
	OutCRLF0();
	OutCRLF0();
	UART0_OutString("Enter a Message: ");
	UART0_InString(string, MAX_STR_LEN);
	UART1_OutString(string);
	OutCRLF0();
	UART0_OutString("Waiting for Acknowledgement...");
	
	while((UART1_FR_R&UART_FR_RXFE) != 0);

	OutCRLF0();
	OutCRLF0();
	UART1_InString(string2, MAX_STR_LEN);
	LED = OFF;
	UART0_OutString(string2);
	OutCRLF0();
	OutCRLF0();
}

void GPIOPortF_Handler(){
	if(mode == '2'){
	//SW1
		if((GPIO_PORTF_RIS_R&0x10) == 0x10){
			if(LED == RED){
				LED = GREEN;
				Color = 'g';
			}
			else if(LED == GREEN){
				LED = BLUE;
				Color = 'b';
			}
			else if(LED == BLUE){
				LED = PURPLE;
				Color = 'p';
			}
			else if(LED == PURPLE){
				LED = WHITE;
				Color = 'w';
			}
			else if(LED == WHITE){
				LED = OFF;
				Color = 'd';
			}
			else if(LED == OFF){
				LED = RED;
				Color = 'r';
			}
			//for(int x = 0; x<655536; x++);
			GPIO_PORTF_ICR_R |= 0x10;
		}
		//SW2
		 else if((GPIO_PORTF_RIS_R&0x01) == 0x01){
			UART1_OutChar(mode);
			UART1_OutChar(Color);
			GPIO_PORTF_IM_R &= ~0x10;
			GPIO_PORTF_ICR_R |= 0x01;
		}
	}
}