
#include "main.h"
#include "usart.h"
#include "sysinit.h"
//#include "sim800c.h"
#include "mqtt.h"
//#include "cmsis_os2.h"  // for osdelay
//#include "barrier.h"
//#include "phonebook.h"

void vTaskButtonUser (void* argument);
void vTimer (void* argument);

int hour = 0;
int sec = 0;
int min = 0;

char testdata = 53;
	void* ptr = &testdata;
	void* ptr1 = (void*)0x08010004; 
uint16_t x;	
int main(void)
{
	
	HSECrystalInit();
	USART2Init();    // connection with pc
	sim800cInit();
	ButtonInit();
	LedInit();
	PhoneBookInit((void*)0x08010000);	
//	BarrierInit(eBarrierStatusClosed);
	xTaskCreate(vTaskButtonUser, "ButtonUser", 64, NULL, 1, NULL);
	xTaskCreate(vTimer, "Timer", 32, NULL, 1, NULL);
	vTaskStartScheduler();
//
	
	while(1)
	{

	}
}



void vTaskButtonUser (void* argument)
{
	osDelay(5000);
	sim800cRestart();
	sim800cSMSInit();
//	sim800cInternetInit();
//	UmqttInit();
//	osDelay(5000);
//	UmqttSub();
//	osDelay(5000);
	eLedState ledstate = eLedOff;
	eButtonState buttonstate = eButtonUp;
	while(1)
	{
		if ((GetButtonState() == eButtonDown) && (buttonstate == eButtonUp))
		{
			vTaskDelay(15);
			if ((GetButtonState() == eButtonDown) & (buttonstate == eButtonUp))
			{
				vTaskDelay(15);
				if ((GetButtonState() == eButtonDown) & (buttonstate == eButtonUp))
				{
					

					vTaskDelay(15);
					buttonstate = eButtonDown;
//					sim800cRestart();
//					sim800cSMSInit();
//					sim800cInternetInit();
//					UmqttInit();
//					osDelay(5000);
//					USART1SendStr("jopa\r\n");
				//	UmqttSub();
			//		osDelay(5000);
					sim800cSendSms("+79135520932", "button down");
					if(ledstate == eLedOff)
					{
						
						ledstate = eLedOn;
//						taskENTER_CRITICAL();
//						Internal_Flash_Erase(4);
//						Internal_Flash_Write(ptr, ptr1, 1);
//						taskEXIT_CRITICAL();
					}
					else
					{
						LedOff();
						ledstate = eLedOff;
					}
				}
			}
		}
		else
		{
			vTaskDelay(60);
			if(GetButtonState() == eButtonUp)
			{
				buttonstate = eButtonUp;
			}
		}
	}
}

void vTimer (void* argument)
{
	while(1)
	{
		sec = sec+1;
		vTaskDelay(1000);
	}
}

typedef enum
{
  mstarted,
	mwait
} mstat;

char mesbuf[8];

//void USART2_IRQHandler(void)
//{
//	
//	char data;
//	while(USART2->SR & USART_SR_RXNE)
//	{
//		data = (char)USART2->DR;
////		USART2SendChar(data);
//	}
//}

// Terminal = SMS
void USART2_IRQHandler(void)
{
	static mstat mstatus = mwait;
	static uint32_t messymbindex = 0;
	char data;
	while(USART2->SR & USART_SR_RXNE)
	{
		data = (char)USART2->DR;
		if((data == '\r') || (data == '\n'))
		{
			if (mstatus == mstarted)
			{
				mesbuf[messymbindex++] = '\r';
				mesbuf[messymbindex++] = '\n';
				mesbuf[messymbindex] = '\0';
//				sim800cSendSms("+79029788005", mesbuf);
//				sim800cSendSms("+79135520932", mesbuf);	
				USART2SendStr(mesbuf);				
			}
			mstatus = mwait;
			messymbindex = 0;
		}
		else if((data >= 32)&&(data <= 125))
		{
			mstatus = mstarted;
			mesbuf[messymbindex++] = data;
		}
	}
}































