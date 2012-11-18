#ifndef LOCK_ARRAY_H_
#define LOCK_ARRAY_H_

template<typename Element, unsigned int Size>
class Lock_Array
{
public:
	Lock_Array();
	unsigned short create_Lock_ID(Element& item);
private:
	char test[];
	int size;
	volatile unsigned int head;
	volatile unsigned int tail;
	unsigned short lock_ID;
};

#endif /* LOCK_ARRAY_H_ */