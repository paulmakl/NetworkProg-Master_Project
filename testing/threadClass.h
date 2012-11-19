#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <iostream>
using namespace std;

class threadClass {
public:
	threadClass(queue<int*> * Que, pthread_mutex_t * muta);
	int threadCall();
private:
	queue<int*> * QueueStuff;
	pthread_mutex_t * mutex_thread;
};