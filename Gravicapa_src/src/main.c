

#include "stm32f10x.h"
#include "enc28j60.h"
#include "misc.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>



char uip_buf[256];

void vTask_uIP(void *pvParameters) {
    volatile uint32_t st;
    uint16_t uip_len=0;

    for (;;) {
        if ( uip_len = enc28j60_recv_packet((uint8_t *) uip_buf, sizeof(uip_buf))) {
            enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
        }
    }
}


void main(void)
{

    
    char mac[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x00 };
   	enc28j60_init(mac);
    
	xTaskCreate( vTask_uIP, ( signed char * ) "uIP",
			configMINIMAL_STACK_SIZE*2, NULL, 2, ( xTaskHandle * ) NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

    for(volatile uint32_t i=0;;i++)
    {
        ;
    }
}
