#include "p_MODE7.h"

using namespace MODE7;

Object::Object():
	refCount_(1)
{
}

Object::~Object()
{
}

Object::RefCount
Object::retain(void)
{
	refCount_++;
	return refCount_;
}

Object::RefCount
Object::release(void)
{
	RefCount r;

	assert(refCount_ > 0);
	r = --refCount_;
	if(r < 1)
	{
		delete this;
	}
	return r;
}
