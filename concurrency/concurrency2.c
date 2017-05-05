#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

#define NUM_PHILOSOPHERS 5

typedef int bool;
#define true 1
#define false 0

static omp_lock_t forks[NUM_PHILOSOPHERS];

void philosopher() {

	#pragma omp barrier
	
	int id = omp_get_thread_num();
	int left_fork;
	int right_fork;

	//1 to 20 seconds of thinking
	srand(time(NULL));
	sleep((rand() % 20) + 1);
	
	if (id < NUM_PHILOSOPHERS - 1) {
		left_fork = id + 1;
		right_fork = id;
	}
	
	else {
		left_fork = id;
		right_fork = 0;
	}
	
	omp_set_lock(&forks[left_fork]);
	omp_set_lock(&forks[right_fork]);

	printf("Philosopher with id %d is eating today.\n", id);	
	printf("In his left hand is fork %d and in his right hand is fork %d.\n", left_fork, right_fork);

	sleep((rand() % 8) + 2); //Eating is between 2 and 9 seconds
	
	printf("Philosopher with id %d is full for now.\n", id);
	
	omp_unset_lock(&forks[left_fork]);
	omp_unset_lock(&forks[right_fork]);
	printf("Forks %d and %d, were put on the table.\n\n", left_fork, right_fork);
}

//openMP splits into threads to coordinate fork acquisition/placement
//3 barriers coordinate timings between threads

main() {
	int i;
	int done;
	done = 0;
	
	for (i = 0; i < NUM_PHILOSOPHERS; i++)
		omp_init_lock(&forks[i]);
	
	do {
		printf("Thinking time!\n");
		#pragma omp parallel num_threads(NUM_PHILOSOPHERS)
		{
			philosopher();
		}
		printf("Continue running? Type 0 to continue or 1 to quit\n\n");
		scanf("%d", &done);
	}while (!done);
	
	
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		omp_destroy_lock(&forks[i]);
	}
	return 0;