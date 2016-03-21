/**
 * Main file to declare all global variables, set all PINs for the use of the 
 * motorshield and distrebute a semaphore que.
 *
 * Author: Andreas Langhammer and John Tengvall
 */
#include <asf.h>
#include <stdlib.h>
#include "uart_SR.h"
#include "task_com.h"
#include "variables.h"
#include "task_reg.h"
#include "pwm_io.h"
#include "adc_io.h"

// for motorshield
#define CHECK_PIN		PIO_PB26_IDX
#define DIRECTION_PIN	PIO_PB27_IDX
#define PWM_PIN			PIO_PC22_IDX

xSemaphoreHandle variables = (xSemaphoreHandle) 1;

char str[100] = {0};
float p_varde;
float i_varde;
float d_varde;
uint16_t set_val;
uint16_t out_val = 0;
uint16_t meassure_val = 0;
int error_val = 0;
uint16_t adc_val_in_mm[LINJAR_ARRAY] = {0};
uint16_t timer;
int offset = 500;
/************************************************************************/
/* 
Function int main() 
Start to initialize all reacquired components then setting pin_levels and
direction of pins for the motorshield.
Calls matlab_values() to start receiving values from matlab, 
then starts a que for the tasks.
Setting tasks priority	and start the schedular.											
																	    */
/************************************************************************/

int main (void)
{
	sysclk_init();
	board_init();
	ioport_init();
	configure_console();
	adc_setup();
	pwm_setup();
	
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(PWM_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PWM_PIN, IOPORT_PIN_LEVEL_LOW);
	ioport_set_pin_dir(DIRECTION_PIN, IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(DIRECTION_PIN, IOPORT_PIN_LEVEL_HIGH);
	
	matlab_values();
	vSemaphoreCreateBinary(variables);
	
	xTaskCreate(task_com, (const signed char * const) "task_com", TASK_COM_STACKSIZE, NULL, 1, NULL);
	xTaskCreate(task_reg, (const signed char * const) "task_reg", TASK_COM_STACKSIZE, NULL, 2, NULL); 
	vTaskStartScheduler();
}
