

#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
/* В этом файле - всё для работы с таймерами */
#include <stm32f10x_tim.h>
/* В этом - для работы с NVIC */
#include <misc.h>

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


void init_timer()
{
    /* Не забываем затактировать таймер */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* Инициализируем базовый таймер: делитель 24000, период 500 мс.
     * Другие параметры структуры TIM_TimeBaseInitTypeDef
     * не имеют смысла для базовых таймеров.
     */
    TIM_TimeBaseInitTypeDef base_timer;
    TIM_TimeBaseStructInit(&base_timer);
    /* Делитель учитывается как TIM_Prescaler + 1, поэтому отнимаем 1 */
    base_timer.TIM_Prescaler = 24000 - 1;
    base_timer.TIM_Period = 10;
    TIM_TimeBaseInit(TIM6, &base_timer);

    /* Разрешаем прерывание по обновлению (в данном случае -
     * по переполнению) счётчика таймера TIM6.
     */
    TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);
    /* Включаем таймер */
    TIM_Cmd(TIM6, ENABLE);

    /* Разрешаем обработку прерывания по переполнению счётчика
     * таймера TIM6. Так получилось, что это же прерывание
     * отвечает и за опустошение ЦАП.
     */
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
}


void TIM6_DAC_IRQHandler()
{
    /* Так как этот обработчик вызывается и для ЦАП, нужно проверять,
     * произошло ли прерывание по переполнению счётчика таймера TIM6.
     */
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        /* Очищаем бит обрабатываемого прерывания */
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        /* Инвертируем состояние светодиодов */
        // GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) ^ (LED_BLUE | LED_GREEN));
        disk_timerproc();
    }
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
    init_timer();
    delay();
    //    spi_init();
    FATFS fs;
    FRESULT temp=FR_OK;
    BYTE work[_MAX_SS];
    if(temp==FR_OK){  /* Деление физического драйва 0 */
        GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) ^ LED_GREEN);
        delay();

        GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) ^ LED_GREEN);
        FIL file;
        if(f_mount(0,&fs) == FR_OK){
            temp=f_open(&file,"asdf",FA_READ|FA_WRITE);
            switch(temp){
                case FR_OK:{
                               int i;
                               uint8_t buffW[512];
                               UINT rd;
                               f_read(&file,&buffW,512,&rd); 
                               if(rd==512){
                                   switch_leds_on();
                               }
                               break;

                           }

            }

            if(f_close(&file)!=FR_OK){
            }
            f_mount(0,0);
        }
    }
    while (1)
    {
    }
}
