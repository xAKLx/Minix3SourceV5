#include "pm.h"
#include <minix/callnr.h>
#include <minix/com.h>
#include <signal.h>
#include <sys/time.h>
#include "mproc.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef SEM_ARRAY_INIT
#define SEM_ARRAY_INIT
Sem *semArray[MAX_SEM] = {NULL};
#endif

void InitQueue(Queue *queue)
{
	queue->first = NULL;
	queue->last = NULL;
}

// To Enqueue an integer
void Enqueue(Queue *queue, pid_t procId) 
{
	Node* newNode = malloc(sizeof(Node));

	newNode->next = NULL;
	newNode->value = procId;
	
	if(queue->last == NULL)
	{
		queue->first = newNode;
		queue->last = newNode;
	}
	else
	{
		queue->last->next = newNode;
		queue->last = newNode;
	}
}

// To Dequeue an integer.
pid_t Dequeue(Queue *queue) 
{

	Node *toDequeue = queue->first;

	pid_t procId = toDequeue->value;

	queue->first = toDequeue->next;

	if(queue->first == NULL)
		queue->last = NULL;

	free(toDequeue);

	return procId;
}

void InitSem(Sem *sem)
{
	sem->value = 1;
	InitQueue(&(sem->process));
}

int sem_create()
{
	//printf("\nPid from sem = %d", getpid());
	//printf("\nDefault value of m1_i2: %d", m_in.m1_i2);

	int id = m_in.m1_i1;

	if(id < 1 || id > 30)
		return 1;

	if(semArray[id-1] == NULL)
	{
		Sem *sem = malloc(sizeof(Sem));
		InitSem(sem);

		semArray[id-1] = sem;
		return 0;
	}
	else
	{
		return -1;
	}
}

int sem_terminate()
{
	int id = m_in.m1_i1;

	if(id < 1 || id > 30)
		return 1;

	if(semArray[id-1] != NULL)
	{
		free(semArray[id-1]); 
		semArray[id-1] = NULL;
		return 0;
	}
	else
	{
		return -1;
	}
}

int sem_down()
{
	int id = m_in.m1_i1;
	int pid = m_in.m1_i2;

	if(id < 1 || id > 30)
		return -1;

	if(semArray[id-1]->value == 1)
	{
		semArray[id-1]->value = 0;
	}
	else
	{
		//block process
		printf("\nblocking process: %d\n",pid);
		kill(pid, SIGSTOP);
	}

	Enqueue(&(semArray[id-1]->process), pid);

	return 0;
}

int sem_up()
{
	int id = m_in.m1_i1;
	int pid = m_in.m1_i2;

	if(id < 1 || id > 30)
		return -1;

	//check if process calling id is the one using the sem
	if(semArray[id-1]->value == 0 && semArray[id-1]->process.first->value == pid)
	{
		
		//remove process of queue
		Dequeue(&(semArray[id-1]->process));

		if(semArray[id-1]->process.first == NULL)
		{
			semArray[id-1]->value = 1;
		}
		else
		{
			printf("\nunblocking process: %d\n",semArray[id-1]->process.first->value);
			kill(semArray[id-1]->process.first->value, SIGCONT);
		}

		return 0;
	}
	else
	{
		return 1;
	}
	
}
