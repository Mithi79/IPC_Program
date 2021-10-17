#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

sem_t producer,consumer;

int shared_val=0;

void *write_func(void *arg)
{
	for(int i=0;i<10;i++)
	{
		sem_wait(&consumer);
		shared_val++;
		sem_post(&producer);
	}
}
void *read_func(void *arg)
{
	for(int i=0;i<10;i++)
	{
		sem_wait(&producer);
		printf("shared_val=%d\n",shared_val);
		sem_post(&consumer);
	}
}

int main()
{
	int thread_err;
	pthread_t th_id1,th_id2;

	if(sem_init(&producer,0,0))
	{
		perror("sem_init");
		exit(1);
	}
	if(sem_init(&consumer,0,1))
	{
		perror("sem_init");
		exit(1);
	}

	thread_err = pthread_create(&th_id1,NULL,write_func,NULL);
	if(thread_err !=0)
	{
		perror("pthread_create()");
		exit(1);
	}
	thread_err = pthread_create(&th_id2,NULL,read_func,NULL);
	if(thread_err !=0)
	{
		perror("pthread_create()");
		exit(1);
	}
	pthread_join(th_id1,NULL);
	pthread_join(th_id2,NULL);

}
