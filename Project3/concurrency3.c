#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <time.h>

#include "linkedlist.h"

//define the specifications of the run here
#define NUM_SEARCHERS 10
#define NUM_INSERTERS 15
#define NUM_DELETERS 5

void * searcher();
void * inserter();
void * deleter();

node_t * list = NULL;
sem_t reading_semaphore, writing_semaphore;

int main() {
	srand(time(NULL));

	int create_check;
	create_check = 0;
	int i;

	pthread_t searchers[NUM_SEARCHERS];
	pthread_t inserters[NUM_INSERTERS];
	pthread_t deleters[NUM_DELETERS];

	sem_init(&reading_semaphore, 1, NUM_SEARCHERS);
	sem_init(&writing_semaphore, 0, 1);


	pid_t searcher_ids[100];
	pid_t inserter_ids[100];
	pid_t deleter_ids[100];

	for (i = 0; i < NUM_INSERTERS; i++) {
		create_check = pthread_create(&inserters[i], NULL, inserter, NULL);
		//get and save thread ID here
		if (create_check) {
			printf("pthread_create() failed with error code %d\n", create_check);
			exit(-1);
		}
	}

	for(i = 0; i < NUM_SEARCHERS; i++) {
		create_check = pthread_create(&searchers[i], NULL, searcher, NULL);
		//get and save thread ID here
		if(create_check){
			printf("pthread_create() failed with error code %d\n", create_check);
			exit(-1);
		}
	}

	for(i = 0; i < NUM_DELETERS; i++) {
		create_check = pthread_create(&deleters[i], NULL, deleter, NULL);
		//get and save thread ID here
		if(create_check){
			printf("pthread_create() failed with error code %d\n", create_check);
			exit(-1);
		}
	}

	for (i = 0; i < NUM_INSERTERS; i++) {
		pthread_join(inserters[i], NULL);
	}

	for (i = 0; i < NUM_SEARCHERS; i++) {
		pthread_join(searchers[i], NULL);
	}

	for (i = 0; i < NUM_DELETERS; i++) {
		pthread_join(deleters[i], NULL);
	}

	sem_destroy(&reading_semaphore);
	sem_destroy(&writing_semaphore);

	free_list(&list);
	exit(0);
}

//outputs need to be spaced a little more with extra line breaks where appropriate

void * searcher()
{
	sem_wait(&reading_semaphore);
	//print the id's of the threads that are blocked here
	int val = rand() % 100;
	printf("Searched for the value %d, result: %d\n (1: value found, 0: value not found)\n", val, search_by_value(list, val));
	pid_t tid = syscall(__NR_gettid);
	printf("Search was done by thread id %d.\n", tid);
	print_list(list);
	sem_post(&reading_semaphore);
}

void * inserter()
{
	int val;
	int i;

	sem_wait(&writing_semaphore);
	//print the id's of the threads that are blocked here
	for (i = 0; i < 5; i++) {
		val = rand() % 100;
		printf("Inserting the value %d\n", val);
		pid_t tid = syscall(__NR_gettid);
		printf("Insert was done by thread id %d.\n", tid);
		list = insert_node(list, val);
		print_list(list);
	}

	sem_post(&writing_semaphore);
}

void * deleter()
{
	int val = rand() % 100;
	int i;

	sem_wait(&writing_semaphore);
	
	//print the id's of the threads that are blocked here
	for(i = 0; i < NUM_SEARCHERS; i++){
		sem_wait(&reading_semaphore);
	}

	printf("removing %d\n", val);
	pid_t tid = syscall(__NR_gettid);
	printf("Deletion was done by thread id %d.\n", tid);
	list = remove_node(list, val);
	print_list(list);

	for(i = 0; i < NUM_SEARCHERS; i++){
		sem_post(&reading_semaphore);
	}
	sem_post(&writing_semaphore);
}
