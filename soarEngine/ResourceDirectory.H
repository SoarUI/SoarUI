#pragma once
#include "../SoarHeader/leeString.h"
//×é¸ÅÄî
class CResourceDirectory
{
public:
	CResourceDirectory(void);
	CResourceDirectory(const CLeeString & name);
	CResourceDirectory(const CResourceDirectory& rhs);
public:
	virtual ~CResourceDirectory(void);
	CLeeString getName(void) const;
	CLeeString getImagesetDirectory(void) const;
	CLeeString getPluginDirectory(void) const;
	CLeeString getTextureDirectory(void) const;
	CLeeString getWidgetlookDirectory(void) const;
	CLeeString getControlDirectory(void) const;
	CLeeString getResourceDirectory(void) const;
	void setImagesetDirectory(const CLeeString& imagesetdir);
	void setPluginDirectory(const CLeeString& feelDir);
	void setTextureDirectory(const CLeeString& textureDir);
	void setWidgetlookDirectory(const CLeeString& textureDir);
	void setControlDirectory(const CLeeString& controlDir);
	void setResourceDirectory(const CLeeString& resDir);
	void setMoudle(HMODULE hmod);
private:
	CLeeString d_name;
	CLeeString d_ImagestDir;
	CLeeString d_PluginsDir;
	CLeeString d_TexDir;
	CLeeString d_WidgetlookDir;
	CLeeString d_ControlDir;
	CLeeString d_resDir;
	CLeeString d_modulePath;
	HMODULE d_hmod;


};
