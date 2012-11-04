//#ifndef __MyClass_H_INCLUDED__   // if x.h hasn't been included yet...
//#define __MyClass_H_INCLUDED__ 
#ifndef __MyClass_H
#define MyClass_H
class MyClass
{
public:
  void foo();
};

void MyClass::foo()
{
	cout << "hi";
}
#endif