
#include "leePathPlus.h"
CLeePathPlus::CLeePathPlus(void)
{
	d_currentExePath=getHostPath();
}

CLeePathPlus::~CLeePathPlus(void)
{
}
CLeeString CLeePathPlus::getabsPath(const CLeeString & path)//��·��ת��Ϊ����·��
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
//�������·�������·����û��ʵ�� ������
CLeeString CLeePathPlus::getRelativePath(const CLeeString & relativePath)
{
	if (isRelative(relativePath))//���������·��
	{
		return relativePath;
	}
	//���·����ͬ������ת��
	int npos=relativePath.find(d_currentExePath);
	if ( npos!= CLeeString::npos)
	{
		//�滻��ͬ����Ϊ.
		CLeeString tmp =relativePath;
		tmp.replace(0,d_currentExePath.length(),_T("."));
		return tmp;
	}
	//��ͬ������ת��
	return relativePath;
}
/*����Ҫʱת��Ϊ����·��������ǰ·����EXE·����һ��ʱ����*/
CLeeString CLeePathPlus::getRevisePath(const CLeeString & path,bool forceAbs)
{
	//���·����ͬ��ʲô������
	if (isSamePath(d_currentExePath,getCurrentPath())&&!forceAbs)
	{
		return path;
	}
	//·������ͬ��ֻ�����·����Ч
	if (isRelative(path)||forceAbs)
	{
		//�Ѿ��Ǿ���·��������
		if (isAbsolute(path))
		{
			return path;
		}
		return smartFixPath(path);
	} 
	else//����·��
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
/*��ȡ�ϼ�·��../ ..\\ */
CLeeString CLeePathPlus::getCurrentPrevPath(void)
{
	CLeeString pa =getCurrentPath();
	int nsplash=pa.rfind(TEXT('\\'));
	if(nsplash==CLeeString::npos)
		return pa;
	pa=pa.substr(0,nsplash);
	return pa;
}
/*��ȡ�ϼ�·��../ ..\\ */
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
  if (!bIsRelaived)//���"\\tese.exe"
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
//���ڲ���X:\��X:/�Ӷ��϶��Ƿ�Ϊ����·��
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
//�Ƿ����ĳǰ׺
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

/*ֻ��../ ..*/
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
 /*��ȡ�ϼ�·��../ ..\\ */
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
	 //��������ǿ��v1.1
	 int npos=szFileName.rfind(TEXT('.'));
	 if (npos>0)
	 {
		 szRet=szFileName.substr(npos+1,szFileName.length()-npos);
		 return szRet;
	 }
	 else//�ļ���
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
	 //�������·��
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
 BOOL CLeePathPlus::IsFolderExit(CLeeString szPath)//���ڲ����ļ���SHELLAPIʵ��
 {
	 LPCTSTR lpFileName=szPath.c_str();
	 //���ڲ������ļ�
	 if(PathFileExists(szPath.c_str())!=0 
		 && PathIsDirectory( szPath.c_str() ) != 0 
		 && !PathIsRoot(szPath.c_str()))
		 return TRUE;
	 else//�ļ���
	 {
		 return FALSE;
	 }
 }
 BOOL CLeePathPlus::IsFileExit(CLeeString szPath)
 {
	 LPCTSTR lpFileName=szPath.c_str();
	 //���ڲ������ļ�
	 if(PathFileExists(szPath.c_str()) && !PathIsDirectory( szPath.c_str() ))
		 return TRUE;
	 else//�ļ���
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