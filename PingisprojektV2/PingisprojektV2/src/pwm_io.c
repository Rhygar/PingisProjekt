/*
 * pwm_io.c
 *
 * From previous lab
 * Created: 2016-03-13 14:57:58
 * Author: John Tengvall and Andreas Langhammer
 * 
 */ 
#include <asf.h>
#include "pwm_io.h"

static pwm_channel_t pwm_instance;

int pwm_setup(void)
{
	/*Enable pwmclock*/
	if (pmc_enable_periph_clk(ID_PWM) == 0) {
		pwm_clock_t pwm_clock = {
			.ul_clka = PWM_FREQUENCY * PWM_RESOLUTION,
			.ul_clkb = 0,
			.ul_mck = sysclk_get_cpu_hz()
		};
		pwm_init(PWM, &pwm_clock);
		pwm_instance.alignment = PWM_ALIGN_LEFT;
		pwm_instance.polarity = PWM_LOW;
		pwm_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
		pwm_instance.ul_period = PWM_RESOLUTION;
		pwm_instance.ul_duty = 0;
		pwm_instance.channel = PWM_CHANNEL_0;
		pwm_channel_init(PWM, &pwm_instance);
		pio_set_peripheral(PWM_PIO, PWM_PIO_PERIPHERAL, PWM_PIO_PIN);
		/*Enable PWM*/
		pwm_channel_enable(PWM, PWM_CHANNEL_0);
		pwm_set_duty_cycle(0);
	}

}

/* Set duty cycle of PWM signal*/
void pwm_set_duty_cycle(uint32_t duty_cycle)
{
	pwm_channel_update_duty(PWM, &pwm_instance, duty_cycle);
}
