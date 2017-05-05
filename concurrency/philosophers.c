#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>
#include <unistd.h>

typedef int bool;

#define true 1
#define false 0

typedef struct dude{
    int num_forks;
    int days_gone_hangry;
    bool will_eat;
} dude;

typedef struct philosophers{
    dude jesus_of_nazareth;
    dude prophet_muhammad;
    dude confucious;
    dude sam_harris;
    dude abraham;
} philosophers;

void think(philosophers religious_dudes);
void get_forks(philosophers religious_dudes);
void eat(philosophers religious_dudes);
void put_forks(philosophers religious_dudes);
void print_status(philosophers religious_dudes);

//openMP split into threads to ensure the putting and getting of forks is correctly coordinated
//3 openMP barriers in each function will coordinate timings between threads

int main() {

    philosophers religious_dudes;
    religious_dudes.jesus_of_nazareth.num_forks = 0;

    religious_dudes.jesus_of_nazareth.num_forks = 0;
    religious_dudes.prophet_muhammad.num_forks = 0;
    religious_dudes.confucious.num_forks = 0;
    religious_dudes.sam_harris.num_forks = 0;
    religious_dudes.abraham.num_forks = 0;
    
    int days_past;
    days_past = 0;

    religious_dudes.jesus_of_nazareth.days_gone_hangry = 0;
    religious_dudes.prophet_muhammad.days_gone_hangry = 0;
    religious_dudes.confucious.days_gone_hangry = 0;
    religious_dudes.sam_harris.days_gone_hangry = 0;
    religious_dudes.abraham.days_gone_hangry = 0;
    
    religious_dudes.jesus_of_nazareth.will_eat = true;
    religious_dudes.prophet_muhammad.will_eat = false;
    religious_dudes.confucious.will_eat = false;
    religious_dudes.sam_harris.will_eat = true;
    religious_dudes.abraham.will_eat = false;
    
    int done;
    done = 0;

    omp_set_num_threads( 4 );
    do{
        #pragma omp parallel sections
        {
            #pragma omp section 
            {
                think(religious_dudes);
            }

            #pragma omp section
            {
                get_forks(religious_dudes);
            }

            #pragma omp section
            {
                eat(religious_dudes);
            }

            #pragma omp section
            {
                put_forks(religious_dudes);
            }
        }
	days_past++;
	printf("Days past: %d\n", days_past);
        printf("Keep running simulation? Type 0 for yes or 1 for no.\n");
        scanf("%d", &done);
    } while (!done);
    return 0;
}

void think(philosophers religious_dudes) {
    //think
    //rotate who is eating
    printf("Thinking time.\n");
    if(religious_dudes.jesus_of_nazareth.will_eat) {
        religious_dudes.prophet_muhammad.will_eat = true;
        religious_dudes.jesus_of_nazareth.will_eat = false;
    }
    if(religious_dudes.prophet_muhammad.will_eat) {
        religious_dudes.confucious.will_eat = true;
        religious_dudes.prophet_muhammad.will_eat = false;
    }
    if(religious_dudes.confucious.will_eat) {
        religious_dudes.sam_harris.will_eat = true;
        religious_dudes.confucious.will_eat = false;
    }
    if(religious_dudes.sam_harris.will_eat) {
        religious_dudes.abraham.will_eat = true;
        religious_dudes.sam_harris.will_eat = false;
    }
    if(religious_dudes.abraham.will_eat) {
        religious_dudes.jesus_of_nazareth.will_eat = true;
        religious_dudes.abraham.will_eat = false;
    }
    srand(time(NULL));
    sleep((rand() % 20) + 1);//rand time between 1 and 20 seconds
    
    //print fork locations
    print_status(religious_dudes);

    #pragma omp barrier
    #pragma omp barrier
    #pragma omp barrier
}

void get_forks(philosophers religious_dudes) {
    #pragma omp barrier
    //get forks
    printf("Fork picking up time.\n");
    if(religious_dudes.jesus_of_nazareth.will_eat)
        religious_dudes.jesus_of_nazareth.num_forks = 2;
    if(religious_dudes.prophet_muhammad.will_eat)
        religious_dudes.prophet_muhammad.num_forks = 2;
    if(religious_dudes.confucious.will_eat)
        religious_dudes.confucious.num_forks = 2;
    if(religious_dudes.sam_harris.will_eat)
        religious_dudes.sam_harris.num_forks = 2;
    if(religious_dudes.abraham.will_eat)
        religious_dudes.abraham.num_forks = 2;

    //print fork locations
    print_status(religious_dudes);

    #pragma omp barrier
    #pragma omp barrier
}

