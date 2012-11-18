#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <iostream>
using namespace std;

#define NUMTHREADS 3
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
queue<int*> sharedData;


void *thready(void *parm){
	pthread_mutex_lock(&mutex);
	wcerr << pthread_self() << " IN THE HOUSE!!!!!!" << endl;
	int * temp_buff[NUMTHREADS];
	int i = 0;
	while(!sharedData.empty()){
		wcerr << *(sharedData.front()) + 1 << " :: " << endl;
		*(sharedData.front()) = *(sharedData.front()) + 1;
		temp_buff[i] = sharedData.front();
		sharedData.pop();
		i++;
	}
	i = 0;
	while(i < 4){
		sharedData.push(temp_buff[i]);
		i++;
	}
	wcerr << pthread_self() << " PEACE OUT" << endl;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char const *argv[])
{
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