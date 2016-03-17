/*
 * variables.h
 *
 * Created: 2016-03-13 13:27:01
 *  Author: Andreas & John
 */ 


#ifndef VARIABLES_H_
#define VARIABLES_H_

extern xSemaphoreHandle variables;
extern uint8_t adc_val_in_cm[];
extern uint16_t bor_varde;
extern uint16_t ut_varde;
extern uint16_t mat_varde;
extern uint16_t fel_varde;
extern float p_varde;
extern float i_varde;
extern float d_varde;
extern uint16_t timer;


#define  LINJAR_BUFFER_LENGTH 10
#define  LINJAR_ARRAY 100
#endif /* VARIABLES_H_ */