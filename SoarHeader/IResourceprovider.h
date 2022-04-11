#pragma once
#include "LeeString.h"
#include "IRawDataContainer.h"
#include "ProviderAny.h"
#include <vector>
class IResourceProvider
{
public:
	IResourceProvider(void){;}
public:
	virtual ~IResourceProvider(void){;}
	virtual void loadRawDataContainer(const CLeeString& filename, IRawDataContainer& output,size_t &totalSize ,const _ProviderKeyAny *key) = 0; 
	//�ͷż��ص��ڴ�����ݣ���������Ǹ��麯���������Ǳ���ʵ�ֵĺ��� 
	virtual void unloadRawDataContainer(IRawDataContainer& data) =0;
	virtual size_t getThunkSize() =0 ;
	virtual bool cleardataState(void) =0 ;
	virtual CLeeString getIdentityString(void) =0;
	virtual void Release(void)=0;
};
