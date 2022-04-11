#include "WidgetTheme.h"
WidgetTheme::WidgetTheme(const CLeeString& name,const CLeeString& group,
						 const CLeeString& smallTexturefile, const CLeeString& tex, const CLeeString& help):
d_ThemeString(name),d_resGroup(group),
d_SmallFileName(smallTexturefile),d_Helpstring(help),d_targetTexturePackage(tex)
{

}
WidgetTheme::WidgetTheme(void)
{
	;
}
WidgetTheme::~WidgetTheme(void)
{

}