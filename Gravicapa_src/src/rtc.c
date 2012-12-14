
#include <stdint.h>
#include "stm32f10x.h"
#include "rtc.h"

uint8_t  rtc_init(){
 //разрешить тактирование модулей управления питанием и управлением резервной областью
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;
  //разрешить доступ к области резервных данных
  PWR->CR |= PWR_CR_DBP;
  //если часы выключены - инициализировать их
  if ((RCC->BDCR & RCC_BDCR_RTCEN) != RCC_BDCR_RTCEN)
  {
    //выполнить сброс области резервных данных
    RCC->BDCR |=  RCC_BDCR_BDRST;
    RCC->BDCR &= ~RCC_BDCR_BDRST;
 
    //установить бит разрешения работы и дождаться установки бита готовности
    RCC->CSR |= RCC_CSR_LSION;
    while (!(RCC->CSR & RCC_CSR_LSIRDY)){}

    //выбрать источником тактовых импульсов LSI  и подать тактирование
    RCC->BDCR |=  RCC_BDCR_RTCEN | RCC_BDCR_RTCSEL_LSI;
 
    while(!(RTC->CRL & RTC_CRL_RTOFF)){}
    RTC->CRL  |=  RTC_CRL_CNF;
    //регистр деления на 40000
    RTC->PRLL  = 0x9C40;   
    RTC->CNTH = 0x00;
    RTC->CNTL = 0x00;
    RTC->CRL  &=  ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF)){}
 
  }
  return 0;
}

uint32_t rtc_get(){
    uint32_t result = RTC->CNTH<<16;
    result |= RTC->CNTL;
    return result; 

}

void rtc_set(uint32_t time){
    while(!(RTC->CRL & RTC_CRL_RTOFF)){}
    RTC->CRL  |=  RTC_CRL_CNF;
    RTC->CNTH = (time>>16);
    RTC->CNTL = time;
    RTC->CRL  &=  ~RTC_CRL_CNF;
    while(!(RTC->CRL & RTC_CRL_RTOFF)){}
}
