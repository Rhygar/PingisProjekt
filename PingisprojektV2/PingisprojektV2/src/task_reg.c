/*
 * task_reg.c
 *
 * This task uses the analog value from the sensor to regulate
 * the ping-pong ball to the setpoint.
 *
 * Created: 2016-03-13 13:59:27
 * Author: Andreas Langhammer and John Tengvall
 */ 
#include <asf.h>
#include "variables.h"
#include "pwm_io.h"
#include "FIRkoef.h"
#include "task_reg.h"
/************************************************************************/
/* task_reg first filters the analog signal then calculates the PID-value
   among other thing also limits the control_val, the value sent to matlab.                                                                  */
/************************************************************************/
void task_reg(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = timer;
	xLastWakeTime = xTaskGetTickCount();	
	while(1)
	{
 		/*Start ADC and read the value */
 		adc_start(ADC);
 		while ((adc_get_status(ADC) & 0x1<<24) == 0);
		int adc_val = adc_get_latest_value(ADC);
		/* Call functions*/
		adc_val = filter(adc_val);
		reg_PID (adc_val);
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}

/* FIR-filter is used to reduce strange adc values from the sensor, combined with a analog lowpass filter*/
int filter(int adc_val)
{
	static int xbuff[M] ={0};
	float sum = 0;
	for (int i = M; i > 0; i--)
	{
		xbuff[i] = xbuff[i-1];
	}
	
	xbuff[0] = (float) adc_val;
	
	for (int i = 0; i < M; i++)
	{
		sum += (xbuff[i]*B[i]);
	}
	return (int) sum;
}


void reg_PID(uint16_t adc_val)
{
		/*------------To calculate the real distance in mm to the ball.---------------*/
		static int i_sum = 0;
		static int o_err = 0;
		const float dT = (float) timer/1000;
		int end_up = 125000;
		int index = (adc_val/10)-1;
		
		if (index < 0)
		{
			index = 0;
		}
		else if (index > LINJAR_ARRAY -2)
		{
			index = LINJAR_ARRAY - 2;
		}
		
		/* to get the values we might lose in "index" by calculating new datapoints between the element 
		in the location and the next location in the list and then convert the value to mm */
		/* New datapoints to get more realistic values. */
		int new_datapoints = ((adc_val_in_mm[index] - adc_val_in_mm[index+1]) *(adc_val % 10))/10; 
		/* the real distance. */
		int real_distance = adc_val_in_mm[index] - new_datapoints;
		/*---------------The regulation-process-------------------*/
		int n_err = set_val - real_distance;
		i_sum += n_err;
		if (i_sum > end_up)
		{
			i_sum = end_up;
		}
		else if (i_sum < (-end_up))
		{
			i_sum = (-end_up);
		}
		
		int m_err = o_err - n_err;
 		
		/*----------PID----------*/
		float PID_regler = (float) ((n_err) + ((i_sum*dT)/i_varde) + ((m_err/dT)*d_varde)) *(-p_varde); 
		int control_val = (int) (offset + PID_regler); //
		/*--------------------------------------*/ 	
 	 	
		if (control_val > 999)
		{
			control_val = 999;
		}
		else if(control_val < 0)
		{
 			control_val = 0;
 		}
			  
		/*-----------WRITE TO ARDUINO-----------*/
		/* Sends the control-value to matlab then gives away the semaphore*/
		pwm_set_duty_cycle(control_val);
		
		if (xSemaphoreTake(variables, portMAX_DELAY))
		{
			meassure_val = real_distance;
			error_val = n_err;
			out_val = control_val;
			xSemaphoreGive(variables);
		}
		/* The latest error becomes the old one*/
		o_err = n_err;		
}


