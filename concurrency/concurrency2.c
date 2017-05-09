#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <omp.h>

typedef int bool;
#define true 1
#define false 0
#define NUM_PHILOSOPHERS 5

int Forks[5];
int Philosophers[5];
int has_eaten[5];

static omp_lock_t forks[NUM_PHILOSOPHERS];

//Pause for a random amount of time between 1 and 20 seconds
void think() {
	printf("Philosophers are thinking about who should eat.\n");
	sleep((rand() % 20) + 1);
}

//Pause for a random amount of time between 2 and 9 seconds
void eat() {
	sleep((rand() % 8) + 2);
}

void philosopher() {
	#pragma omp barrier
	
	int id = omp_get_thread_num();
	int left;
	int right;
	
	srand(time(NULL));
		
	if (id < NUM_PHILOSOPHERS - 1) {
		left = id + 1;
		right = id;
	}
	
	else {
		left = id;
		right = 0;
	}
	
	omp_set_lock(&forks[left]);
	omp_set_lock(&forks[right]);
	
	Forks[left] = 0;
	Forks[right] = 0;
	Philosophers[id] = 1;
	has_eaten[id] = 1;

	printf("Forks #%d and #%d were picked up by philosopher #%d.\n\n", left, right, id);
	
	printf("In the following fork status description, 0 indicates the fork is in use and 1 indicates the fork is on the table.\n");
	printf("Fork #0 is %d, Fork #1 is %d, Fork #2 is %d, Fork #3 is %d, Fork #4 is %d.\n\n", Forks[0], Forks[1], Forks[2], Forks[3], Forks[4]);

	printf("In the following philosopher status description, 0 indicates that the philosopher is not eating and 1 indicates that the philosopher is eating.\n");
	printf("Philosopher #0 is %d, Philosopher #1 is %d, Philosopher #2 is %d, Philosopher #3 is %d, Philosopher #4 is %d.\n\n", Philosophers[0], Philosophers[1], Philosophers[2], Philosophers[3], Philosophers[4]);
	
	eat();
	
	printf("Philosopher #%d has finished eating.\n", id);
	
	omp_unset_lock(&forks[left]);
	omp_unset_lock(&forks[right]);
	
	printf("Forks #%d and #%d, are put down.\n\n", left, right);
	
	Forks[left] = 1;
	Forks[right] = 1;
	Philosophers[id] = 0;
	
	printf("In the following fork status description, 0 indicates the fork is in use and 1 indicates the fork is on the table.\n");
	printf("Fork #0 is %d, Fork #1 is %d, Fork #2 is %d, Fork #3 is %d, Fork #4 is %d.\n\n", Forks[0], Forks[1], Forks[2], Forks[3], Forks[4]);

	printf("In the following philosopher status description, 0 indicates that the philosopher is not eating and 1 indicates that the philosopher is eating.\n");
	printf("Philosopher #0 is %d, Philosopher #1 is %d, Philosopher #2 is %d, Philosopher #3 is %d, Philosopher #4 is %d.\n\n", Philosophers[0], Philosophers[1], Philosophers[2], Philosophers[3], Philosophers[4]);
	
}

int main() {
	int finished;
	finished = 0;
	
	int i;

	//forks all begin available
	for(i = 0; i < 5; i++)
		Forks[i] = 1;

	//philosophers all begin without forks
	for(i = 0; i < 5; i++)
		Philosophers[i] = 0;

	//philosophers have not yet eaten
	for(i = 0; i < 5; i++)
		has_eaten[i] = 0;

	for(i = 0; i < NUM_PHILOSOPHERS; i++)
		omp_init_lock(&forks[i]);
	
	do{
		#pragma omp parallel num_threads(NUM_PHILOSOPHERS)
		{
			philosopher();
		}
		printf("The philosophers who have eaten are as follows, 1 representing having eaten, and 0 representing still hungry.\n");
		printf("Philosopher #1: %d, Philosopher #2: %d,Philosopher #3: %d, Philosopher #4: %d, Philosopher #5: %d.\n\n", has_eaten[0], has_eaten[1], has_eaten[2], has_eaten[3], has_eaten[4]);
		printf("Keep eating? Enter 0 to eat, 1 to finish simulation.\n");
		scanf("%d", &finished);
	}while(!finished);
	
	for (i = 0; i < NUM_PHILOSOPHERS; i++) {
		omp_destroy_lock(&forks[i]);
	}
	return 0;
}
