#define GPIO_BASEADDR 0x40010000
//#define GPIO_PIN_SET 1
//#define GPIO_PIN_RESET 0

#define GPIO_LED 1
#define GPIO_SWITCH 15

typedef struct{
	volatile uint32_t ID;
	volatile uint32_t OP_DATA;
	volatile uint32_t IP_DATA;
	volatile uint32_t DATA_SRC;
	volatile uint32_t DIRECTION;	
	volatile uint32_t DIRECTION_SELECT;	
	volatile uint32_t INT_STATUS;	
	volatile uint32_t INT_MASK;
}GPIO_RegDef;

#define GPIO ((GPIO_RegDef *)GPIO_BASEADDR) 

typedef struct{
	volatile uint8_t GPIO_ID;
	volatile uint32_t GPIO_PinNum;
	volatile uint32_t GPIO_DataSource;
	volatile uint32_t GPIO_PinMode;
	volatile uint32_t GPIO_DirectionSelect;
}GPIO_PinConfig;


typedef struct{
	GPIO_RegDef *pGPIO;
	GPIO_PinConfig Pin_Config;
}GPIO_Config;




void GPIO_Init(GPIO_Config* Config)
{	
	Config->pGPIO->DIRECTION_SELECT |= ((Config->Pin_Config.GPIO_DirectionSelect)<< (Config->Pin_Config.GPIO_PinNum));
	Config->pGPIO->DIRECTION |= ((Config->Pin_Config.GPIO_PinMode)<< (Config->Pin_Config.GPIO_PinNum));
	Config->pGPIO->DATA_SRC |= ((Config->Pin_Config.GPIO_DataSource)<< (Config->Pin_Config.GPIO_PinNum));

}



bool GPIO_Read_from_inputpin(GPIO_RegDef *pGPIO,uint32_t PinNumber)
{
	bool value;
	value = (bool)((pGPIO->IP_DATA >> PinNumber) & 0x01);
	return value;

}



void GPIO_Toggle_outputpin(GPIO_RegDef *pGPIO,uint32_t PinNumber)
{
	pGPIO->OP_DATA = pGPIO->OP_DATA ^ (1 << PinNumber);
}



void GPIO_ButtonInit(void) {
	GPIO_Config GPIOBtn;

	memset(&GPIOBtn, 0, sizeof(GPIOBtn));
 	  
	GPIOBtn.Pin_Config.GPIO_PinNum = GPIO_SWITCH;
	GPIOBtn.Pin_Config.GPIO_DirectionSelect = 1; //DIRECTION FROM PIN
	GPIOBtn.Pin_Config.GPIO_PinMode = 1;	//INPUT MODE
	GPIOBtn.Pin_Config.GPIO_DataSource = 1; //SOURCE FROM PIN

	GPIO_Init(&GPIOBtn);
}
 
void GPIO_LedInit(void) {
	GPIO_Config GPIOLed;
	memset(&GPIOLed, 0, sizeof(GPIOLed));
 	  
	GPIOLed.Pin_Config.GPIO_PinNum = GPIO_LED;
	GPIOLed.Pin_Config.GPIO_DirectionSelect = 1; //DIRECTION FROM PIN
	GPIOLed.Pin_Config.GPIO_PinMode = 0;	//OUTPUT MODE
	GPIOLed.Pin_Config.GPIO_DataSource = 1; //SOURCE FROM PIN

	GPIO_Init(&GPIOLed);
}

int main(void)
{
	GPIO_ButtonInit();
	GPIO_LedInit();
	// while(1){
	// 		if(GPIO_Read_from_inputpin(GPIO_Config.pGPIO,GPIO_SWITCH)==1)
	// 		{
	// 			for(int i=0;i<100000;i++);
	// 			GPIO_Toggle_outputpin(GPIO_Config.pGPIO,GPIO_LED);
	// 		}
	// }
	while(1){
			if(GPIO_Read_from_inputpin(GPIO,GPIO_SWITCH)==1)
			{
				for(int i=0;i<100000;i++);
				GPIO_Toggle_outputpin(GPIO,GPIO_LED);
			}
	}

}
