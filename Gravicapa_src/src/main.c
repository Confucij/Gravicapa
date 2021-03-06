

#include "stm32f10x.h"

#include "enc28j60.h"
#include "rtc.h"

#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "uip.h"
#include "uip_arp.h"
#include "lan.h"

#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include "misc.h"

#include "onewire.h"
#include "sensors.h"
#define BUF ((struct uip_eth_hdr *)&uip_buf[0])
xQueueHandle message_q;
xQueueHandle new_data;
/*Server addres*/
extern uint16_t server_addr[2];


uint16_t c_state,p_state;

sensor term,door;

uint8_t tmp;

void EXTI4_handler(){
    uint8_t temp=1;
    xQueueSendFromISR(message_q,&temp,0);
    // Сбрасываем флаг прерывания
    EXTI->PR |= EXTI_PR_PR4;
}

void EXTI5_9_handler(){
    door.value ^= 1; 
    xQueueSendFromISR(new_data,&door,0);
    if(server_addr[0] != 0 && server_addr[1] !=0){
        uip_connect(&server_addr, HTONS(80));
    }
    EXTI->PR |= 0xFFFF;

}

void vTask_uIP_periodic(void *pvParameters) {

        uint32_t i;
        uint8_t delay_arp = 0;

        for (;;) {
            vTaskDelay(configTICK_RATE_HZ/2); // полсекунды
            delay_arp++;
            for (i = 0; i < UIP_CONNS; i++) {
                uip_periodic(i);
                if (uip_len > 0) {
                    uip_arp_out();
                    enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
                }
            }

#if UIP_UDP
            for(i = 0; i < UIP_UDP_CONNS; i++) {
                uip_udp_periodic(i);
                if(uip_len > 0) {
                    uip_arp_out();
                    network_send();
                }
            }
#endif /* UIP_UDP */

            if (delay_arp >= 50) { // один раз за 50 проходов цикла, около 10 сек.
                delay_arp = 0;
                uip_arp_timer();
            }
        }


}

void vTask_uIP(void *pvParameters) {

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
    
    /*Initialize tcp/ip processing part*/
    lan_init();
    for (;;) {
        // vTaskDelay(configTICK_RATE_HZ/10); 
        xQueueReceive(message_q,&tmp,configTICK_RATE_HZ*3);
        while ( uip_len = enc28j60_recv_packet((uint8_t *) uip_buf, sizeof(uip_buf))) {
            if (BUF->type == htons(UIP_ETHTYPE_IP)) {
                uip_arp_ipin();
                uip_input();
                if (uip_len > 0) {
                    uip_arp_out();
                    enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
                }
            } else if (BUF->type == htons(UIP_ETHTYPE_ARP)) {
                uip_arp_arpin();
                if (uip_len > 0) {
                    enc28j60_send_packet((uint8_t *) uip_buf, uip_len);
                }
            }
        }
    }
}

/*Periodic function. Check if any data available*/
void vTask_check_data(void* param){
    
    /*if we dont know where server is*/
//    if(server_addr==NULL){
//        return 1;
//    }
for(;;){
    taskENTER_CRITICAL();
    ds_start_convert_single(10);
    taskEXIT_CRITICAL();
    vTaskDelay(configTICK_RATE_HZ*5);
    taskENTER_CRITICAL();
    term.value = ds_read_temperature(10);
    taskEXIT_CRITICAL();
}    

}

void TimerTask(xTimerHandle xTimer){
    if(!term.time_left){
        if(server_addr[0] != 0 && server_addr[1] !=0){
           if(uip_connect(&server_addr, HTONS(80))){
                xQueueSend(new_data,&term,0);
           }
        }
    term.time_left=term.period;
    }
    term.time_left--;

}

void door_init(){
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOC, &GPIO_InitStruct);
    EXTI->FTSR |= EXTI_FTSR_TR6;
    EXTI->RTSR |= EXTI_FTSR_TR6;
    EXTI->IMR |= EXTI_IMR_MR6;
    EXTI->EMR |= EXTI_EMR_MR6;
    AFIO->EXTICR[1] |=  AFIO_EXTICR2_EXTI6_PC;
    NVIC_EnableIRQ(EXTI9_5_IRQn);

}

void init_structs(){
    message_q = xQueueCreate(10,sizeof(uint8_t));
    new_data = xQueueCreate(10,sizeof(sensor));

    door.id = 0x1234567812345678;
    door.period = -1;
    door.value = 0;
    term.id = 0x0987654321098765;
    term.period = 6;
    term.time_left = 6;

}


void main(void)
{

    init_structs();
    rtc_init(); 
    ds_init();
    door_init();

    xTimerReset(xTimerCreate("tmr", configTICK_RATE_HZ, pdTRUE, 0, TimerTask),0);
	xTaskCreate( vTask_check_data, ( signed char * ) "dtc",
			configMINIMAL_STACK_SIZE, NULL, 1, ( xTaskHandle * ) NULL);
    xTaskCreate( vTask_uIP_periodic, ( signed char * ) "uIPp",
            configMINIMAL_STACK_SIZE, NULL, 1, ( xTaskHandle * ) NULL);

	xTaskCreate( vTask_uIP, ( signed char * ) "uIP",
			configMINIMAL_STACK_SIZE, NULL, 2, ( xTaskHandle * ) NULL);


	/* Start the scheduler. */
	vTaskStartScheduler();

    for(volatile uint32_t i=0;;i++)
    {
        ;
    }
}

