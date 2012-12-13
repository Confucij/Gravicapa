/**
 * \addtogroup helloworld
 * @{
 */

/**
 * \file
 *         An example of how to write uIP applications
 *         with protosockets.
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

/*
 * This is a short example of how to write uIP applications using
 * protosockets.
 */

/*
 * We define the application state (struct lan_state) in the
 * hello-world.h file, so we need to include it here. We also include
 * uip.h (since this cannot be included in hello-world.h) and
 * <string.h>, since we use the memcpy() function in the code.
 */
#include "lan.h"
#include "uip.h"
#include <string.h>



/*
 * This is the function that handle connection with theserver over 12345 port.
 * It processes data from server.
 */
 static uint8_t b19b00b5_process(){


 }

/*
 * This is the function that handle connection with theserver over 80 port.
 * It sends data to server.                                             
 */
static uint8_t http_send_data(){


}


/*
 * Declaration of the protosocket function that handles the connection
 * (defined at the end of the code).
 */
static int handle_connection(struct lan_state *s);
/*---------------------------------------------------------------------------*/
/*
 * The initialization function. We must explicitly call this function
 * from the system initialization code, some time after uip_init() is
 * called.
 */
void lan_init(void){
  /* We start to listen for connections on TCP port 12345. */
  uip_listen(HTONS(APP_PORT));
}
/*---------------------------------------------------------------------------*/
/*
 * In hello-world.h we have defined the UIP_APPCALL macro to
 * lan_appcall so that this funcion is uIP's application
 * function. This function is called whenever an uIP event occurs
 * (e.g. when a new connection is established, new data arrives, sent
 * data is acknowledged, data needs to be retransmitted, etc.).
 */
void lan_appcall(void){
  if(uip_newdata() || uip_rexmit()) {
    uip_send("ok\n", 3);
}}
/*---------------------------------------------------------------------------*/
/*
 * This is the protosocket function that handles the communication. A
 * protosocket function must always return an int, but must never
 * explicitly return - all return statements are hidden in the PSOCK
 * macros.
 */
static int handle_connection(struct lan_state *s){
  PSOCK_BEGIN(&s->p);

  PSOCK_SEND_STR(&s->p, "Hello. What is your name?\n");
  PSOCK_READTO(&s->p, '\n');
  strncpy(s->name, s->inputbuffer, sizeof(s->name));
  PSOCK_SEND_STR(&s->p, "Hello ");
  PSOCK_SEND_STR(&s->p, s->name);
  PSOCK_CLOSE(&s->p);
  
  PSOCK_END(&s->p);
}
/*---------------------------------------------------------------------------*/

