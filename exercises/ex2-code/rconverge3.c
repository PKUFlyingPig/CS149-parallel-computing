// Example demonstrating barrier synchronization 

// Set VERBOSE to 1 to see what's going on
#ifndef VERBOSE
#define VERBOSE 0
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define MAXBATCH 128
#define PIPEDEPTH 128

// Generate uniformly distributed random value x with 0 <= x < 1.0
float uniform() {
    return (float) random() / RAND_MAX;
}

pthread_barrier_t barrier_value;
pthread_mutex_t mutex;

static void barrier() {
    pthread_barrier_wait(&barrier_value);
}

static void atomic_add(float *dest, float value) {
    pthread_mutex_lock(&mutex);
    *dest += value;
    pthread_mutex_unlock(&mutex);
}

pthread_t threads[MAXBATCH];

//// Global variables ////
// Read-only
float target_average;         // Convergence goal
int batch_size;               // Number in batch
// Read-write  
float phase_sum[PIPEDEPTH];  // Sum of all values in current phase
// Only written by single thread
long pcount = 0;              // Number of phases required

//// Thread procedure ////
void *thread_proc(void *ival) {
    long myid = (long) ival;
    bool myconverged = false;
    int index = 1;
    long count = 0;
    phase_sum[1] = 0;
    barrier();       // Barrier #1

    while (!myconverged) {
	count++;

	int nextindex = (index+1) % PIPEDEPTH;
	phase_sum[nextindex] = 0.0;            // Action 1: Set to zero

	float myval = uniform();
	atomic_add(&phase_sum[index],  myval); // Action 2: Increment

	barrier();   // Barrier #2

        myconverged =                          // Action 3: Test
	    (phase_sum[index]/batch_size) <= target_average;

#if VERBOSE
	if (myid == 0) 
	    printf("Phase %d.  Average[%d] = %.3f\n",
		   count, index, phase_sum[index]/batch_size);
#endif

	index = nextindex;
    }
    if (myid == 0)
	pcount = count;
    return NULL;
}

void run(int bcount, float tavg) {
    batch_size = bcount;
    target_average = tavg;
    srandom(0);   
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier_value, NULL, batch_size);

    long i;
    for (i = 0; i < PIPEDEPTH; i++)
	phase_sum[i] = batch_size + 1;

    for (i = 0; i < bcount; i++) {
	pthread_create(&threads[i], NULL, thread_proc, (void *) i);
    }
    for (i = 0; i < bcount; i++) {
	pthread_join(threads[i], NULL);
    }

    float avg = phase_sum[pcount % PIPEDEPTH] / bcount;
    printf("Batch size = %d.  Target = %.3f.  Phases = %ld.  Achieved average = %.3f\n",
	   bcount, tavg, pcount, avg);

}

int main(int argc, char *argv[]) {
    int bcount = 16;
    float tavg = 0.25;
    if (argc > 1)
	bcount = atoi(argv[1]);
    if (argc > 2)
	tavg = atof(argv[2]);
    run(bcount, tavg);
    return 0;
}
