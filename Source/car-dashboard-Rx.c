#include<stm32f10x.h>
char data;
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
char Rx()
{
	while(!(USART1->SR & 1<<5));
	data= USART1->DR;
  return data;
}

int main()
{
	RCC->CR=0X83;
	RCC->CFGR=0X00;
	RCC->APB2ENR=1<<0|1<<2|1<<4|1<<14;
	GPIOA->CRL=0X00002222;
	GPIOA->CRH=0X000008A0;
	GPIOC->CRH=0X22000000;
	
	USART1->CR1=1<<2|1<<13;
	USART1->CR2=0X00;
	USART1->BRR=0X341;
	
	lcd(0,0x02);
	lcd(0,0x28);
	lcd(0,0x0c);
	
	while(1)
	{
		char ch=Rx();
		int i=0;
		char msg[21];
		if(ch=='D')
		{
			
			lcd(0,0xC0);
			while(i < 21)
			{
					msg[i]=Rx();
					if(msg[i]=='#')
					{
            break;
					}
					i++;
			}
			msg[i]= '\0';
			string(msg);
		}			
		else if(ch=='F') 
		{
			char d1=Rx(); 
			char d2=Rx();   
			char d3=Rx();    

			lcd(0,0x80);
			string("FUEl lvl: ");
			lcd(1,d1);
			lcd(1,d2);
			lcd(1,d3);
			lcd(1,'%');
		}			
			 
		
	}
	
}

