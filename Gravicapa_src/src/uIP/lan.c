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

#include "rtc.h"

#include <string.h>
#include <stdlib.h>

static uint8_t cgi_path[50];
static uip_ipaddr_t* server_addr;

static uint8_t func_num(const char * func){

    if(strcmp(func,"reg")==0){
        return FUNC_REG; 
    }

    if(strcmp(func,"settime")==0){
        return FUNC_SETTIME; 
    }
    if(strcmp(func,"gettime")==0){
        return FUNC_GETTIME;
    }

    return FUNC_WRONG;

}

/* reverse:  переворачиваем строку s на месте */
 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

/* itoa:  конвертируем n в символы в s */
 void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)  /* записываем знак */
         n = -n;          /* делаем n положительным числом */
     i = 0;
     do {       /* генерируем цифры в обратном порядке */
         s[i++] = n % 10 + '0';   /* берем следующую цифру */
     } while ((n /= 10) > 0);     /* удаляем */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

uint32_t atoi_s(char* str){

uint16_t len=strlen(str);
uint32_t result=0;
for(uint16_t i=0;i<len;i++){
    result*=10;
    result+=str[i]-'0';
}
return result;
}

/*this is the same as strtok in stdlib but without malloc*/
uint16_t strpos;
char* strptr;
int16_t strtok_s(char* s,uint16_t len, char delim,char* buf){    
    if(s != NULL){
        strptr = s;
        strpos = 0 ;
        return 0;
    }else{
        uint16_t bufpos=0;
        while(strpos < len && strptr[strpos]!=delim){
            buf[bufpos]=strptr[strpos];
            strpos++;
            bufpos++;
        }
        strpos++;
        return strpos;
    }
    
}


/*
 * This is the function that handle connection with server over 12345 port.
 * It processes data from server.
 */
static uint8_t b19b00b5_process(){
    uint8_t buf[20];
    int16_t b_len=0;
    strtok_s(uip_appdata,uip_datalen(),'\0',(char*)buf);
    /*check magic data*/
    memset(buf,'\0',sizeof(buf));
    b_len=strtok_s(NULL,uip_datalen(),'=',(char*)buf);
    if(strcmp(buf,"magicnumber")!=0){
        /*bad pack*/
        return 0;
    }
    memset(buf,'\0',sizeof(buf));
    b_len=strtok_s(NULL,uip_datalen(),'&',(char*)buf);
    if(strcmp(buf,MAGIC_WORD)!=0){
        /*bad magic*/
        return 0;
    }

    memset(buf,'\0',sizeof(buf));
    b_len=strtok_s(NULL,uip_datalen(),'=',(char*)buf);
    if(strcmp(buf,"func")!=0){
        /*bad function*/
        return 0;
    }
    memset(buf,'\0',sizeof(buf));
    b_len=strtok_s(NULL,uip_datalen(),'&',(char*)buf);

    switch(func_num(buf)){
            case FUNC_REG:{                
                memset(buf,'\0',sizeof(buf));
                b_len=strtok_s(NULL,uip_datalen(),'=',(char*)buf);                
                if(strcmp(buf,"path")!=0){
                    /*bad path*/
                    return 0;
                }
//                memset(buf,'\0',sizeof(cgi_path));
                /*Get path to CGI script*/
                b_len=strtok_s(NULL,uip_datalen(),'&',(char*)cgi_path);
                /*get time from server*/
                memset(buf,'\0',sizeof(buf));
                b_len=strtok_s(NULL,uip_datalen(),'=',(char*)buf);                
                if(strcmp(buf,"time")!=0){
                    /*bad time part*/
                    return 0;
                }
                memset(buf,'\0',sizeof(buf));
                b_len=strtok_s(NULL,uip_datalen(),'&',(char*)buf);                
                if(strlen(buf)==0){
                    /*bad time*/
                    return 0;
                }
                uint32_t time=atoi_s((char*)buf);
                rtc_set(time);
                
                server_addr = uip_conn->ripaddr;
                break;
            }
            case FUNC_GETTIME:{
                char time[10];
                itoa(rtc_get(),(char*)time);
                uip_send(time,strlen(time));
                break;
            }
            case FUNC_SETTIME:{
                

            }
                    

    }

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

//uint8_t d_proc = 0;
/*---------------------------------------------------------------------------*/
/*
 * In hello-world.h we have defined the UIP_APPCALL macro to
 * lan_appcall so that this funcion is uIP's application
 * function. This function is called whenever an uIP event occurs
 * (e.g. when a new connection is established, new data arrives, sent
 * data is acknowledged, data needs to be retransmitted, etc.).
 */
void lan_appcall(void){
    switch(uip_conn->lport) {
        case HTONS(APP_PORT):
            if(uip_newdata()){
                b19b00b5_process();
//              uip_close();
            }
            break;
        case HTONS(80):
            http_send_data();
            break;
    }
}
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

