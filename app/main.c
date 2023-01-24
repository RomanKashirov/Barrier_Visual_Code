#include "stm32f411xe.h"
void Internal_Flash_Write(void* Src, void* Dst, int Len);
void UnlockFlash(void);
void LockFlash(void);
void Internal_Flash_Erase(unsigned int Sectornumber);

uint16_t testdata = 53;

void* ptr1 = (void*)0x08010004; 

int i = 0;

int main(void)
{
	
	// IO port A clock enabled
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0b01 << GPIO_MODER_MODER5_Pos; // General purpose output mode
	
	while(1)
	{
		for(i = 0; i < 100000; i++);
		GPIOA->BSRR |= GPIO_BSRR_BS5;
		for(i = 0; i < 100000; i++);
		GPIOA->BSRR |= GPIO_BSRR_BR5;
//		Internal_Flash_Erase(4);
//        Internal_Flash_Write(&testdata, ptr1, 1);
		testdata++;
	}
}







void Internal_Flash_Write(void* Src, void* Dst, int Len)
{
	while (FLASH->SR & FLASH_SR_BSY);
	UnlockFlash();
	FLASH->CR &= ~FLASH_CR_PSIZE_Msk;              // the device voltage range is 2.7V to 3.6V,
  FLASH->CR |= 0b01 << FLASH_CR_PSIZE_Pos;           // the operation is by word (32-bit)
	uint16_t* SrcW = (uint16_t*)(&testdata);
  volatile uint16_t* DstW = (uint16_t*)Dst;
	
	
//	if (FLASH->SR & FLASH_SR_EOP) 
//	{
//		FLASH->SR |= FLASH_SR_EOP;
//	}

	FLASH->CR |= FLASH_CR_PG;
	*DstW = *SrcW;
//	FLASH->CR |= FLASH_CR_STRT;
//	while (!(FLASH->SR & FLASH_SR_EOP));
//	FLASH->SR |= FLASH_SR_EOP;
	
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_PG; // ?
	LockFlash();
}



#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)
void UnlockFlash(void)
{
	FLASH->KEYR = FLASH_KEY1;
	FLASH->KEYR = FLASH_KEY2;
}

void LockFlash(void)
{
	FLASH->CR |= FLASH_CR_LOCK;
}





void Internal_Flash_Erase(unsigned int Sectornumber)
{
	
	while (FLASH->SR & FLASH_SR_BSY);
	
	UnlockFlash();
	
	FLASH->CR &= ~FLASH_CR_PSIZE_Msk;              // the device voltage range is 2.7V to 3.6V,
  FLASH->CR |= 0b01 << FLASH_CR_PSIZE_Pos;           // the operation is by word (32-bit)
	
//	if (FLASH->SR & FLASH_SR_EOP) 
//	{
//		FLASH->SR = FLASH_SR_EOP;
//	}
	
	FLASH->CR |= FLASH_CR_SER;
	FLASH->CR |= Sectornumber << FLASH_CR_SNB_Pos;
	FLASH->CR |= FLASH_CR_STRT;
	
//	while (!(FLASH->SR & FLASH_SR_EOP));
//	FLASH->SR = FLASH_SR_EOP;
	 // ?
	
	while (FLASH->SR & FLASH_SR_BSY);
	FLASH->CR &= ~FLASH_CR_SER;
	LockFlash();
}
