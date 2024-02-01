#include "LEDSW.h"
#include "UART1.h"
#include "Nokia5110.h"
#include "tm4c123gh6pm.h"

#define RED 0x02
#define BLUE 0x04
#define GREEN 0x08
#define PURPLE 0x06
#define WHITE 0x0E
#define OFF 0x00

#define MAX_STR_LEN 30

#define LED (*((volatile unsigned long *)0x40025038))

char Color;
char mode;
int loop = 1;

void LEDSW_Init(void){
	unsigned volatile long delay;
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF;
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;
	GPIO_PORTF_CR_R |= 0x1F;
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_PUR_R |= 0x11;
	GPIO_PORTF_DEN_R |= 0x1F;
	
	//Interrupt Initialization
	GPIO_PORTF_IS_R &= ~0x11;									//Edge-Triggered
	GPIO_PORTF_IBE_R &= ~0x11;								//Not Both Edges
	GPIO_PORTF_IEV_R |= 0x11;									//Positive Edge
	GPIO_PORTF_ICR_R = 0x11;									//Clear 4
	NVIC_PRI7_R |= (NVIC_PRI7_R&0xFF00FFFF) | 0x00200000; //bits 23-21; priority 1
	NVIC_EN0_R |= 0x40000000;									//IRQ 30
}

void Mode2(){
	mode = '2';
	GPIO_PORTF_IM_R |= 0x11;
	Color = UART1_InChar();
	while(loop){
		if(Color == 'r' || Color == 'R'){
			LED = RED;
		}
		else if(Color == 'g' || Color == 'G'){
			LED = GREEN;
		}
		else if(Color == 'b' || Color == 'B'){
			LED = BLUE;
		}
		else if(Color == 'p' || Color == 'P'){
			LED = PURPLE;
		}
		else if(Color == 'w' || Color == 'W'){
			LED = WHITE;
		}
		else if(Color == 'd' || Color == 'D'){
			LED = OFF;
		}
	}
}

void Mode3(){
	mode = '3';
	char string[MAX_STR_LEN];
	//Wait for information from MCU1 to transfer to MCU2 via UART1
	UART1_InString(string, MAX_STR_LEN);
	//MCU Writing to LCD
	Nokia5110_Clear();
	Nokia5110_OutString(string);
	//Arm SW1 and wait
	GPIO_PORTF_IM_R |= 0x10;									//Arm PF4
	while((GPIO_PORTF_IM_R&0x10) == 0x10);
	UART1_OutString("I recieved ");
	UART1_OutChar(0x27);
	UART1_OutString(string);
	UART1_OutChar(0x27);
	UART1_OutChar(END);
}

void GPIOPortF_Handler(){
	if (mode == '2'){
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
		else if((GPIO_PORTF_RIS_R&0x01) == 0x01){
			GPIO_PORTF_IM_R &= ~0x10;
			UART1_OutChar(Color);
			loop = 0;
			GPIO_PORTF_ICR_R |= 0x01;
		}
	}
	if(mode == '3'){
		if((GPIO_PORTF_RIS_R&0x10) == 0x10){
			GPIO_PORTF_IM_R &= ~0x10;
			GPIO_PORTF_ICR_R = 0x10;
		}
	}
}