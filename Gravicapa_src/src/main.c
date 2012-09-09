

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
void spi_init(void)
{
    RCC->APB2ENR |=  RCC_APB2ENR_AFIOEN;//включить тактирование альтернативных функций
    RCC->APB2ENR |=  RCC_APB2ENR_IOPAEN;//включить тактирование порта А

    //вывод управления SS: выход двухтактный, общего назначения,50MHz
    GPIOA->CRL   |=  GPIO_CRL_MODE4;    //
    GPIOA->CRL   &= ~GPIO_CRL_CNF4;     //
    GPIOA->BSRR   =  GPIO_BSRR_BS4;     //

    //вывод SCK: выход двухтактный, альтернативная функция, 50MHz
    GPIOA->CRL   |=  GPIO_CRL_MODE5;    //
    GPIOA->CRL   &= ~GPIO_CRL_CNF5;     //
    GPIOA->CRL   |=  GPIO_CRL_CNF5_1;   //

    //вывод MISO: вход цифровой с подтягивающим резистором, подтяжка к плюсу
    GPIOA->CRL   &= ~GPIO_CRL_MODE6;    //
    GPIOA->CRL   &= ~GPIO_CRL_CNF6;     //
    GPIOA->CRL   |=  GPIO_CRL_CNF6_1;   //
    GPIOA->BSRR   =  GPIO_BSRR_BS6;     //

    //вывод MOSI: выход двухтактный, альтернативная функция, 50MHz
    GPIOA->CRL   |=  GPIO_CRL_MODE7;    //
    GPIOA->CRL   &= ~GPIO_CRL_CNF7;     //
    GPIOA->CRL   |=  GPIO_CRL_CNF7_1;   //

    //настроить модуль SPI
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //подать тактирование
    SPI1->CR2     = 0x0000;             //
    SPI1->CR1     = SPI_CR1_MSTR;       //контроллер должен быть мастером,конечно    
    SPI1->CR1    |= SPI_CR1_BR;         //для начала зададим маленькую скорость
    SPI1->CR1    |= SPI_CR1_SSI;
    SPI1->CR1    |= SPI_CR1_SSM;
    SPI1->CR1    |= SPI_CR1_SPE;        //разрешить работу модуля SPI
}

uint8_t spi_send (uint8_t data)
{ 
    while (!(SPI1->SR & SPI_SR_TXE));      //убедиться, что предыдущая передача завершена
    SPI1->DR = data;                       //загружаем данные для передачи
    while (!(SPI1->SR & SPI_SR_RXNE));     //ждем окончания обмена
    return (SPI1->DR);		         //читаем принятые данные
}

#define delay()						\
    do {							\
        register unsigned int i;				\
        for (i = 0; i < 10; ++i)				\
        __asm__ __volatile__ ("nop\n\t":::"memory");	\
    } while (0)

void main(void)
{
    setup_leds();

    //  SysTickCurrent = 0x9000;
    //  SysTickReload = 0x9000;
    //  SysTickControl = 0x07;
    uint8_t temp;
    spi_init();
    while (1)
    {
    temp=0;
    temp=spi_send(10);
    if(temp==10){
        switch_leds_on();
    }
    delay();
    switch_leds_off();
    delay();
  }
}
