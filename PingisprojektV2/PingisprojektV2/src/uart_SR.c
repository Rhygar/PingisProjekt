/*
 * uart_SR.c
 * 
 * Receives the values sent from matlab.
 * First the array of 100 elements then the other
 * values that are needed to regulate.
 * The function configure console is used to make the serial communication work properly.
 * Created: 2016-03-12 17:41:31
 * Author: John Tengvall and Andreas Langhammer
 */ 

#include <asf.h>
#include "uart_SR.h"
#include "variables.h"

void matlab_values(void){
	
 	uint16_t linjarBuffer [LINJAR_BUFFER_LENGTH] = {0};
 	/*linjarBuffer receives first 100 values from matlab*/
	int i = 0;
 	while (i < LINJAR_ARRAY)
 	{
		gets(linjarBuffer);
		adc_val_in_mm[i] = atoi(linjarBuffer);
		i++; 
 	}
 	/* Receives P,I,D,set_val and timer-values*/
 	gets(linjarBuffer);
 	p_varde = (float) atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	i_varde =(float) atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	d_varde =(float) atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	set_val = atoi(linjarBuffer);
	 
	gets(linjarBuffer);
	timer = atoi(linjarBuffer);
}

void configure_console(void)
/* Enables feedback through the USB-cable back to terminal within Atmel Studio */
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}