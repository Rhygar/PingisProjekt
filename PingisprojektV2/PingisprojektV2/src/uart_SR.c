/*
 * uart_SR.c
 *
 * Created: 2016-03-12 17:41:31
 *  Author: Andreas & John
 */ 

#include <asf.h>
#include "uart_SR.h"
#include "variables.h"

void matlab_values(void){
	// linjarBuffer receives first 100 values from matlab
 	uint16_t linjarBuffer [LINJAR_BUFFER_LENGTH] = {0};
 	
	int i = 0;
 	while (i < LINJAR_ARRAY)
 	{
		gets(linjarBuffer);
		adc_val_in_cm[i] = atoi(linjarBuffer);
		i++; 
 	}
 	
 	gets(linjarBuffer);
 	p_varde = atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	i_varde = atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	d_varde = atoi(linjarBuffer)/1000;
 	
 	gets(linjarBuffer);
 	bor_varde = atoi(linjarBuffer);
	 
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