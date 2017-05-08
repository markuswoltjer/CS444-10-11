#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>
#include "mt19937ar.c"

typedef int bool;
#define true 1
#define false 0
#define NUM_PHILOSOPHERS 5

int Forks[5];
static omp_lock_t forks[NUM_PHILOSOPHERS];

//add five philoshers global array here

void philosopher() {
	#pragma omp barrier
	
	
	
	int id = omp_get_thread_num();
	int left_fork;
	int right_fork;
	
	srand(time(NULL));
	
	sleep((rand() % 20) + 1); //Waiting/Thinking is between 1 and 20 seconds
	
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
	
	Forks[left_fork] = 0;
	Forks[right_fork] = 0;
	
	printf("forks %d and %d were picked up\n", left_fork, right_fork);
	
	printf("Fork status: 0 = unavailable, 1 = available\n");
	printf("Fork 0: %d Fork 1: %d Fork 2: %d Fork 3: %d Fork 4: %d\n\n", Forks[0], Forks[1], Forks[2], Forks[3], Forks[4]);
	
	printf("philosopher %d is eating\n\n", id);

	sleep((rand() % 8) + 2); //Eating is between 2 and 9 seconds
	
	printf("philosopher %d has finished eating\n", id);
	
	omp_unset_lock(&forks[left_fork]);
	omp_unset_lock(&forks[right_fork]);
	
	printf("forks %d and %d, were put on the table\n\n", left_fork, right_fork);
	
	Forks[left_fork] = 1;
	Forks[right_fork] = 1;
	
	printf("Fork status: 0 = unavailable, 1 = available\n");
	printf("Fork 0: %d Fork 1: %d Fork 2: %d Fork 3: %d Fork 4: %d\n\n", Forks[0], Forks[1], Forks[2], Forks[3], Forks[4]);
	
}

//openMP splits into threads to coordinate fork acquisition/placement
//3 barriers coordinate timings between threads

main() {
	int i;
	int done;
	done = 0;
	
	for (i = 0; i < 5; i++)
		Forks[i] = 1;
	
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
}