#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// structure for message
struct msg{
    int index;
    time_t time;
    pthread_t thread_id;
};

#define BUFFER_SIZE 3
struct msg buffer[BUFFER_SIZE];

// structure for lock
int n = 3;
sem_t mutex, empty, full;

#define PRODUCER_NUM 3
#define PRODUCER_NUM_PER_THREAD 5
#define CONSUMER_NUM 5
#define CONSUMER_NUM_PER_THREAD 3
void producer_thread(void *arg);
void consumer_thread(void *arg);

int main(int argc, char * argv[]){

    printf("before init\n");
    // initializing semaphore
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, (unsigned)n);
    sem_init(&full, 0, 0);

    printf("init\n");

    pthread_t consumer_tid[CONSUMER_NUM], producer_tid[PRODUCER_NUM];
    int i;
    // Create producer threads
    for(i = 0; i < PRODUCER_NUM; i++){
        pthread_create(&producer_tid[i], NULL, (void *)producer_thread, NULL);
    }
    // Create consumer threads
    for(i = 0; i < CONSUMER_NUM; i++){
        pthread_create(&consumer_tid[i], NULL, (void *)consumer_thread, NULL);
    }

    printf("All producers & consumers created.\n");
    fflush(stdout);

    // Join producer threads
    for(i = 0; i < PRODUCER_NUM; i++){
        pthread_join(producer_tid[i], NULL);
    }
    // Join consumer threads
    for(i = 0; i < CONSUMER_NUM; i++){
        pthread_join(consumer_tid[i], NULL);
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
    printf("[Enter] producer thread with thread-id: %lu\n", pthread_self());
    fflush(stdout);

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
        printf("[Produce]A new msg.\n");
    }
}
void consumer_thread(void *arg){
    printf("\t\t\t\t\t\t\t\t\t\t[Enter] consumer thread with thread-id: %lu\n", pthread_self());
    fflush(stdout);
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
        printf("\t\t\t\t\t\t\t\t\t\t[Receiver]The time is %lu, the tid is %lu, the index is %u\n", tmp_msg.time, tmp_msg.thread_id, tmp_msg.index);
    }
}

