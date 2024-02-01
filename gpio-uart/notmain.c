
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define IO_BANK0_BASE               0x40014000
#define IO_BANK0_GPIO0_CTRL_RW      (IO_BANK0_BASE+0x004+0x0000)
#define IO_BANK0_GPIO1_CTRL_RW      (IO_BANK0_BASE+0x00C+0x0000)

#define SECEN_BASE_SYS_CTRL	    0x5008E000
#define SECENCCLK_CTRL		    0x5008E800 
#define SECENCCLK_DIV		    0x5008E804

#define UART0_BASE                  0x50090000

#define UART0_BASE_UARTDR_RW        (UART0_BASE+0x000+0x0000)
#define UART0_BASE_UARTFR_RW        (UART0_BASE+0x018+0x0000)
#define UART0_BASE_UARTIBRD_RW      (UART0_BASE+0x024+0x0000)
#define UART0_BASE_UARTFBRD_RW      (UART0_BASE+0x028+0x0000)
#define UART0_BASE_UARTLCR_H_RW     (UART0_BASE+0x02C+0x0000)
#define UART0_BASE_UARTCR_RW        (UART0_BASE+0x030+0x0000)

#define GPIO_LED 1
#define GPIO_BASEADDR       0x40010000

#define OP_DATA             (*(volatile unsigned int *)(GPIO_BASEADDR + 0x04))
#define IP_DATA             (*(volatile unsigned int *)(GPIO_BASEADDR + 0x08))
#define DATA_SRC            (*(volatile unsigned int *)(GPIO_BASEADDR + 0x0C))
#define DIRECTION           (*(volatile unsigned int *)(GPIO_BASEADDR + 0x10))
#define DIRECTION_SELECT    (*(volatile unsigned int *)(GPIO_BASEADDR + 0x14))

void delay(){
    for(unsigned int i = 0; i<300000 ; i++);
}

static unsigned int uart_recv(void)
{ 
	while(1)
 	{
 		if((GET32(UART0_BASE_UARTFR_RW) & (1<<4))==0)
 			break;
 	}
 	return(GET32(UART0_BASE_UARTDR_RW));
 }
static void uart_send ( unsigned int x )
{
	while(1)
	{
		if((GET32(UART0_BASE_UARTFR_RW)&(1<<5))==0)
			break;
	}
 
	PUT32(UART0_BASE_UARTDR_RW,x);
}
static void hexstrings ( unsigned int d )
{
	//unsigned int ra;
	unsigned int rb;
	unsigned int rc;
	rb=32;
 	while(1)
 	{
 		rb-=4;
 		rc=(d>>rb)&0xF;
 		if(rc>9) rc+=0x37; else rc+=0x30;
 		uart_send(rc);
 		if(rb==0) break;
 	}
 	uart_send(0x20);
 }

static void hexstring ( unsigned int d )
{

	hexstrings(d);
	uart_send(0x0D);
	uart_send(0x0A);
}

/* Not used for Secure Enclave */
#if 0
static void clock_init ( void ) 
{
     PUT32(CLK_SYS_RESUS_CTRL_RW,0);
     //PUT32(CLK_REF_CTRL_RW,0); //ring oscillator
     //PUT32(CLK_SYS_CTRL_RW,0);
     PUT32(XOSC_CTRL_RW,0xAA0);      //1 - 15MHZ
     PUT32(XOSC_STARTUP_RW,47);      //straight from the datasheet
     PUT32(XOSC_CTRL_SET,0xFAB000);  //enable
    while(1)
    {
        if((GET32(XOSC_STATUS_RW)&0x80000000)!=0) break;
 
    }
    PUT32(CLK_REF_CTRL_RW,2); //XOSC
    PUT32(CLK_SYS_CTRL_RW,0); //reset/clk_ref
}
#endif

/* UART Send single character */

static void uartTx(unsigned char x)
{
    while((GET32((UART0_BASE_UARTFR_RW)) & (1 << 5)) != 0); // wait until TX FIFO is not full
    PUT32(UART0_BASE_UARTDR_RW, x);                                 // Write the TX data
}

/* UART Send character string */
static void uartTxStr( unsigned char *x )
{
    // Write the string of data until the NULL char is detected
    while( *x != '\0' )
    {
        uartTx( *x );
        *x++;
    }
}

/*	Initalization of GPIO pin as LED */ 
void GPIO_Init() {
    DIRECTION_SELECT |= (1 << GPIO_LED);
    DIRECTION &= ~(1 << GPIO_LED);  // Clear the bit for an output
    DATA_SRC |= (1 << GPIO_LED);
}
 /*	GPIO Turn on API	*/ 
void GPIO_LED_ON(){
	OP_DATA |= (1 << GPIO_LED);
}

/*	GPIO Turn off API	*/

void GPIO_LED_OFF(){
	OP_DATA &= ~(1 << GPIO_LED);
}

int notmain ( void )
{
    //unsigned int ra;
    char *welcomeMsg = "Welcome Home\n";
    //(10000000/(16/115200)) = 5.42534722222
    //0.425*64 = 27
    PUT32(UART0_BASE_UARTIBRD_RW, 5);
    PUT32(UART0_BASE_UARTFBRD_RW, 27);
    //0 11 1 0 0 0 0
    //0111 0000
    PUT32(UART0_BASE_UARTLCR_H_RW,0x70);
    PUT32(UART0_BASE_UARTCR_RW,(1<<9)|(1<<8)|(1<<0));
    GPIO_Init();

    while(1)
    {
        hexstring(uart_recv());
	uartTxStr(welcomeMsg);   
	
	delay();
	GPIO_LED_ON();
	
	delay();
	GPIO_LED_OFF();   
    }
    return(0);
}
 


