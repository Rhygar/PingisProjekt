/*
 * task_com.c
 *
 * This task sends data to matlab from the arduino that
 * later on is used to plot in matlab.
 *
 * Created: 2016-03-13 13:06:56
 * Author: Andreas Langhammer and John Tengvall
 */ 
#include <asf.h>
#include "variables.h"
/*Task that handles the communication from matlab*/
void task_com(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer; //Timer arrives from matlab
	
	xLastWakeTime = xTaskGetTickCount();
	uint16_t str[LINJAR_BUFFER_LENGTH] = {0};
	/*runs continuously until task is stopped */
	while(1)
	{
		
		if (xSemaphoreTake(variables, portMAX_DELAY))
		{
			/*Sends values to matlab from integer to ascii*/
			itoa(meassure_val, str, 10);
			printf(str);
			printf("\n");
			
			itoa(error_val, str, 10);
			printf(str);
			printf("\n");
			
			itoa(out_val, str, 10);
			printf(str);
			printf("\n");
			xSemaphoreGive(variables);
		}
		/* Delay task for some time*/
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}