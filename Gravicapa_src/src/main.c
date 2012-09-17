

#include "stm32f10x.h"
#include "ff.h"
#include "diskio.h"
#include "onewire.h"
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

volatile int time_zm;

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
        time_zm++;
    }
}

//void init_uart(){
//GPIO_InitTypeDef  GPIO_InitStructure;                                 //для инициализации порта
//USART_InitTypeDef USART_InitStructure;                                //для инициализации USART
// 
////настроить выводы, к которым подключены RX и TX
//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);                 //тактирование GPIOA
//GPIO_InitStructure.GPIO_Pin                   = GPIO_Pin_10;          //линия RX
//GPIO_InitStructure.GPIO_Mode                  = GPIO_Mode_IN_FLOATING;//вход, третье состояние
//GPIO_Init(GPIOA, &GPIO_InitStructure);                                //выполнить инициализацию
//GPIO_InitStructure.GPIO_Pin                   = GPIO_Pin_9;           //линия TX
//GPIO_InitStructure.GPIO_Speed                 = GPIO_Speed_50MHz;     //макс частота сигнала
//GPIO_InitStructure.GPIO_Mode                  = GPIO_Mode_AF_PP;      //симетричный выход
//GPIO_Init(GPIOA, &GPIO_InitStructure);
//RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //Тактирование GPIO
//RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;   //Тактирование альтернативных функций GPIO
//RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Тактирование модуля USART1
//USART1->BRR = 0x09C4;
//USART1->CR2 |= USART_CR2_STOP_0;
//USART1->CR1 = USART_CR1_RE | USART_CR1_TE | USART_CR1_UE;
//}



#define delay()						\
    do {							\
        register unsigned int i;				\
        for (i = 0; i < 1000000000; ++i)				\
        __asm__ __volatile__ ("nop\n\t":::"memory");	\
    } while (0)

void main(void)
{
    setup_leds();
    init_timer();
    FATFS fs;
    FIL file;

    if(f_mount(0,&fs) == FR_OK){
        OW_Init();
            if(f_open(&file,"ASDF",FA_WRITE)==FR_OK){
            GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) | LED_GREEN );
            int k;
            uint8_t tempbuf[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            for(k=0;k<1;k++){
                time_zm=0;
                while(time_zm<120){}
                uint8_t temp=OW_Send(OW_SEND_RESET,"\xcc\x44",2,0,0,OW_NO_READ);
                if( temp== OW_NO_DEVICE)break;
                
                uint8_t buf[2];
                time_zm=0;
                while(time_zm<120){}
                GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) | LED_BLUE );

                temp=OW_Send(OW_SEND_RESET, "\xcc\xBE\xff\xff", 11, tempbuf,2, 2);
                if( temp== OW_NO_DEVICE)break;
                //f_printf("Измеренная температура - %d;\n",(uint16_t)buf);
            }
            UINT rd;
            f_write(&file,tempbuf,20,&rd);
            f_close(&file);
            f_mount(0,0);
          //  GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) | LED_BLUE );
        }
    }
    while(1)
    {
    }
}
