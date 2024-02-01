//Sets up communication between MCU1 and Serial Port of the PC

#define BS 0x08
#define CR 0x0D
#define LR 0x0A
#define SP 0x20
#define DEL 0x7F
#define NULL 0x00

//UART0 Initialization
void UART0_Init(void);

//Display Menu on PC Serial Port console
void Display_Menu(void);

char UART0_In(void);

char UART0_InChar(void);

char UART0_OutChar(char ascii);

void UART0_InString(char* str, unsigned int max);

void UART0_OutString(char *ptr);

//Move to the beginning of new line
void OutCRLF0(void);