void eat(philosophers religious_dudes) {
    #pragma omp barrier
    #pragma omp barrier
    //eat
    //first prove that no more than four forks are in use
    printf("Eating time.\n");
    int forks_on_table = 5 - (religious_dudes.jesus_of_nazareth.num_forks + religious_dudes.prophet_muhammad.num_forks + religious_dudes.confucious.num_forks + religious_dudes.sam_harris.num_forks + religious_dudes.abraham.num_forks);

    if(religious_dudes.jesus_of_nazareth.num_forks < 2) {
        religious_dudes.jesus_of_nazareth.days_gone_hangry++;
    }
    else {
        religious_dudes.jesus_of_nazareth.days_gone_hangry = 0;
    }
    if(religious_dudes.prophet_muhammad.num_forks < 2) {
        religious_dudes.prophet_muhammad.days_gone_hangry++;
    }
    else {
        religious_dudes.prophet_muhammad.days_gone_hangry = 0;
    }
    if(religious_dudes.confucious.num_forks < 2) {
        religious_dudes.confucious.days_gone_hangry++;
    }
    else {
        religious_dudes.confucious.days_gone_hangry = 0;
    }
    if(religious_dudes.sam_harris.num_forks < 2) {
        religious_dudes.sam_harris.days_gone_hangry++;
    }
    else {
        religious_dudes.sam_harris.days_gone_hangry = 0;

    }
    if(religious_dudes.abraham.num_forks < 2) {
        religious_dudes.abraham.days_gone_hangry++;
    }
    else {
        religious_dudes.abraham.days_gone_hangry = 0;
    }

    //print fork locations
    print_status(religious_dudes);
    
    sleep((rand() % 8) + 2);//rand time between 2 and 9 seconds

    #pragma omp barrier
}

void put_forks(philosophers religious_dudes) {
    #pragma omp barrier
    #pragma omp barrier
    #pragma omp barrier

    //put forks
    printf("Fork putting down time.\n");
    religious_dudes.jesus_of_nazareth.num_forks = 0;
    religious_dudes.prophet_muhammad.num_forks = 0;
    religious_dudes.confucious.num_forks = 0;
    religious_dudes.sam_harris.num_forks = 0;
    religious_dudes.abraham.num_forks = 0;

    //print fork locations
    print_status(religious_dudes);
}

void print_status(philosophers religious_dudes) {
    printf("Forks on the table: %d\n", 5 - (religious_dudes.jesus_of_nazareth.num_forks + religious_dudes.prophet_muhammad.num_forks + religious_dudes.confucious.num_forks + religious_dudes.sam_harris.num_forks + religious_dudes.abraham.num_forks));
    printf("Jesus of Nazareth, savior of man, has %d forks.\n", religious_dudes.jesus_of_nazareth.num_forks);
    printf("Prophet Muhammad, peace be upon him, has %d forks.\n", religious_dudes.prophet_muhammad.num_forks);
    printf("Confucious, all wisdom to him, has %d forks.\n", religious_dudes.confucious.num_forks);
    printf("Sam Harris, the anti-liberal, has %d forks.\n", religious_dudes.sam_harris.num_forks);
    printf("Abraham, father of Isreal, has %d forks.\n", religious_dudes.abraham.num_forks);

    printf("Jesus of Nazareth, savior of man, has been hungry for %d days.\n", religious_dudes.jesus_of_nazareth.days_gone_hangry);
    printf("Prophet Muhammad, peace be upon him, has been hungry for %d days.\n", religious_dudes.prophet_muhammad.days_gone_hangry);
    printf("Confucious, all wisdom to him, has been hungry for %d days.\n", religious_dudes.confucious.days_gone_hangry);
    printf("Sam Harris, the anti-liberal, has been hungry for %d days.\n", religious_dudes.sam_harris.days_gone_hangry);
    printf("Abraham, father of Isreal, has been hungry for %d days.\n\n\n", religious_dudes.abraham.days_gone_hangry);

}
