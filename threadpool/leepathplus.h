#ifndef _CLeePathPlus_h_
#define _CLeePathPlus_h_
/********************************************************************
    Descibe:      ·��У�����������·��./ .\\ ../ ..\\,��EXE�ļ�·���뵱ǰ·����һ��ʱ�����и���
	�����ƽ̨�����Ը�����Ϊһ��������
    created:      2012/9/7 11:23
    FullName:     CLeePathPlus.h
    Path:         
    ClassName:   CLeePathPlus
    FileType:     h
    Author:       nicklisir@gmail.com
    NeededLib:    
    requires:      
*********************************************************************/
#include "../SoarHeader/leeString.h"
#include <windows.h>
#include <iostream>
#include "Shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
class CLeePathPlus
{
public:
	CLeePathPlus(void);
public:
	virtual ~CLeePathPlus(void);
	//·��ת��Ϊ����·��
	virtual CLeeString getabsPath(const CLeeString & Path);
	/*����Ҫʱת��Ϊ����·��������ǰ·����EXE·����һ��ʱ����*/
	virtual CLeeString getRevisePath(const CLeeString & relativePath,bool forceAbs=false);
	//�������·�������·��
	virtual CLeeString getRelativePath(const CLeeString & relativePath);
	//��ȡ��ǰ·��
	virtual CLeeString getCurrentPath(void);
	//��ȡ��ǰģ��·��
	virtual CLeeString getHostPath(void);
	//��ȡ��ǰ·����һ��
	virtual CLeeString getCurrentPrevPath(void);
	//��ȡ��ǰģ��·����һ��
	virtual CLeeString getHostPrevPath(void);
	virtual bool isSamePath(const CLeeString& path1,const CLeeString& path2);
	//�����·��
	virtual bool isRelative(const CLeeString& path);
	//�Ǿ���·��
	virtual bool isAbsolute(const CLeeString& path);
	/*ֻ��../ ..*/
	virtual bool isPrevRelative(const CLeeString& path);
	//�Ƿ����ĳǰ׺
	virtual bool isPrefix(const CLeeString prefix,const CLeeString& path) const;
	static BOOL IsFolder(CLeeString szPath);
	static BOOL IsFile(CLeeString szPath);
	static CLeeString GetFilePath(CLeeString szPath);
	static CLeeString GetFileName(CLeeString szPath);
	static CLeeString GetFileExt(CLeeString szPath);
	//-----------�ڲ�������ʹ��SHELL API
	static BOOL IsFolderExit(CLeeString szPath);
	static BOOL IsFileExit(CLeeString szPath);
private:
	/*��ȡ�ϼ�·��../ ..\\ */
	CLeeString getPrevPath(const CLeeString& path);
	CLeeString smartFixPath(const CLeeString& path);
private:
	//��ǰģ��·��
	CLeeString d_currentExePath;
};
#endif