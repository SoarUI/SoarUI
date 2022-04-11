#ifndef LeeSDK_LOG_SDK_H
#define LeeSDK_LOG_SDK_H
//2011-08-02 14:09 created
/*
Update:  2014/09/20 00:02
Content: small change ,only the macro,donot devired from singleton_t
Person : nicklisir@gmail.com
*/

#include <vadefs.h>
#include <stdarg.h>
#include<tchar.h>
#include <stdio.h>
#include "leemacro.h"

namespace LEE {
#pragma warning( disable : 4996 ) // disable deprecated warning

class CLeeLog
{
public:
	CLeeLog(void){};
public:
	~CLeeLog(void){};
	//一般函数，输出格式数据到文件
	static BOOL WriteLog(LPCTSTR data,...)
	{
		SYSTEMTIME SystemTime;
		TCHAR CurrentDate[32]={0,};
		TCHAR CurrentFileName[MAX_PATH]={0,};//
		FILE* FilePtr=NULL;
		TCHAR DeBugLog[2048*MAX_PATH]={0,};
                va_list ap;
		TCHAR Log[MAX_PATH*1024]={0,};
		memset(CurrentFileName,0,sizeof(CurrentFileName));
		TCHAR tCurPath[MAX_PATH] ={0,};
		::GetModuleFileName(NULL,tCurPath ,sizeof(tCurPath) );
		TCHAR * tLastSpash = _tcsrchr(tCurPath,_T('\\'));
		if(!tLastSpash)
			return FALSE;
		_tcsncpy(CurrentFileName,tCurPath,tLastSpash - tCurPath);
		va_start(ap,data);
		_vstprintf(Log,data,ap);
		va_end(ap);
		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate,32,TEXT("%d-%.2d-%.2d %.2d:%.2d:%.2d %.4d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond,
			SystemTime.wMilliseconds
			);
		_sntprintf(CurrentFileName,MAX_PATH,TEXT("%s\\GLog_%d-%.2d-%.2d.log"),
			CurrentFileName,
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay);
		CurrentFileName[MAX_PATH-1]=TEXT('\0');
		FilePtr=_tfopen(CurrentFileName,TEXT("a"));
		if(!FilePtr) return FALSE;
		_ftprintf(FilePtr,TEXT("[%s] %s \n"),CurrentDate,Log);
		_sntprintf(DeBugLog,MAX_PATH*2048,TEXT("[%s] %s\n"),CurrentDate,Log);
		DeBugLog[MAX_PATH*2048-1]=TEXT('\0');
		fflush(FilePtr);
		fclose(FilePtr);	
                //显示
		OutputDebugString(DeBugLog);
		return TRUE;

	}
	static BOOL ModuleLog(HMODULE hModule,LPCTSTR data,...)
	{
		SYSTEMTIME SystemTime;
		TCHAR CurrentDate[32]={0,};
		TCHAR CurrentFileName[MAX_PATH]={0,};//
		FILE* FilePtr=NULL;
		TCHAR DeBugLog[2048*MAX_PATH]={0,};
                va_list ap;
		TCHAR Log[MAX_PATH*1024]={0,};
		memset(CurrentFileName,0,sizeof(CurrentFileName));
		TCHAR tCurPath[MAX_PATH] ={0,};
		::GetModuleFileName(hModule,tCurPath ,sizeof(tCurPath) );
		TCHAR * tLastSpash = _tcsrchr(tCurPath,_T('\\'));
		if(!tLastSpash)
			return FALSE;
		_tcsncpy(CurrentFileName,tCurPath,tLastSpash - tCurPath);
		va_start(ap,data);
		_vstprintf(Log,data,ap);
		va_end(ap);
		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate,32,TEXT("%d-%.2d-%.2d %.2d:%.2d:%.2d %.4d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond,
			SystemTime.wMilliseconds
			);
		_sntprintf(CurrentFileName,MAX_PATH,TEXT("%s\\GLog_%d-%.2d-%.2d.log"),
			CurrentFileName,
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay);
		CurrentFileName[MAX_PATH-1]=TEXT('\0');
		FilePtr=_tfopen(CurrentFileName,TEXT("a"));
		if(!FilePtr) return FALSE;
		_ftprintf(FilePtr,TEXT("[%s] %s \n"),CurrentDate,Log);
		_sntprintf(DeBugLog,MAX_PATH*2048,TEXT("[%s] %s\n"),CurrentDate,Log);
		DeBugLog[MAX_PATH*2048-1]=TEXT('\0');
		fflush(FilePtr);
		fclose(FilePtr);	
                //显示
		OutputDebugString(DeBugLog);
		return TRUE;

	}
	static BOOL ModuleLogEx(LPCSTR CurrentFileName,LPCTSTR data,...)
	{
		if(!CurrentFileName ) return false;
		SYSTEMTIME SystemTime;
		TCHAR CurrentDate[32]={0,};
		FILE* FilePtr=NULL;
		TCHAR DeBugLog[2048*MAX_PATH]={0,};
        va_list ap;
		TCHAR Log[MAX_PATH*1024]={0,};
		va_start(ap,data);
		_vstprintf(Log,data,ap);
		va_end(ap);
		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate,32,TEXT("%d-%.2d-%.2d %.2d:%.2d:%.2d %.4d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond,
			SystemTime.wMilliseconds
			);
		
		FilePtr=_tfopen(CurrentFileName,TEXT("a"));
		if(!FilePtr) return FALSE;
		_ftprintf(FilePtr,TEXT("[%s] %s \n"),CurrentDate,Log);
		_sntprintf(DeBugLog,MAX_PATH*2048,TEXT("[%s] %s\n"),CurrentDate,Log);
		DeBugLog[MAX_PATH*2048-1]=TEXT('\0');
		fflush(FilePtr);
		fclose(FilePtr);	
                //显示
		OutputDebugString(DeBugLog);
		return TRUE;

	}

};
//日志宏
#ifdef _DEBUG
//详细函数调用,可以加备注
#define   Dbgc_FunctionLog(x)          LEE::CLeeLog::WriteLog(TEXT("TARGET_FILENAME:<%s>\n LINE:<%.3d> TARGET_FUNCTION:<%s>\n %s\n"),\
	Dbgc_FILE,\
	Dbgc_LINE,\
	Dbgc_FUNCTION,\
	x);
