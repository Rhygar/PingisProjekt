/*
 * task_com.c
 *
 * Created: 2016-03-13 13:06:56
 *  Author: Andreas & John
 */ 
#include <asf.h>
#include "variables.h"

void task_com(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	
	xLastWakeTime = xTaskGetTickCount();
	uint16_t str[LINJAR_BUFFER_LENGTH] = {0};
	
	
	while(1)
	{
		if (xSemaphoreTake(variables, portMAX_DELAY))
		{
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
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);

	}
}