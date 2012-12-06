

#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"
#include "onewire.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>


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




void main(void)
{
    for(volatile uint32_t i=0;;i++)
    {
        ;
    }
}
