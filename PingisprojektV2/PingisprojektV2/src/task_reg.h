/*
 * task_reg.h
 *
 * Created: 2016-03-13 13:59:14
 * Author: John Tengvall and Andreas Langhammer
 */ 


#ifndef TASK_REG_H_
#define TASK_REG_H_

void task_reg(void *pvParameters);
int filter(int);
void reg_PID(uint16_t adc_val);
#define TASK_COM_STACKSIZE	(2048/sizeof(portSTACK_TYPE))
#define NEWLINE				10


#endif /* TASK_REG_H_ */