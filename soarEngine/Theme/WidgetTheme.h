#ifndef _WidgetTheme_h_
#define _WidgetTheme_h_
#include "../../SoarHeader/LeeString.h"
class WidgetTheme
{
public:
	WidgetTheme(void);
	WidgetTheme(const CLeeString& name, const CLeeString& group,
		const CLeeString& smallTexturefile,const CLeeString& tex, const CLeeString& help);
public:
	virtual ~WidgetTheme(void);
	const CLeeString& getName() const{return d_ThemeString ;}
	const CLeeString& getGroup() const{ return d_resGroup ;}
	const CLeeString& getsmallTextureFile() const { return d_SmallFileName;}
	const CLeeString& getHelpstring() const{ return d_Helpstring ;}
	const CLeeString& getTexturePackage() const{ return d_targetTexturePackage ;}
private:
	CLeeString d_ThemeString;
	CLeeString d_resGroup;
	CLeeString d_SmallFileName;
	CLeeString d_Helpstring;
	CLeeString d_targetTexturePackage;
};
#endif