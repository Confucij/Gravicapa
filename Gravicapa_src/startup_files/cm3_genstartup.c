/**
 * Karl Palsson, 2011.  Compiled from all sorts of sources, with wisdom
 * from places untold and untracked.  Considered to be released into the public
 * domain.
 *
 * This was helpful:
 * http://dev.frozeneskimo.com/notes/getting_started_with_cortex_m3_cmsis_and_gnu_tools
 *
 * Handles setting up data in ram, zeroing bss, and calling SystemInit for clocks
 * then jumping to main
 */

/**
 * Expected to be provided by your application, naturally :)
 * @return 
 */
#include "stm32f10x.h"
#include "misc.h"
//TODO Почему-то не видело объявления в "stm32f10x.h" скопипастил сюда. Костыль. Поправить. Ага, поправить. Так оно тут и будет.
#define  RCC_CFGR_PLLSRC_HSE                ((uint32_t)0x00010000)        /*!< HSE clock selected as PLL entry clock source */
extern int main(void);

/* provided by the linker script */
extern unsigned long _estack;
extern unsigned long __text_end;
extern unsigned long __data_start;
extern unsigned long __data_end;
extern unsigned long __bss_start;
extern unsigned long __bss_end;


/**
 * Cortex M3 core interrupt handlers
 * 
 * We provide basic implementations of the core of them, but "weak" so that
 * simply declaring them again will count as implementing them
 */
void Reset_Handler(void);
void NMI_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void HardFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void MemManage_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void BusFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void UsageFault_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SVC_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void DebugMon_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void PendSV_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
void SysTick_Handler(void) __attribute__ ((weak, alias ("Dummy_Handler")));
extern void TIM6_DAC_IRQHandler();
extern void EXTI4_handler();
extern void EXTI5_9_handler();
/**
 * The chip specific (STM32L1xx or F1, or LPC, or TI etc) vectors are in a 
 * chip specific file.  by placing them in a subsection, they can be linked in
 * immediately after the core cortex handler table.
 */

void Dummy_Handler(void) {
    while(1) {
        ;
    }
}


/**
 * All the parts start on some sort of an internal clock.
 * STM32F101, 103, 105, 107 start on HSI (~8MHz)
 * STM32F100xx (Value Line) start on HSI (~8MHz)
 * STM32F2xx starts on HSI (~16MHz)
 * STM32F4xx starts on HSI (~16MHz)
 * STM32L1xx starts on MSI (~2MHz)
 * Additionally, the STM32L1xx starts in voltage range 2 (1.5V), and needs
 * to be switched to voltage range 1 before enabling its HSI (~16MHz)
 *
 * Regardless, the default startup clock is enough to run.  However, you may
 * wish to override this function, (simply implement your own) and configure
 * clocks, power regulators, or any other "premain" code you wish to run.
 */
void SystemInit(void) __attribute__ ((weak));
void SystemInit(void) {
__IO uint32_t StartUpCounter = 0, HSEStatus = 0;
 
/* Конфигурацяи  SYSCLK, HCLK, PCLK2 и PCLK1 */    
/* Включаем HSE */    
RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
/* Ждем пока HSE не выставит бит готовности либо не выйдет таймаут*/
do 
	{
    	HSEStatus = RCC->CR & RCC_CR_HSERDY;
    	StartUpCounter++;  
  	} 
while( (HSEStatus == 0) && (StartUpCounter != HSEStartUp_TimeOut));
 
//if ( (RCC->CR & RCC_CR_HSERDY) != RESET) 
//  	{
//    	HSEStatus = (uint32_t)0x01;
//  	}
//else
//  	{
//	HSEStatus = (uint32_t)0x00;
//  	}  
 
/* Если HSE запустился нормально */
if ( (RCC->CR & RCC_CR_HSERDY) != RESET) 
  	{
 
    	/* HCLK = SYSCLK */
    	RCC->CFGR |= (uint32_t)RCC_CFGR_HPRE_DIV1;
 
    	/* PCLK2 = HCLK */
    	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE2_DIV1;
 
    	/* PCLK1 = HCLK */
    	RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;
 
    	/* Конфигурируем множитель PLL configuration: PLLCLK = HSE * 9 = 72 MHz */
    	/* При условии, что кварц на 8МГц! */
    	/* RCC_CFGR_PLLMULL9 - множитель на 9. Если нужна другая частота, не 72МГц */
    	/* то выбираем другой множитель. */
    	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL));
    	RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLMULL6);
 
    	/* Включаем PLL */
    	RCC->CR |= RCC_CR_PLLON;
 
    	/* Ожидаем, пока PLL выставит бит готовности */
    	while((RCC->CR & RCC_CR_PLLRDY) == 0) 
    		{
    		// Ждем
    		}
 
    	/* Выбираем PLL как источник системной частоты */
    	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;    
 
    	/* Ожидаем, пока PLL выберется как источник системной частоты */
    	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)0x08) 
    		{
    		// Ждем
    		}
 
  	}
  else 
 	{
   	/* Все плохо... HSE не завелся... Чего-то с кварцем или еще что...
      	Надо бы както обработать эту ошибку... Если мы здесь, то мы работаем
      	от HSI! */
  	}
}


/**
 * This table contains the core Coretex vectors, and should be linked first.
 * You should link any chip specific tables after this.
 */


void *vector_table[] __attribute__ ((section(".vectors"))) = {
	&_estack,
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
};


void __attribute__((noreturn, naked)) Reset_Handler() {
    unsigned long *src;
    unsigned long *dest;
    
    src = &__text_end;
    dest = &__data_start;
    if (src != dest)
        while(dest < &__data_end)
            *(dest++) = *(src++);
 
    dest = &__bss_start;
    while(dest < &__bss_end)
        *(dest++) = 0;
     static unsigned int fstack;
     fstack = (unsigned int)vector_table[0];
  __asm__ __volatile__
    (
     "ldr sp, %0\n\t"
     : 
     : "m"(fstack)
     : "sp"
    );

    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0);
	SystemInit();
	main();
}


void *other_vector_table[] __attribute__ ((section(".vectors.other"))) = {
0, // Window watchdog interrupt 
0,
0,
0,
0,
0,
0,
0,
0,
0,
EXTI4_handler,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
EXTI5_9_handler,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,//UART1_Interupt,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,//TIM6_DAC_IRQHandler, //TIM6
0,
0, 
0,
0,
0,
};
