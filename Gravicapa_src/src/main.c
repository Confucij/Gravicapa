

#include "stm32f10x.h"
#define LED_PORT GPIOC

#define LED_BLUE (1 << 8) /* port C, pin 8 */
#define LED_GREEN (1 << 9) /* port C, pin 9 */
#define LED_ORANGE 0
#define LED_RED 0

static inline void setup_leds(void)
{
    // Make sure clocks work...
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    // Make sure we're in mode 0, with output push-pull
    LED_PORT->CRH |= GPIO_CRH_MODE8_0 | GPIO_CRH_MODE9_0;
    LED_PORT->CRH &= ~(GPIO_CRH_CNF8 | GPIO_CRH_CNF9);
}


static inline void switch_leds_on(void)
{
    LED_PORT->ODR = LED_BLUE | LED_GREEN | LED_ORANGE | LED_RED;
}

static inline void switch_leds_off(void)
{
    LED_PORT->ODR = 0;
}

#define delay()						\
do {							\
  register unsigned int i;				\
  for (i = 0; i < 1000000; ++i)				\
    __asm__ __volatile__ ("nop\n\t":::"memory");	\
} while (0)

void main(void)
{
  setup_leds();
    
  //  SysTickCurrent = 0x9000;
  //  SysTickReload = 0x9000;
  //  SysTickControl = 0x07;
   while (1)
   {
    switch_leds_on();
    delay();
    switch_leds_off();
    delay();
  }
}
