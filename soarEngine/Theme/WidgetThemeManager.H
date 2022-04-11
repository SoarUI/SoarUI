#pragma once
#include "../../SoarHeader/Singleton_t.h"
#include "../../SoarHeader/scopeSync.h"
#include "../../SoarHeader/LeeString.h"
#include "WidgetTheme.h"
//#include "../FeelStructDeclares.h"
#include <map>
class WidgetThemeManager: public LEE::Singleton_t<WidgetThemeManager>
{
public:
	WidgetThemeManager(void);
public:
	virtual ~WidgetThemeManager(void);
	typedef std::map<CLeeString, WidgetTheme, CLeeString::FastLessCompare> WidgetThemeList;
	//解析XML
	void parseFile(const CLeeString& filename);
	bool isThemeAvailable(const CLeeString& theme);
	bool isThemePresent(const CLeeString& theme);
	void eraseWidgetTheme(const CLeeString& Theme);
	void addWidgetTheme(const WidgetTheme& Theme);
	void clearAllTheme(void);
	 WidgetTheme* getWidgetTheme(const CLeeString& Theme) ;
	 void setActiveTheme(const CLeeString& theme);
	 //获取当前激活的主题名称
     CLeeString& getActiveTheme(void);
	 //获取当前激活的主题的皮肤文件
	 CLeeString  getTexturePackage(void);
	 //---------------------------------------------//
 //提供给外部使用
 WidgetThemeList getWidgetThemes(void) ;
private:
	WidgetThemeList  d_widgetThemes;
	CLeeString d_activeTheme;//当前激活的皮肤，在Mixed模式有效
	CcriticalSection d_themelock;
};
