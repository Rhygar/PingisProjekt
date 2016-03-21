/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
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
uint16_t vat_varde = 0;
uint16_t adc_val_in_mm[LINJAR_ARRAY] = {0};
uint16_t timer;
int offset = 500;


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
