
#include "FreeRTOS.h"
#include "task.h"


/*Task for Idle state - when non of the task is ready.*/
void vApplicationIdleHook( void){



}

/*If we have problems with stack we got here*/
void vApplicationStackOverflowHook( xTaskHandle xTask, signed portCHAR *pcTaskName ){



}

/*Hook that get called if pvPortMalloc() ever returns NULL*/
void vApplicationMallocFailedHook( void ){


}
