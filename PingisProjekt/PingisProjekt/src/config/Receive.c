/*
 * Receive.c
 *
 * Created: 2016-03-10 12:40:31
 *  Author: Andreas
 */ 
#include <asf.h>
#include "Receive.h"

void matlab_values(void)
{
	/* store string from matlab in buffer*/
	uint8_t recBuff[]={0};
	while(1)
	{
	gets(recBuff);
	printf(recBuff);
	}
}