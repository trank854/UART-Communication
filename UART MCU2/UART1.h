#define BS 0x08
#define CR 0x0D
#define LR 0x0A
#define SP 0x20
#define DEL 0x7F
#define END 0x03

void UART1_Init(void);

char UART1_InChar(void);

char UART1_OutChar(char ascii);

void UART1_InString(char* str, unsigned int max);

void UART1_OutString(char* ptr);