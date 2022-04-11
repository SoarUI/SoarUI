#ifndef LEE_SINGLETON_SDK_H
#define LEE_SINGLETON_SDK_H
#include <string>
#include"scopeSync.h"
#include<assert.h>
/*
this file comes from CEGUI 
*/
namespace LEE {
// the content here
/*
the singleton template base class just for LeeSDK use
*/

template<typename T>
class Singleton_t
{
public:
	Singleton_t()
	{
		if(_shinstance)
		{
			throw(_shinstance);
		}
		_shinstance = static_cast<T*>(this);
	}
	virtual ~Singleton_t(void)
	{
		_shinstance = NULL;
	}
	static T * getSingletonPtr( void )
	{
		return _shinstance ;
	}
	static T & getSingleton( void )
	{
		return (* _shinstance ) ;
	}
private:
 Singleton_t(const Singleton_t<T> &);
 Singleton_t& operator=(const Singleton_t<T> &);    

protected:
	static T*  _shinstance;
};
template<typename T>
T* Singleton_t<T>::_shinstance = NULL;
// ------------------------------------------
//spector
//END_PROTECTH
}
#endif //end of file