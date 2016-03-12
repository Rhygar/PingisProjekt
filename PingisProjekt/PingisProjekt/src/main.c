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
#include "ad_conv.h"
#include "pw_mod.h"
#include "recieveValue.h"
#include "FreeRTOS.h"

xSemaphoreHandle signal_semafor = 1;

int main (void)
{
	
	sysclk_init();
	board_init();
	ioport_init();
	adc_config();
	pwm_setup();
	
	ioport_set_pin_dir(PIO_PC22_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PC22_IDX,LOW);
	ioport_set_pin_dir(PIO_PB27_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PB27_IDX,HIGH);
	
	vSemaphoreCreateBinary(signal_semafor);
	matlab_values();
	//L�gg till kod med taskcreate
		xTaskCreate(task_valueMatlab, (const signed char * const) "task_valueMatlab", TASK_STACKSIZE, NULL, 2, NULL);
		xTaskCreate(task_regulator, (const signed char * const) "task_regulator", TASK_STACKSIZE,NULL,2,NULL);
	vTaskStartScheduler();
	
	
	
}
