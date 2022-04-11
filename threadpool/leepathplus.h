#ifndef _CLeePathPlus_h_
#define _CLeePathPlus_h_
/********************************************************************
    Descibe:      路径校正，处理相对路径./ .\\ ../ ..\\,当EXE文件路径与当前路径不一样时，进行改正
	如需跨平台可以以该类作为一个抽象类
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
	//路径转化为绝对路径
	virtual CLeeString getabsPath(const CLeeString & Path);
	/*有需要时转化为绝对路径，当当前路径与EXE路径不一样时修正*/
	virtual CLeeString getRevisePath(const CLeeString & relativePath,bool forceAbs=false);
	//传入绝对路径求相对路径
	virtual CLeeString getRelativePath(const CLeeString & relativePath);
	//获取当前路径
	virtual CLeeString getCurrentPath(void);
	//获取当前模块路径
	virtual CLeeString getHostPath(void);
	//获取当前路径上一级
	virtual CLeeString getCurrentPrevPath(void);
	//获取当前模块路径上一级
	virtual CLeeString getHostPrevPath(void);
	virtual bool isSamePath(const CLeeString& path1,const CLeeString& path2);
	//是相对路径
	virtual bool isRelative(const CLeeString& path);
	//是绝对路径
	virtual bool isAbsolute(const CLeeString& path);
	/*只对../ ..*/
	virtual bool isPrevRelative(const CLeeString& path);
	//是否具有某前缀
	virtual bool isPrefix(const CLeeString prefix,const CLeeString& path) const;
	static BOOL IsFolder(CLeeString szPath);
	static BOOL IsFile(CLeeString szPath);
	static CLeeString GetFilePath(CLeeString szPath);
	static CLeeString GetFileName(CLeeString szPath);
	static CLeeString GetFileExt(CLeeString szPath);
	//-----------内部函数，使用SHELL API
	static BOOL IsFolderExit(CLeeString szPath);
	static BOOL IsFileExit(CLeeString szPath);
private:
	/*获取上级路径../ ..\\ */
	CLeeString getPrevPath(const CLeeString& path);
	CLeeString smartFixPath(const CLeeString& path);
private:
	//当前模块路径
	CLeeString d_currentExePath;
};
#endif