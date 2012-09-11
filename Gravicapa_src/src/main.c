

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



uint8_t spi_send (uint8_t data)
{ 
    while (!(SPI1->SR & SPI_SR_TXE));      //убедиться, что предыдущая передача завершена
    SPI1->DR = data;                       //загружаем данные для передачи
    while (!(SPI1->SR & SPI_SR_RXNE));     //ждем окончания обмена
    return (SPI1->DR);		         //читаем принятые данные
}
uint8_t spi_read (void)
{ 
  return spi_send(0xff);		  //читаем принятые данные
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
    DWORD plist[] = {50, 50, 0, 0};  /* Деление драйва на два раздела */
    BYTE work[_MAX_SS];
   // temp=f_fdisk(0, plist, work);
    if(temp==FR_OK){  /* Деление физического драйва 0 */
 //   f_mount(0, &Fatfs);
 //   f_mkfs(0, 0, 512);          /* Создание тома FAT на логическом драйве 0. 2-й аргумет игнорируется. */
 //   f_mount(0, 0);
    GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) ^ LED_GREEN);
    delay();

    GPIO_Write(GPIOC, GPIO_ReadOutputData(GPIOC) ^ LED_GREEN);
    FIL file;
    if(f_mount(0,&fs) == FR_OK){
            switch_leds_on();            
            temp=f_open(&file,"asdf",FA_CREATE_NEW);
            switch(temp){
                case FR_EXIST:{

                    temp=f_open(&file,"asdf",FA_READ|FA_WRITE);
                    if(temp==FR_OK){
                    int i;
                    uint8_t buffW[512];
                    for(i=0;i<512;i++){
                        buffW[i]=i;   
                    }
                    
                    UINT rd;
                   temp=f_write(&file,&buffW,512,&rd);
                    if(temp=!FR_OK){
                        switch_leds_off();
                    }
                    f_read(&file,&buffW,512,&rd); 
                    if(rd!=512){
                          switch_leds_off();
                      }
                    break;
                }
                }
                case FR_OK:{
                    int i;
                    uint8_t buffW[512];
                    for(i=0;i<512;i++){
                        buffW[i]=i;   
                    }
                    
                    UINT rd;
                    f_write(&file,&buffW,512,&rd);
                    f_write(&file,&buffW,512,&rd);
                    f_read(&file,&buffW,512,&rd); 
                    if(rd!=512){
                          switch_leds_off();
                      }
                    break;

                }

            }

                if(f_close(&file)!=FR_OK){
                    switch_leds_off();
            }
        f_mount(0,0);
    }
    }
//    SD_WriteSector(4,&buffW);
//    LED_PORT->ODR = LED_BLUE;
//    SD_ReadSector(4,&buffR);
//    LED_PORT->ODR = LED_GREEN;
//    for(i=0;i<512;i++){
//        if(buffW[i]!=buffR[i]){
//            LED_PORT->ODR = 0;
//        }
//    }
    while (1)
    {
  }
}
