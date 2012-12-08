

#include "stm32f10x.h"
#include "enc28j60.h"
#include "misc.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>



xQueueHandle message_q;
char uip_buf[256];

void EXTI4_handler(){
    uint8_t temp=1;
    xQueueSendFromISR(message_q,&temp,0);
    // Сбрасываем флаг прерывания
    EXTI->PR |= EXTI_PR_PR4;
}

void vTask_uIP(void *pvParameters) {
    volatile uint32_t st;
    uint16_t uip_len=0;

    for (;;) {
        uint8_t tmp;
        xQueueReceive(message_q,&tmp,portMAX_DELAY);
        if ( uip_len = enc28j60_recv_packet((uint8_t *) uip_buf, sizeof(uip_buf))) {
            enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
        }
    }
}

void init_structs(){
message_q = xQueueCreate(10,sizeof(uint8_t));

}


void main(void)
{

    
    char mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x00 };
   	enc28j60_init(mac);
    
	xTaskCreate( vTask_uIP, ( signed char * ) "uIP",
			configMINIMAL_STACK_SIZE*2, NULL, 2, ( xTaskHandle * ) NULL);
    init_structs();

	/* Start the scheduler. */
	vTaskStartScheduler();

    for(volatile uint32_t i=0;;i++)
    {
        ;
    }
}
