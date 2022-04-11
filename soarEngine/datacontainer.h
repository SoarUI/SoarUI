
#ifndef _DataContainer_h_
#define _DataContainer_h_
#include<tchar.h>
#include "../SoarHeader/IRawDataContainer.h"
class  RawDataContainer:public IRawDataContainer
{
public:
    RawDataContainer()
      : mData(0),
        mSize(0)
    {
    }
	~RawDataContainer(void) 
    { 
        release();
    }
    void setData( void* data) { mData = data; }
    void* getDataPtr(void) { return mData; }
    void* getDataPtr(void) const { return mData; }
    void setSize(size_t size) { mSize = size; }
    size_t getSize(void) const { return mSize; }
	void release(void){  ; }

private:
    /*************************************************************************
		Implementation Data
	*************************************************************************/
    void* mData;
    size_t mSize;
};

#endif	// end of guard _DataContainer_h_
