#include "p_MODE7.h"

using namespace MODE7;

Singleton::Singleton(): Object()
{
}

Singleton::~Singleton()
{
}

Object::RefCount
Singleton::retain(void)
{
	return 2;
}

Object::RefCount
Singleton::release(void)
{
	return 1;
}
