
#ifndef _soarResourceProvider_h_
#define _soarResourceProvider_h_

#include <map>
#include "../SoarHeader/IResourceprovider.h"

#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif

class CSoarResourceProvider : public IResourceProvider
{
public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	CSoarResourceProvider(); 
	virtual~CSoarResourceProvider(void); 
    void loadRawDataContainer(const CLeeString& filename, IRawDataContainer& output,size_t &totalSize ,const _ProviderKeyAny *key =0);
    void unloadRawDataContainer(IRawDataContainer& data);
	virtual void Release(void);
	virtual size_t getThunkSize()  ;
	virtual bool cleardataState(void)  ;
	virtual CLeeString getIdentityString(void);
protected:
	CLeeString d_identityString ;
};


#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif	// end of guard _soarResourceProvider_h_
