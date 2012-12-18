/*
 * onewire.h
 *
 *  Version 1.0.1
 */

#ifndef ONEWIRE_H_
#define ONEWIRE_H_

// для разных процессоров потребуется проверить функцию OW_Init
// на предмет расположения ножек USART
#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_usart.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

// выбираем, на каком USART находится 1-wire
//А вот хрен там оно нормально выбереться - надо править функци!
//Работает нормально с вторым юартом.
//#define OW_USART1
#define OW_USART2
//#define OW_USART3
//#define OW_USART4


// если нужно отдавать тики FreeRTOS, то раскомментировать
//#define OW_GIVE_TICK_RTOS

// первый параметр функции OW_Send
#define OW_SEND_RESET		1
#define OW_NO_RESET		2

// статус возврата функций
#define OW_OK			1
#define OW_ERROR		2
#define OW_NO_DEVICE	3

#define OW_NO_READ		0xff

#define OW_READ_SLOT	0xff

#define OW_COM_SEARCH 0xF0
#define OW_COM_RDROM  0x33
#define OW_COM_MTCROM 0x55
#define OW_COM_SKPROM 0xCC
#define OW_PRESENCE 0xE0

/*DS18B20 Commands*/
#define OW_DS18b20_FAMILY 0x28
#define OW_DS18B20_CONV 0x44
#define OW_DS18b20_WRTSCRATCh 0x4E
#define OW_DS18b20_RDSCRATCH 0xBE
#define OW_DS18b20_CPYSCRATCH 0x48
#define OW_DS18b20_RECALL 0xB8
#define OW_DS18b20_RDPWRSUPP 0xB4
/*DS18B20 Commands*/


uint8_t ds_reset_pulse(uint16_t PinMask);

void ds_write_bit(uint8_t bit,uint16_t PinMask);

uint16_t ds_read_bit(uint16_t PinMask);

void ds_write_byte(uint8_t byte, uint16_t PinMask);

uint8_t ds_read_byte(uint16_t PinMask);

uint8_t ds_start_convert_single(uint8_t PinNumb);

uint8_t ds_read_data_single(uint8_t *buff, uint8_t PinNumb);

signed int ds_read_temperature(uint8_t PinNumb);

uint8_t ds_init();

#endif /* ONEWIRE_H_ */
