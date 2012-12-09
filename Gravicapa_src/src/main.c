

#include "stm32f10x.h"
#include "enc28j60.h"
#include "misc.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uip.h"
#include "uip_arp.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>



xQueueHandle message_q;

uint8_t tmp;

void EXTI4_handler(){
    uint8_t temp=1;
    xQueueSendFromISR(message_q,&temp,0);
    // Сбрасываем флаг прерывания
    EXTI->PR |= EXTI_PR_PR4;
}

void vTask_uIP_periodic(void *pvParameters) {
    for(;;){

        vTaskDelay(configTICK_RATE_HZ/5); 
    }

}

void vTask_uIP(void *pvParameters) {

    for (;;) {
        // vTaskDelay(configTICK_RATE_HZ/10); 
        xQueueReceive(message_q,&tmp,portMAX_DELAY);
        tmp++;
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

    init_structs();
    // это будет наш МАС-адрес
    struct uip_eth_addr mac = { { 0x00, 0x01, 0x02, 0x03, 0x04, 0x00 } };

    // проинитим наш  enc28j60
    enc28j60_init(mac.addr);

    // инициализация стека
    uip_init();
    uip_arp_init();

    // инициализация приложения, потом втулим сюда веб-сервер
   // hello_world_init();

    // установим наш МАС
    uip_setethaddr(mac);

    // установим адрес хоста (не используем dhcp)
    // наш хост будет доступен по адресу 192.168.2.55
    uip_ipaddr_t ipaddr;
    uip_ipaddr(ipaddr, 192, 168, 0, 55);
    uip_sethostaddr(ipaddr);
    uip_ipaddr(ipaddr, 192, 168, 0, 12);
    uip_setdraddr(ipaddr);
    uip_ipaddr(ipaddr, 255, 255, 255, 0);
    uip_setnetmask(ipaddr);

    xTaskCreate( vTask_uIP_periodic, ( signed char * ) "uIPp",
            configMINIMAL_STACK_SIZE*2, NULL, 1, ( xTaskHandle * ) NULL);

	xTaskCreate( vTask_uIP, ( signed char * ) "uIP",
			configMINIMAL_STACK_SIZE*2, NULL, 2, ( xTaskHandle * ) NULL);

	/* Start the scheduler. */
	vTaskStartScheduler();

    for(volatile uint32_t i=0;;i++)
    {
        ;
    }
}
