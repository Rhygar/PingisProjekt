/*
 * variables.h
 *
 * Created: 2016-03-13 13:27:01
 *  Author: Andreas & John
 */ 


#ifndef VARIABLES_H_
#define VARIABLES_H_

extern xSemaphoreHandle variables;
extern uint16_t adc_val_in_mm[];
extern uint16_t set_val;
extern uint16_t out_val;
extern uint16_t meassure_val;
extern int error_val;
extern float p_varde;
extern float i_varde;
extern float d_varde;
extern uint16_t timer;
extern int offset;

#define TASK_COM_STACKSIZE	(2048/sizeof(portSTACK_TYPE))
#define  LINJAR_BUFFER_LENGTH 10
#define  LINJAR_ARRAY 100
#endif /* VARIABLES_H_ */