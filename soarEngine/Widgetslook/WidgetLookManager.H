
#ifndef _WidgetLookManager_h_
#define _WidgetLookManager_h_

#include "../../SoarHeader/Singleton_t.h"
#include "../../SoarHeader/scopeSync.h"
#include "../../SoarHeader/leeString.h"
#include "WidgetlookPackage.h"
#include <map>
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#	pragma warning(disable : 4275)
#endif

class  WidgetLookManager : public LEE::Singleton_t<WidgetLookManager>
    {
    public:
        WidgetLookManager();
        ~WidgetLookManager();
		bool parseFile(const CLeeString& filename );
        bool isWidgetLookAvailable(const CLeeString& widget) ;
        CWidgetLook* getWidgetLook(const CLeeString& lookname,const CLeeString& widget) ;
		CWidgetLook* getWidgetLook(const CLeeString& widget) ;
        void eraseWidgetPackage(const CLeeString& widget);
		void clearAllWidgetPackages(void);
        void addWidgetPackage( CWidgetLookPackage& lookpacke);
		bool isWidgetPackagePresent(const CLeeString& Widgetpack);
		//公共属性，备用
		//const FeelCommon* getSharedCommon(void) const;
		//FeelCommon* getSharedCommon(void);
		//为了与Windows系统解耦而设的函数
	//unsigned int getTextColour(CLeeString& stringColour);
         const CLeeString& getActiveLook()
            { return d_thisActiveLook; }
         void setActiveLook(const CLeeString& resourceLook)
            { d_thisActiveLook = resourceLook; }

    public:
        typedef std::map<CLeeString,  CWidgetLookPackage, CLeeString::FastLessCompare> WidgetFeelList;
	private:
        WidgetFeelList  d_WidgetLookPackages;//基元打包
		 CLeeString d_thisActiveLook;//正在激活的look
		CcriticalSection d_controllock;
		CcriticalSection d_feellock;
		CcriticalSection d_feellinklock;
		CcriticalSection d_controllinklock;
		
    };

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif

#endif