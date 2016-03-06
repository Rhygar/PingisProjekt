/*
 * pwm.h
 *
 * Created: 2016-03-06 13:01:36
 *  Author: John Tengvall
 */ 


#ifndef PWM_H_
#define PWM_H_

#define PWM_FREQUENCY	1000
#define PWM_RESOLUTION	255
#define PWM_PIO			PIOB
#define PWM_PERIPHERAL	PIO_PERIPH_B
#define PWM_PIN			PIO_PB16B_PWML0
#define PWM_POLARITY	PWM_LOW

void pwm_setup(void);
void pwm_set_duty(uint32_t);


#endif /* PWM_H_ */