
#include "leePathPlus.h"
CLeePathPlus::CLeePathPlus(void)
{
	d_currentExePath=getHostPath();
}

CLeePathPlus::~CLeePathPlus(void)
{
}
CLeeString CLeePathPlus::getabsPath(const CLeeString & path)//将路径转化为绝对路径
{
	if (!isRelative(path) )
	{
		return path;
	} 
	if (isPrevRelative(path))
	{
		return getPrevPath(path);
	}
	CLeeString pa =getHostPath();
	if (isPrefix(_T("./"),path))
	{
		int npos=path.find(_T('/'));
		CLeeString str=path.substr(npos,CLeeString::npos);
		pa+=str;
		return pa;
	}
	if (isPrefix(_T(".\\"),path))
	{
		int npos=path.find(_T('\\'));
		CLeeString str=path.substr(npos,CLeeString::npos);
		pa+=str;
		return pa;
	}
	if (isPrefix(_T("\\"),path))
	{
		int npos=path.find(_T('\\'));
		CLeeString str=path.substr(npos,CLeeString::npos);
		pa+=str;
		return pa;
	}
	if (isPrefix(_T("/"),path))
	{
		int npos=path.find(_T('/'));
		CLeeString str=path.substr(npos,CLeeString::npos);
		pa+=str;
		return pa;
	}
	if (isPrefix(_T("."),path))
	{
		pa +=_T("\\");
		return pa;
	}
	pa +=_T("\\");
	pa += path;
	return pa;
}
//传入绝对路径求相对路径，没有实现 。。。
CLeeString CLeePathPlus::getRelativePath(const CLeeString & relativePath)
{
	if (isRelative(relativePath))//本身是相对路径
	{
		return relativePath;
	}
	//如果路径相同，进行转换
	int npos=relativePath.find(d_currentExePath);
	if ( npos!= CLeeString::npos)
	{
		//替换相同部分为.
		CLeeString tmp =relativePath;
		tmp.replace(0,d_currentExePath.length(),_T("."));
		return tmp;
	}
	//不同，不能转换
	return relativePath;
}
/*有需要时转化为绝对路径，当当前路径与EXE路径不一样时修正*/
CLeeString CLeePathPlus::getRevisePath(const CLeeString & path,bool forceAbs)
{
	//如果路径相同，什么都不做
	if (isSamePath(d_currentExePath,getCurrentPath())&&!forceAbs)
	{
		return path;
	}
	//路径不相同，只对相对路径有效
	if (isRelative(path)||forceAbs)
	{
		//已经是绝对路径，返回
		if (isAbsolute(path))
		{
			return path;
		}
		return smartFixPath(path);
	} 
	else//绝对路径
	{
		return path;
	}
}
CLeeString CLeePathPlus::getCurrentPath(void)//current path
{
	TCHAR tChPath[MAX_PATH] ={0,};
	::GetCurrentDirectory(sizeof(tChPath),tChPath );
	CLeeString szPath = tChPath ;
	return szPath;
}
CLeeString CLeePathPlus::getHostPath(void)//EXE path
{
	TCHAR tChPath[MAX_PATH] ={0,};
	::GetModuleFileName(NULL,tChPath ,sizeof(tChPath) );
	CLeeString szPath = tChPath ;
	int nsplash=szPath.rfind(TEXT('\\'));
	if(nsplash==CLeeString::npos)
		return _T(".");
	szPath=szPath.substr(0,nsplash);
	return szPath;
}
/*获取上级路径../ ..\\ */
CLeeString CLeePathPlus::getCurrentPrevPath(void)
{
	CLeeString pa =getCurrentPath();
	int nsplash=pa.rfind(TEXT('\\'));
	if(nsplash==CLeeString::npos)
		return pa;
	pa=pa.substr(0,nsplash);
	return pa;
}
/*获取上级路径../ ..\\ */
CLeeString CLeePathPlus::getHostPrevPath(void)
{
	CLeeString pa =getHostPath();
	int nsplash=pa.rfind(TEXT('\\'));
	if(nsplash==CLeeString::npos)
		return pa;
	pa=pa.substr(0,nsplash);
	return pa;
}
bool CLeePathPlus::isSamePath(const CLeeString& path1,const CLeeString& path2)
{
	if (path1.empty()||path2.empty())
	{
		return false;
	}
	return path2==path1;
}
bool CLeePathPlus::isRelative(const CLeeString& path)
{
	if (path.empty())
	{
		return false;
	}
  bool bIsRelaived= false;
  bIsRelaived =PathIsRelative(path.c_str());
  if (!bIsRelaived)//检测"\\tese.exe"
  {
	  CLeeString prepp=path.substr(0,1);
	  CLeeString sTest=_T("\\");
	  if (prepp==sTest)
	  {
		  bIsRelaived =true;
	  }
  }
  return bIsRelaived;
}
//基于查找X:\或X:/从而认定是否为绝对路径
bool CLeePathPlus::isAbsolute(const CLeeString& path)
{
	if (path.empty())
	{
		return false;
	}
	int npos=path.find(_T(":\\"));
	int npos1=path.find(_T(":/"));
	bool bAbs=(npos!=CLeeString::npos?true:false);
	bAbs |=(npos1!=CLeeString::npos?true:false);
	return bAbs;
}
//是否具有某前缀
bool CLeePathPlus::isPrefix(const CLeeString prefix,const CLeeString& path) const
{
	if (prefix.empty()||path.empty()||
		path.length()<prefix.length())
	{
		return false;
	}
	const CLeeString pres=path.substr(0,prefix.length());
	return prefix == pres;
}

