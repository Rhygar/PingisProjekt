/*
 * ad_conv.c
 *
 * Created: 2016-03-12 15:31:55
 *  Author: John Tengvall & Andreas Langhammer
 */ 
#include <asf.h>
#include "ad_conv.h"

void adc_setup(void)
{
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), 20000000, 0);
	adc_configure_timing(ADC, 0, 0, 0);
	adc_set_resolution(ADC, ADC_MR_LOWRES);
	adc_enable_channel(ADC, ADC_CHANNEL_10);
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);
}