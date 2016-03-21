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
	static int xbuff[BL] ={0};
	float sum = 0;
	for (int i = BL; i > 0; i--)
	{
		xbuff[i] = xbuff[i-1];
	}
	
	xbuff[0] = (float) adc_val;
	
	for (int i = 0; i < BL; i++)
	{
		sum += (xbuff[i]*B[i]);
	}
	return (int) sum;
}


void reg_PID(uint16_t adc_val)
{
		/*------------To calculate the real distance in mm to the ball.---------------*/
		
		int index = min(max((adc_val/10) - 1, 0), LINJAR_ARRAY-2); 
		
		/* to get the values we might lose in "index" by calculating a diff between the element 
		in the location and the next location in the list and then convert the value to mm */
		int diff = adc_val_in_mm[index] - adc_val_in_mm[index+1];
		/* interpolation to get more realistic values. */
		int interpolation = (diff *(adc_val % 10))/10; 
		/* the real distance. */
		int real_distance = adc_val_in_mm[index] - interpolation;
		/*-------------------------------------------------------
		Sets integral windup to a max value if the ball gets stucked
		so the integral sum doesn't affect the system to much
		---------------The regulation-process-------------------*/
		
		static int i_sum = 0;
		static int o_err = 0;
		const float dT = (float) timer/1000;
		int itgr_wndp = 125000;
		int n_err = set_val - real_distance;
		i_sum += n_err;
		i_sum = max(min(i_sum, itgr_wndp), -itgr_wndp);
		int m_err = o_err - n_err;
		
		/*----------P-REGULATOR----------*/
		float p_regler = (float) (n_err * (-p_varde));
		
 		/*----------I-REGULATOR----------*/
 		float i_regler = (float) (((i_sum*dT)/i_varde) * (-p_varde));
 		
		/*----------D-REGULATOR----------*/
 		float d_regler = (float) ((m_err/dT)*d_varde) * (-p_varde);
 		
		/*----------PID----------*/
 		float PID_regler =  p_regler + i_regler + d_regler;
		int control_val = (int) (offset + PID_regler);
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