/*只对../ ..*/
 bool CLeePathPlus::isPrevRelative(const CLeeString& path)
 {
	 if (!isRelative(path))
	 {
		 return false;
	 }
	 if (isPrefix(_T("../"),path)||isPrefix(_T("..\\"),path))
	 {
		 return true;
	 }
	 return false;
 }
 /*获取上级路径../ ..\\ */
 CLeeString CLeePathPlus::getPrevPath(const CLeeString& path)
 {
	 CLeeString pa =getHostPrevPath();
	 if (isPrefix(_T("../"),path))
	 {
		 int npos=path.find(_T('/'));
		 CLeeString str=path.substr(npos,CLeeString::npos);
		 pa+=str;
		 return pa;
	 }
	 if (isPrefix(_T("..\\"),path))
	 {
		 int npos=path.find(_T('\\'));
		 CLeeString str=path.substr(npos,CLeeString::npos);
		 pa+=str;
		 return pa;
	 }
 }
 CLeeString CLeePathPlus::smartFixPath(const CLeeString& path)
 {
	 if (isPrefix(_T("../"),path)||isPrefix(_T("..\\"),path))
	 {
		 return getPrevPath(path);
	 }
	 return getabsPath(path);
 }


 CLeeString CLeePathPlus::GetFileExt(CLeeString szFileName)
 {
	 CLeeString szRet;
	 szRet.clear();
	 if (szFileName.empty()||IsFolder(szFileName))
	 {
		 return szRet;
	 }
	 //功能需增强如v1.1
	 int npos=szFileName.rfind(TEXT('.'));
	 if (npos>0)
	 {
		 szRet=szFileName.substr(npos+1,szFileName.length()-npos);
		 return szRet;
	 }
	 else//文件夹
	 {
		 return szRet;
	 }
 }
 CLeeString CLeePathPlus::GetFilePath(CLeeString szPath)
 {
	 CLeeString szRet;
	 szRet.clear();
	 if (szPath.empty())
	 {
		 return szRet;
	 }
	 if(IsFolder(szPath) )
		 return szPath;
	 int npos=szPath.rfind(TEXT('\\'));
	 if (npos>0)
	 {
		 szRet=szPath.substr(0,npos+1);
		 return szRet;
	 }
	 //返回相对路径
	 szRet=TEXT(".\\");
	 return szRet;
 }
 CLeeString CLeePathPlus::GetFileName(CLeeString szPath)
 {
	 CLeeString szRet;
	 szRet.clear();
	 if (szPath.empty()||IsFolder(szPath) )
	 {
		 return szRet;
	 }
	 int npos=szPath.rfind(TEXT('.'));
	 int nPos1=szPath.rfind(TEXT('\\'));
	 if (npos>0)
	 {
		 szRet=szPath.substr(nPos1+1,npos-nPos1-1);
		 return szRet;
	 }
	 return szPath;
 }
 BOOL CLeePathPlus::IsFolderExit(CLeeString szPath)//存在并是文件夹SHELLAPI实现
 {
	 LPCTSTR lpFileName=szPath.c_str();
	 //存在并且是文件
	 if(PathFileExists(szPath.c_str())!=0 
		 && PathIsDirectory( szPath.c_str() ) != 0 
		 && !PathIsRoot(szPath.c_str()))
		 return TRUE;
	 else//文件夹
	 {
		 return FALSE;
	 }
 }
 BOOL CLeePathPlus::IsFileExit(CLeeString szPath)
 {
	 LPCTSTR lpFileName=szPath.c_str();
	 //存在并且是文件
	 if(PathFileExists(szPath.c_str()) && !PathIsDirectory( szPath.c_str() ))
		 return TRUE;
	 else//文件夹
	 {
		 return FALSE;
	 }
 }
 BOOL CLeePathPlus::IsFolder(CLeeString szPath)
 {
	 return IsFolderExit(szPath);
 }
 BOOL CLeePathPlus::IsFile(CLeeString szPath)
 {
	 return IsFileExit(szPath);
 }