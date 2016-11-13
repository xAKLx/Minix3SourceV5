#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <sys/time.h>
#include "mproc.h"

int sem_create(int id)
{
	return 3;

}

int sem_terminate(int id)
{
	return 4;
}

int sem_down(int id)
{
	return 5;
}

int sem_up(int id)
{
	return 6;
}
