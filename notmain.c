#include <stdint.h>
#include <stdbool.h>
void PUT32 ( unsigned int, unsigned int );
unsigned int GET32 ( unsigned int );

#define GPIO_BASEADDR       0x40010000
#define OP_DATA             (GPIO_BASEADDR + 0x04)
#define IP_DATA             (GPIO_BASEADDR + 0x08)
#define DATA_SRC            (GPIO_BASEADDR + 0x0C)
#define DIRECTION           (GPIO_BASEADDR + 0x10)
#define DIRECTION_SELECT    (GPIO_BASEADDR + 0x14)

#define GPIO_PIN_SET  1
#define GPIO_PIN_RESET  0

#define INPUT	1
#define OUTPUT	0
 
#define PIN 1
#define REGISTER 0

void Dummy_sleep(int secs)
{
    #define STEPS_PER_SEC 50000
    unsigned int i,s;
    for (s=0; s < secs; s++) 
    {
        for (i=0; i < STEPS_PER_SEC; i++) 
        {
            // skip CPU cycle or any other statement(s) for making loop
            // untouched by C compiler code optimizations
            asm("nop");
        }
    }
}

void GPIO_Init(uint32_t PinNumber,bool DataSrc,bool Mode,bool DirSel) 
{	   	
    PUT32 (DIRECTION_SELECT,(DirSel << PinNumber); //direction select
    PUT32 (DIRECTION,(Mode << PinNumber)); //direction
    PUT32 (DATA_SRC,(DataSrc << PinNumber)); //data source
}

void GPIO_LED_ON()
{
	PUT32 (OP_DATA , 0x00001FC0); //output data -> all green led
}

void GPIO_LED_OFF()
{
	PUT32 (OP_DATA , 0x0000003F); //output data -> all red led
}

bool GPIO_ReadFromInputPin(uint32_t PinNumber)
{
    bool value ;
    value = ((GET32(IP_DATA)>>PinNumber)& 0x00000001 );
    return value ;
}

bool GPIO_ReadFromInputReg()
{
    uint32_t value ;
    value = GET32(IP_DATA);
    return value ;
}

void GPIO_WriteToOutputPin( uint32_t PinNumber , bool Value)
{

    if (Value == GPIO_PIN_RESET) 
    {
        // write 1 to the bit field corresponding pin number -> turns off all leds
        //uint32_t value = (GET32(OP_DATA) |(0x01 <<PinNumber));
        PUT32 (OP_DATA , (0x01 << PinNumber));
        //PUT32 (OP_DATA , value);
    }
    
    if(Value == GPIO_PIN_SET)
    {
        // write 0  to the bit field corresponding pin number -> turns on all leds        
        //uint32_t value = (GET32(OP_DATA) & (~(0x01 << PinNumber)));
        PUT32(OP_DATA , (~(0x01 << PinNumber)));
        //PUT32 (OP_DATA , value);
    }
    Dummy_sleep(1);
}

void GPIO_WriteToOutputReg(uint32_t Value ) 
{
    PUT32(OP_DATA,Value);
}

void GPIO_Toggle_outputpin(uint32_t PinNumber) 
{
    //OP_DATA ^= (1 << PinNumber);
    Dummy_sleep(1);
    uint32_t value = (GET32(OP_DATA) ^ (0x01 << PinNumber));
    PUT32 (OP_DATA , value);
    //PUT32 (OP_DATA, ((0x01 << PinNumber)));
}


int notmain ( void )
{
    GPIO_Init(14,REGISTER,INPUT,REGISTER);
    GPIO_Init(10,REGISTER,OUTPUT,REGISTER);
    GPIO_Init(2,REGISTER,OUTPUT,REGISTER);
    
    GPIO_WriteToOutputPin(10,GPIO_PIN_SET); 
   
    while (1) 
    {
        bool status = GPIO_ReadFromInputPin(14);
      	if(status==1)
	{
		GPIO_Toggle_outputpin(10);
	}
	if(status==0)
	{
		GPIO_Toggle_outputpin(2);
	}
     }
    return 0;
}

