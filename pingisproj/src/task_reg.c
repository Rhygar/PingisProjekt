/*
 * task_reg.c
 *
 * Created: 2016-03-13 13:59:27
 *  Author: Andreas
 */ 
#include <asf.h>
#include "variables.h"
#include "pwm_io.h"

void task_reg(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 60;
	xLastWakeTime = xTaskGetTickCount();
	
	char reg_buffer[50] = {0};
	int i_sum = 0;
	int p_regler = 0;
	int i_regler = 0;
	int d_regler = 0;
	int n_err = 0;
	int m_err = 0;
	int g_err = 0;
	
	while(1)
	{
 		/*Start ADC and read the value */
 		adc_start(ADC);
 		while ((adc_get_status(ADC) & 0x1<<24) == 0);
		int adc_val = adc_get_latest_value(ADC);
		
		/*------------To calculate the real distance in cm to the ball.---------------*/
		
		int index = adc_val/10;
		// get the value in mm
		int bas_val = adc_val_in_cm[index];
		/* to get the values we might lose in "index" 
		by calculating a diff between the element 
		in the location and the next location in the list */
		int diff = ((bas_val - adc_val_in_cm[index+1]) * (adc_val % 10)); 
		// the real distance.
		int real_distance = bas_val + diff;
		/*-------------------------------------------------------
		
		
		---------------The regulation-process-------------------*/
		n_err = bor_varde - real_distance;
		i_sum += n_err;
		m_err = n_err - g_err;
		/*----------P-REGULATOR----------*/
		float p_regler = (float) (n_err* -p_varde);
		
		float styrvarde = (float) p_regler;
// 		/*----------I-REGULATOR----------*/
// 		float i_regler = (float) ((i_sum*(timer/1000))/i_varde) * p_varde;
// 		/*----------D-REGULATOR----------*/
// 		float d_regler = (float) ((m_err/timer)*d_varde) * p_varde;
// 		/*----------PID----------*/
// 		float styrvarde = p_regler + i_regler + d_regler;
		
		g_err = n_err;
		 		
	 		if (styrvarde > 999)
	 		{
	 			styrvarde = 999;
	 		}
	 		else if(styrvarde < 0)
	 		{
	 			styrvarde = 0;
	 		}
		/*-----------WRITE TO ARDUINO-----------*/
		pwm_set_duty_cycle(styrvarde);
		
		if (xSemaphoreTake(variables, portMAX_DELAY))
		{
			mat_varde = real_distance;
			fel_varde = n_err;
			ut_varde = styrvarde;
			xSemaphoreGive(variables);
		}
		
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}