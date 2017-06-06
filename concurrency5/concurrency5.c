#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <pthread.h>
#include <semaphore.h>

// From the Little Book of Semaphores
// http://greenteapress.com/semaphores/downey08semaphores.pdf

#define UNUSED(x)  x __attribute__ ((unused))

#define EATING_TIME 4
#define RESTING_TIME 2

// typedef int bool;
// #define true 1
// #define false 0

sem_t tofu, bread, BBQSauce, agent_semaphore, tofu_semaphore, bread_semaphore, BBQSauce_semaphore;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int loaves = 0,  bottles = 0, tofu_blocks = 0;

void eat_sandwich(void) {//remove
	sleep(EATING_TIME);
}

void *bread_baker(void *UNUSED(_)) {
	while (true) {
		printf("Baker is waiting to eat.\n");
		sem_wait(&bread_semaphore);
		printf("Baker is eating.\n");
		eat_sandwich();
		printf("Baker is done eating.\n\n");
		sem_post(&agent_semaphore);
	}
}

void *BBQSauce_mixer(void *UNUSED(_)) {
	while (true) {
		printf("BBQSauce mixer is waiting to eat.\n");
		sem_wait(&BBQSauce_semaphore);
		printf("BBQSauce mixer is eating.\n");
		eat_sandwich();
		printf("BBQSauce is done eating.\n\n");
		sem_post(&agent_semaphore);
	}
}

void *tofu_coagulator(void *UNUSED(_)) {
	while (true) {
		printf("Tofu coagulator is waiting to eat.\n");
		sem_wait(&tofu_semaphore);
		printf("Tofu coagulator is eating.\n");
		eat_sandwich();
		printf("Tofu coagulator is done eating.\n\n");
		sem_post(&agent_semaphore);
	}
}

void sauceless_agent(void) {
	sem_wait(&agent_semaphore);
	printf("Agent puts tofu on the table.\n");
	sem_post(&tofu);
	printf("Agent puts bread on the table.\n");
	sem_post(&bread);
}

void tofuless_agent(void) {
	sem_wait(&agent_semaphore);
	printf("Agent puts bread on the table.\n");
	sem_post(&bread);
	printf("Agent puts BBQSauce on the table.\n");
	sem_post(&BBQSauce);
}

void breadless_agent(void) {
	sem_wait(&agent_semaphore);
	printf("Agent puts tofu on the table.\n");
	sem_post(&tofu);
	printf("Agent puts BBQSauce on the table.\n");
	sem_post(&BBQSauce);
}

void *tofuman_nudges(void *UNUSED(_)) {
	while (true) {
		sem_wait(&tofu);
		pthread_mutex_lock(&mutex);
		if (loaves) {
			loaves--;
			printf("Tofu nudger nudges BBQSauce mixer.\n");
			sem_post(&BBQSauce_semaphore);
		} else if (bottles) {
			bottles--;
			printf("Tofu nudger nudges baker.\n");
			sem_post(&bread_semaphore);
		} else {
			printf("Tofu nudger got tofu.\n");
			tofu_blocks++;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void *baker_nudges(void *UNUSED(_)) {
	while (true) {
		sem_wait(&bread);
		pthread_mutex_lock(&mutex);
		if (tofu_blocks) {
			tofu_blocks--;
			printf("Bread nudger nudges BBQSauce mixer.\n");
			sem_post(&BBQSauce_semaphore);
		} else if (bottles) {
			bottles--;
			printf("Bread nudger nudges tofu coagulator.\n");
			sem_post(&tofu_semaphore);
		} else {
			printf("Bread nudger got bread.\n");
			loaves++;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void *saucer_nudges(void *UNUSED(_)) {
	while (true) {
		sem_wait(&BBQSauce);
		pthread_mutex_lock(&mutex);
		if (tofu_blocks) {
			tofu_blocks--;
			printf("Sauce nudger nudges baker.\n");
			sem_post(&bread_semaphore);
		} else if (loaves) {
			loaves--;
			printf("Sauce nudger nudges tofu coagulator.\n");
			sem_post(&tofu_semaphore);
		} else {
			printf("Sauce nudger got BBQSauce.\n");
			bottles++;
		}
		pthread_mutex_unlock(&mutex);
	}
}

void *agent_main(void *UNUSED(_)) {
	void (*agents[])(void) = {sauceless_agent, tofuless_agent, breadless_agent};
	while (true) {
		agents[rand() % 3]();
		sleep(RESTING_TIME);
	}
}

int main() {
	srand(time(NULL));//seed random number generator

	printf("This concurrency demonstration applies the same concurrency concepts as \"The Smokers Problem\", but uses a medium of sandwich-making instead of cigarette-making to encourage healthy habits.\n\n");

	pthread_t agent_thr, tofuman_nudges_thr, baker_nudges_thr, saucer_nudges_thr, baker_thr, mixer_thr, coagulator_thr;

	//create semaphores
	sem_init(&tofu, 0, 0);
	sem_init(&bread, 0, 0);
	sem_init(&BBQSauce, 0, 0);
	sem_init(&agent_semaphore, 0, 1);
	sem_init(&tofu_semaphore, 0, 0);
	sem_init(&BBQSauce_semaphore, 0, 0);
	sem_init(&bread_semaphore, 0, 0);

	//create threads
	pthread_create(&agent_thr, NULL, agent_main, NULL);
	pthread_create(&baker_thr, NULL, bread_baker, NULL);
	pthread_create(&mixer_thr, NULL, BBQSauce_mixer, NULL);
	pthread_create(&coagulator_thr, NULL, tofu_coagulator, NULL);
	pthread_create(&tofuman_nudges_thr, NULL, tofuman_nudges, NULL);
	pthread_create(&baker_nudges_thr, NULL, baker_nudges, NULL);
	pthread_create(&saucer_nudges_thr, NULL, saucer_nudges, NULL);

	//wait for the agent to terminate
	pthread_join(agent_thr, NULL);
}