#define   Dbgc_FunctionLogEx(xFile,x)          LEE::CLeeLog::ModuleLogEx(xFile,TEXT("TARGET_FILENAME:<%s>\n LINE:<%.3d> TARGET_FUNCTION:<%s>\n %s\n"),\
	Dbgc_FILE,\
	Dbgc_LINE,\
	Dbgc_FUNCTION,\
	x);
//路径输出
#define   Dbgc_Step_dump()          LEE::CLeeLog::WriteLog(TEXT("STEP"),TEXT("LINE:<%.3d> TARGET_FUNCTION:<%s>\n"),\
	Dbgc_LINE,\
	Dbgc_FUNCTION);
#define   Dbgc_Step_dumpX(xFile)          LEE::CLeeLog::ModuleLogEx(xFile,TEXT("LINE:<%.3d> TARGET_FUNCTION:<%s>\n"),\
	Dbgc_LINE,\
	Dbgc_FUNCTION);
#else
#define   Dbgc_FunctionLog(x) NULL;
#define   Dbgc_FunctionLogEx(xFile,x) NULL;
#define   Dbgc_Step_dump() NULL;
#define   Dbgc_Step_dumpX(xFile) NULL;
#endif

#ifdef _DEBUG
#define  Dbgc_Dump(tFormat,params)  LEE::CLeeLog::WriteLog(\
	tFormat ,\
	params);
#define  Dbgc_DumpX(xFile,tFormat,params)  LEE::CLeeLog::ModuleLogEx( xFile,\
	tFormat ,\
	params);
#else
#define  Dbgc_Dump(tFormat,params) NULL ;
#define  Dbgc_DumpX(xFile,tFormat,params) NULL ;
#define  Dbgc_DumpX2(dirRoot,xFile,tFormat,params) NULL;
#endif


#ifdef    _DEBUG //内存改写检查

#define   Dbgc_Break_Detail(tFormat,params)          if(_heapchk()!=_HEAPOK) \
													 {\
													 Dbgc_Function_Dump(tFormat , params);\
													 DebugBreak(); \}
#define   Dbgc_Break_DetailX(xFile,tFormat,params)          if(_heapchk()!=_HEAPOK) \
													 {\
													 Dbgc_Function_DumpX(xFile,tFormat , params);\
													 DebugBreak(); \}

#else
#define   Dbgc_Break_Detail(tFormat,params)                NULL ;
#define   Dbgc_Break_DetailX(xFile,tFormat,params)         NULL ;
#endif   
//end of namespace
}
#endif