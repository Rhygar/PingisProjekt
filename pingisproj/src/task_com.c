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
	uint8_t string[LINJAR_BUFFER_LENGTH] = {0};
	//char buffer[50] = {0};
	
	while(1)
	{
		if (xSemaphoreTake(variables, portMAX_DELAY))
		{
			itoa(mat_varde, string, 10);
			printf(string);
			printf("\n");
			
			itoa(fel_varde, string, 10);
			printf(string);
			printf("\n");
			
			itoa(ut_varde, string, 10);
			printf(string);
			printf("\n");
			xSemaphoreGive(variables);
		}
		//gets(buffer);
		//testPWMVal = atoi (buffer);
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);

	}
}