#include<stm32f10x.h>
int sample=0,percent=0;
void delay(unsigned int a)
{
	while(a--);
}
void enable()
{
	GPIOC->ODR|=1<<14;
	delay(10000);
	GPIOC->BRR=1<<14;
	delay(10000);
}
void lcd(char a,char data)
{
	GPIOC->ODR=(a<<15);
	GPIOA->ODR=(data & 0XF0)>>4;
	enable();
	GPIOA->ODR=(data & 0X0F);
	enable();
}
void string(char *str)
{
	while(*str)
	{
		lcd(1,*(str++));
	}
}
void adc()
{
	
	ADC1->CR2=1<<0;
	while(!(ADC1->SR & 1<<1));
	sample=ADC1->DR;
	ADC1->SR=0<<1;
}

void Tx(char *data)
{
	while(*data)
	{
		USART1->DR=*(data++);
		while(!(USART1->SR & 1<<7));
		delay(50000);
	}  
}
void Tx_fuel(int data)
{
    USART1->DR = 'F';      
    while(!(USART1->SR & 1<<7));

    USART1->DR = (data/100) + 48;       
    while(!(USART1->SR & 1<<7));

    USART1->DR = ((data/10)%10) + 48;    
    while(!(USART1->SR & 1<<7));

    USART1->DR = (data%10) + 48;          
    while(!(USART1->SR & 1<<7));
}


int main()
{
	RCC->CR=0X83;
	RCC->CFGR=0X00;
	RCC->APB2ENR=1<<0|1<<2|1<<3|1<<4|1<<9|1<<14;
	GPIOA->CRL=0X00002222;
	GPIOA->CRH=0X200000A0;
	GPIOC->CRH=0X22000000;
	GPIOB->CRL=0X88000000;
	GPIOB->CRH=0X00000088;
	
	ADC1->CR2=1<<0|1<<1|1<<2|1<<22|1<<17|1<<18|1<<19;
	ADC1->SMPR2=0X00;
	ADC1->SQR1=0X00;
	ADC1->SQR3=0X04;
	
	USART1->CR1=1<<3|1<<13;
	USART1->BRR=0X341;
	
	lcd(0,0x02);
	lcd(0,0x28);
	lcd(0,0x0C);
	
	while(1)
	{
		//lcd(0,0x80);
		//string("d");

		adc();
		
		percent=(sample*100)/4095;
		
		
		lcd(0,0xC0);
		string("FUEL LVL:");
		lcd(1,(percent/100)+48);
		lcd(1,((percent%100)/10)+48);
		lcd(1,((percent%100)%10)+48);
		string("%   ");
		
		
		Tx_fuel(percent);
		
		if(percent<=20)
		{
			GPIOA->ODR=1<<15;     //fuel buzzer
			delay(10000);
			GPIOA->BRR=1<<15;
			delay(10000);
		}
		
		
		
		if((!(GPIOB->IDR & 1<<9))
		 &&(!(GPIOB->IDR & 1<<8))     //DOOR 1
		 &&(!(GPIOB->IDR & 1<<7))
		 &&((GPIOB->IDR & 1<<6)))
		{
			lcd(0,0x80);
			string("DOOR 1 OPEN     ");
			
			Tx("DDOOR 1 OPEN     #");
		}
		
		else if((!(GPIOB->IDR & 1<<9))
					&&(!(GPIOB->IDR & 1<<8))
					&&((GPIOB->IDR & 1<<7))        //DOOR 2
					&&(!(GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 2 OPEN     ");
			
			Tx("DDOOR 2 OPEN     #");
		}
		
		else if((!(GPIOB->IDR & 1<<9))
					&&((GPIOB->IDR & 1<<8))
					&&(!(GPIOB->IDR & 1<<7))     //DOOR 3
					&&(!(GPIOB->IDR & 1<<6)))
		{
			lcd(0,0x80);
			string("DOOR 3 OPEN     ");
			
			Tx("DDOOR 3 OPEN     #");
		}
		
		else if(((GPIOB->IDR & 1<<9))
					&&(!(GPIOB->IDR & 1<<8))
					&&(!(GPIOB->IDR & 1<<7))      //DOOR 4
					&&(!(GPIOB->IDR & 1<<6)))
		{
			lcd(0,0x80);
			string("DOOR 4 OPEN     ");
			
			Tx("DDOOR 4 OPEN     #");
		}
		
		
		else if((!(GPIOB->IDR & 1<<9))
			    &&(!(GPIOB->IDR & 1<<8))     //DOOR 1&2
			    &&((GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{
			lcd(0,0x80);
			string("DOOR 1-2 OPEN   ");
			
			Tx("DDOOR 1-2 OPEN     #");
		}
		else if((!(GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 1&3
			    &&(!(GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 1-3 OPEN   ");
			
			Tx("DDOOR 1-3 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&(!(GPIOB->IDR & 1<<8))     //DOOR 1&4
			    &&(!(GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{

			lcd(0,0x80);
			string("DOOR 1-4 OPEN   ");
			
			Tx("DDOOR 1-4 OPEN     #");
		}
		else if((!(GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 2&3
			    &&((GPIOB->IDR & 1<<7))
			    &&(!(GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 2-3 OPEN   ");
			
			Tx("DDOOR 2-3 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&(!(GPIOB->IDR & 1<<8))     //DOOR 2&4
			    &&((GPIOB->IDR & 1<<7))
			    &&(!(GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 2-4 OPEN   ");
			
			Tx("DDOOR 2-4 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 3&4
			    &&(!(GPIOB->IDR & 1<<7))
			    &&(!(GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 3-4 OPEN   ");
			
			Tx("DDOOR 3-4 OPEN     #");
		}
		
		
		else if((!(GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 1&2&3
			    &&((GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 1-2-3 OPEN ");
			
			Tx("DDOOR 1-2-3 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&(!(GPIOB->IDR & 1<<8))     //DOOR 1&2&4
			    &&((GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 1-2-4 OPEN ");
			
			Tx("DDOOR 1-2-4 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 1&3&4
			    &&(!(GPIOB->IDR & 1<<7))
			    &&((GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("DOOR 1-3-4 OPEN  ");
			
			Tx("DDOOR 1-3-4 OPEN     #");
		}
		else if(((GPIOB->IDR & 1<<9))
			    &&((GPIOB->IDR & 1<<8))     //DOOR 2&3&4
			    &&((GPIOB->IDR & 1<<7))
			    &&(!(GPIOB->IDR & 1<<6)))
		{
		
			lcd(0,0x80);
			string("DOOR 2-3-4 OPEN  ");
			
			Tx("DDOOR 2-3-4 OPEN     #");
		}
		
		
		else if((!(GPIOB->IDR & 1<<9))
					&&(!(GPIOB->IDR & 1<<8))
					&&(!(GPIOB->IDR & 1<<7))     //ALL DOORS
					&&(!(GPIOB->IDR & 1<<6)))
		{
			
			lcd(0,0x80);
			string("ALL DOORS OPENED ");
			
			Tx("DALL DOORS OPENED #");
		}
		
		else 
		{
			
			lcd(0,0x80);
			string("CAR CLOSED       ");
			
			Tx("DCAR CLOSED       #");
		}
			
		
	}
	
		
	return 0;
}
	