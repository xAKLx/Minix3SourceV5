#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <sys/time.h>
#include "mproc.h"

#ifndef SEM_ARRAY_INIT
#define SEM_ARRAY_INIT
int myArray[10];
#endif

int sem_create()
{
	int id = m_in.m1_i1;
	
	for(int i = 0; i < 10; i++)
		printf("%d\n", myArray[i]);

	return 3;

}

int sem_terminate()
{
	int id = m_in.m1_i1;

	return 4;
}

int sem_down()
{
	int id = m_in.m1_i1;

	if(id >= 0 && id < 10)
		myArray[id] = 1;

	return 5;
}

int sem_up()
{
	int id = m_in.m1_i1;

	if(id >= 0 && id < 10)
		myArray[id] = 0;

	return 6;
}
