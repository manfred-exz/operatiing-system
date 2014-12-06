#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int counter = 0; /* shared varible */

void thread1(void *arg);
void thread2(void *arg);

int main(int argc, char * argv[])
{
	pthread_t id1, id2;
	pthread_create(&id1, NULL, (void *) thread1, NULL);
	pthread_create(&id2, NULL, (void *) thread2, NULL);

	pthread_join(id1, NULL); /* wait for the threads */
	pthread_join(id2, NULL); /* wait for the threads */

	printf("The final value of counter is %d\n", counter);

	exit(0);
}

void thread1(void *arg){
	printf("thread1's id is %lu\n", pthread_self());
	int i, val;
	for(i = 1; i <= 5; i++){
		printf("[thread1, loop%d]Entering loop\n", i);
		val = ++counter;
		printf("[thread1, loop%d]\tcounter value++, and saved as val:%d\n",i, val);

		printf("[thread1, loop%d] \t\tfirst ref: counter = %d\n", i, counter);
		printf("[thread1, loop%d]\tEntering 800us wait.\n", i);
		usleep(800);
		printf("[thread1, loop%d] \t\tsecond ref: counter = %d\n", i, counter);

		counter = val;
		printf("[thread1, loop%d]\tcounter value restored to val:%d\n",i, val);
	}
}

void thread2(void *arg){
	int i, val;
	for(i = 1; i <= 5; i++){
		printf("[thread2, loop%d]\t\t\t\t\t\tEntering loop\n", i);
		val = ++counter;
		printf("[thread2, loop%d]\t\t\t\t\t\t\tcounter value++, and saved as val:%d\n",i, val);

		printf("[thread2, loop%d]\t\t\t\t\t\t\tEntering 100us wait.\n", i);
		usleep(100);
		printf("[thread2, loop%d]\t\t\t\t\t\t\tcounter = %d\n", i, counter);

		counter = val;
		printf("[thread2, loop%d]\t\t\t\t\t\t\tcounter value restored to val:%d\n",i, val);
	}
}
