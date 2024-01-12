#define GPIO_LED 1
#define GPIO_BASEADDR 		0x40010000
#define OP_DATA				(GPIO_BASEADDR + 0x04)
#define IP_DATA				(GPIO_BASEADDR + 0x08)
#define DATA_SRC			(GPIO_BASEADDR + 0x0C)
#define DIRECTION			(GPIO_BASEADDR + 0x10)
#define DIRECTION_SELECT	(GPIO_BASEADDR + 0x14)

void GPIO_Init()
{
	DIRECTION_SELECT |= (1<<GPIO_LED);
	DIRECTION |= (0<<GPIO_LED);
	DATA_SRC |= (1<<GPIO_LED);
}


void GPIO_Toggle_outputpin()
{
	OP_DATA ^=(1<<GPIO_LED);
}


int main(void)
{
	GPIO_Init();
	while(1)
	{
		for(int i=0;i<100000;i++);
		GPIO_Toggle_outputpin();
	}
}