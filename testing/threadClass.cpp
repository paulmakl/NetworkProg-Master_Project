#include "threadClass.h"

pthread_mutex_t * mutex_thread;
queue<int*> * QueueStuff;

threadClass::threadClass(queue<int*> * Que, pthread_mutex_t * muta){
	QueueStuff = Que;
	mutex_thread = muta;
}

int threadClass::threadCall(){
	pthread_mutex_lock(mutex_thread);
	wcerr << pthread_self() << " IN THE HOUSE!!!!!!" << endl;
	int * temp_buff[4];
	int i = 0;
	while(!QueueStuff->empty()){
		wcerr << *(QueueStuff->front()) + 1 << " :: " << endl;
		*(QueueStuff->front()) = *(QueueStuff->front()) + 1;
		temp_buff[i] = QueueStuff->front();
		QueueStuff->pop();
		i++;
	}
	i = 0;
	while(i < 4){
		QueueStuff->push(temp_buff[i]);
		i++;
	}
	wcerr << pthread_self() << " PEACE OUT" << endl;
	pthread_mutex_unlock(mutex_thread);
	return 1;
}
