#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <iostream>
#include "threadClass.h"
using namespace std;

#define NUMTHREADS 3
pthread_mutex_t mutex; //= PTHREAD_MUTEX_INITIALIZER;
pthread_mutexattr_t attr;
queue<int*> sharedData;


void *thready(void *parm){
	threadClass pompus(&sharedData, &mutex);
	pompus.threadCall();
	return NULL;
}

int main(int argc, char const *argv[])
{
	pthread_mutexattr_init(&attr);
	pthread_mutex_init(&mutex, &attr);
	pthread_t thread[NUMTHREADS];
	int rc=0;
	int i;

	rc = pthread_mutex_lock(&mutex);
	for (i=0; i<NUMTHREADS; ++i) {
		rc = pthread_create(&thread[i], NULL, thready, NULL);
	}
	wcerr << "Initializing...";
	int a = 0;
	int b = 1;
	int c = 2;
	int d = 3;
	sharedData.push(&a);
	sharedData.push(&b);
	sharedData.push(&c);
	sharedData.push(&d);
	wcerr << "done. Unlocking." << endl;
	rc = pthread_mutex_unlock(&mutex);
	i = 0;
	for (int i=0; i<3; i++){
        pthread_join(thread[i], NULL);
    }
    sleep(5);
    wcerr << "\n RESULTS :: " << a << b << c << d << endl;
	return 0;
}