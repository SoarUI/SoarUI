#ifndef _IRawDataContainer_h_
#define _IRawDataContainer_h_
class IRawDataContainer
{
public:
	IRawDataContainer(void){}
	virtual ~IRawDataContainer(void){}
	virtual void setData(void* data) =0;
	virtual void* getDataPtr(void) =0 ;
	virtual void* getDataPtr(void)const =0 ;
	virtual void setSize(size_t size)=0;
	virtual size_t getSize(void)const =0;
};
#endif