/*
 * pwm_io.h
 *
 * Created: 2016-03-13 14:58:13
 * Author: Andreas Langhammer and John Tengvall
 */ 


#ifndef PWM_IO_H_
#define PWM_IO_H_
/*sets the frequency on pwm*/
#define PWM_FREQUENCY		1000
/*Sets the resolution for the pwm*/
#define PWM_RESOLUTION		999
#define PWM_PIO				PIOB
#define PWM_PIO_PERIPHERAL	PIO_PERIPH_B
#define PWM_PIO_PIN			PIO_PB16B_PWML0	
/*Initilize pwm*/
int pwm_setup(void);

void pwm_set_duty_cycle(uint32_t dutycycle);




#endif /* PWM_IO_H_ */