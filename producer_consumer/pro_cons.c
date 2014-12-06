#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

// structure for message
struct msg{
	unsigned int index;
	time_t time;
	pthread_t thread_id;	
};

#define BUFFER_SIZE 8
struct msg buffer[BUFFER_SIZE];

// structure for lock
int n;
sem_t mutex, empty, full;

#define PRODUCER_NUM 3
#define PRODUCER_NUM_PER_THREAD 5
#define CONSUMER_NUM 5
#define CONSUMER_NUM_PER_THREAD 3
void producer_thread(void *arg);
void consumer_thread(void *arg);

int main(int argc, char * argv[]){
	// initializing semaphore
	sem_init(&mutex, 0, 1);
	sem_init(&empty, 0, n);
	sem_init(&full, 0, 0);

	pthread_t consumer[CONSUMER_NUM],producer[PRODUCER_NUM];
	int i;
	// Create producer threads
	for(i = 0; i < PRODUCER_NUM; i++){
		pthread_create(&producer[i], NULL, (void *)producer_thread, NULL);
	}
	// Create consumer threads
	for(i = 0; i < CONSUMER_NUM; i++){
		pthread_create(&consumer[i], NULL, (void *)consumer_thread, NULL);
	}

	// Join producer threads
	for(i = 0; i < PRODUCER_NUM; i++){
		pthread_join(producer[i], NULL);
	}
	// Join consumer threads
	for(i = 0; i < CONSUMER_NUM; i++){
		pthread_join(consumer[i], NULL);
	}

	// destroy the semaphores
	sem_destroy(&mutex);
	sem_destroy(&full);
	sem_destroy(&empty);
	
	printf("exit.");

	exit(0);
}


void producer_thread(void *arg){
	pthread_t thread_id = pthread_self();
	printf("Enter producer thread with thread-id: %lu", pthread_self());
	int i;
	struct msg tmp_msg;
	int sem_value;
	for(i = 0; i < PRODUCER_NUM_PER_THREAD; i++){
		// create the current tmp_msg
		tmp_msg.time = time(NULL);
		tmp_msg.index = i;
		tmp_msg.thread_id = thread_id;
		// wait semaphore to produce a message
		sem_wait(&empty); /* wait for an empty buffer to produce. */
		sem_wait(&mutex); /* wait for an mutex to operate exclusively */
		//		add the tmp_msg into buffer
		sem_getvalue(&full, &sem_value);
		buffer[sem_value] = tmp_msg;
		sem_post(&mutex);
		sem_post(&full);
	}
}
void consumer_thread(void *arg){
	struct msg tmp_msg;
	int i;
	int sem_value;
	for(i = 0; i < CONSUMER_NUM_PER_THREAD; i++){
		// wait semaphore to consume a message
		sem_wait(&full);
		sem_wait(&mutex);
		sem_getvalue(&full, &sem_value);
		tmp_msg = buffer[sem_value];
		sem_post(&mutex);
		sem_post(&empty);

		// print the message got
		printf("The time is %lu, the tid is %lu, the index is %u", tmp_msg.time, tmp_msg.thread_id, tmp_msg.index);
	}
}
