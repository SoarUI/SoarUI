#include "ResourceDirectory.h"

CResourceDirectory::CResourceDirectory(const CLeeString & name):
d_name(name)
{
}
CResourceDirectory::CResourceDirectory(void)
{
}
CResourceDirectory::~CResourceDirectory(void)
{
}
CResourceDirectory::CResourceDirectory(const CResourceDirectory& rhs)
{
	d_name =rhs.getName();
	d_ImagestDir =rhs.getImagesetDirectory();
	d_PluginsDir =rhs.getPluginDirectory();
	d_WidgetlookDir =rhs.d_WidgetlookDir;
	d_ControlDir = rhs.d_ControlDir;
	d_resDir =rhs.d_resDir;
}
CLeeString CResourceDirectory::getName() const
{
	return d_name;
}
CLeeString CResourceDirectory::getImagesetDirectory() const
{
	return d_ImagestDir;
}
CLeeString CResourceDirectory::getPluginDirectory() const
{
	return d_PluginsDir;
}
CLeeString CResourceDirectory::getTextureDirectory() const
{
	return d_TexDir;
}
CLeeString CResourceDirectory::getWidgetlookDirectory() const
{
	return d_WidgetlookDir;
}
CLeeString CResourceDirectory::getControlDirectory() const
{
	return d_ControlDir;
}
CLeeString CResourceDirectory::getResourceDirectory() const
{
	return d_resDir;
}
void CResourceDirectory::setImagesetDirectory(const CLeeString& imagesetdir)
{
	d_ImagestDir =imagesetdir;
	//������ǵĳ���
	CLeeString szAp;
	szAp=TEXT("./");
	//
	CLeeString::size_type position;
	//ȡ�����ǳ���Ϊ,
	position =d_ImagestDir.find(szAp);
	//����Ѿ���ӽ�ע���
	if(position !=CLeeString::npos)
	{
		//��""ȡ��
		d_ImagestDir.replace(position,szAp.length(),d_modulePath.c_str());
	}
	
}
void CResourceDirectory::setPluginDirectory(const CLeeString& feelDir)
{
  d_PluginsDir =feelDir ;
  //������ǵĳ���
  CLeeString szAp;
  szAp=TEXT("./");
  //
  CLeeString::size_type position;
  //ȡ�����ǳ���Ϊ,
  position =d_PluginsDir.find(szAp);
  //����Ѿ���ӽ�ע���
  if(position !=CLeeString::npos)
  {
	  //��""ȡ��
	  d_PluginsDir.replace(position,szAp.length(),d_modulePath.c_str());
  }
}
void CResourceDirectory::setTextureDirectory(const CLeeString& textureDir)
{
	d_TexDir =textureDir;;
	//������ǵĳ���
	CLeeString szAp;
	szAp=TEXT("./");
	//
	CLeeString::size_type position;
	//ȡ�����ǳ���Ϊ,
	position =d_TexDir.find(szAp);
	//����Ѿ���ӽ�ע���
	if(position !=CLeeString::npos)
	{
		//��""ȡ��
		d_TexDir.replace(position,szAp.length(),d_modulePath.c_str());
	}
}
void CResourceDirectory::setWidgetlookDirectory(const CLeeString& ControlDir)
{
	d_WidgetlookDir =ControlDir;
	//������ǵĳ���
	CLeeString szAp;
	szAp=TEXT("./");
	//
	CLeeString::size_type position;
	//ȡ�����ǳ���Ϊ,
	position =d_WidgetlookDir.find(szAp);
	//����Ѿ���ӽ�ע���
	if(position !=CLeeString::npos)
	{
		//��""ȡ��
		d_WidgetlookDir.replace(position,szAp.length(),d_modulePath.c_str());
	}
}
void CResourceDirectory::setControlDirectory(const CLeeString& ControlDir)
{
	d_ControlDir =ControlDir;
	//������ǵĳ���
	CLeeString szAp;
	szAp=TEXT("./");
	//
	CLeeString::size_type position;
	//ȡ�����ǳ���Ϊ,
	position =d_ControlDir.find(szAp);
	//����Ѿ���ӽ�ע���
	if(position !=CLeeString::npos)
	{
		//��""ȡ��
		d_ControlDir.replace(position,szAp.length(),d_modulePath.c_str());
	}
}
void CResourceDirectory::setResourceDirectory(const CLeeString& ControlDir)
{
	d_resDir =ControlDir;
	//������ǵĳ���
	CLeeString szAp;
	szAp=TEXT("./");
	//
	CLeeString::size_type position;
	//ȡ�����ǳ���Ϊ,
	position =d_resDir.find(szAp);
	//����Ѿ���ӽ�ע���
	if(position !=CLeeString::npos)
	{
		//��""ȡ��
		d_resDir.replace(position,szAp.length(),d_modulePath.c_str());
	}
}
void CResourceDirectory::setMoudle(HMODULE hmod)
{
	d_hmod =hmod;
	TCHAR temPath[MAX_PATH];
	::GetModuleFileName(hmod,temPath,MAX_PATH);
	temPath[MAX_PATH-1]=TEXT('\0');
	TCHAR* pPath =_tcsrchr(temPath, TEXT('\\') );
	CLeeString tem =temPath;
	d_modulePath =tem.substr(0,pPath-temPath+sizeof(TCHAR));
}