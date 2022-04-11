#include "objectReferentBase.h"
objectReferentBase::objectReferentBase(void):d_cref(0)
{
}

objectReferentBase::~objectReferentBase(void)
{
}
void objectReferentBase::addRef(void)
{
	++d_cref;
}
void objectReferentBase::release(void)
{
	--d_cref;
}
size_t objectReferentBase::getRef(void)const
{
	return d_cref;
}