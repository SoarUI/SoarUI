#ifndef GLOBAL_MACRO_H
#define GLOBAL_MACRO_H
/********************************************************************
	copyright       ecsuper@163.com
        created:	2011/09/11
	updated:	2012/03/08 22:51
	filename: 	LeeMacro.h
	file path:	$(LeeSDK_INCLUDE)
	file base:	LeeSDK
	file ext:	h
	author:		nicklisir
        email:  ecsuper@163.com
	LastUpdate      2017/10/19
	purpose:	the macro of the LeeX software design
*********************************************************************/
/*
Update:  2012/04/25 13:07:40
Content: add Dbgc macro for debug
Person : nicklisir@gmail.com
*/
//内存检测
#include<malloc.h>
#include <tchar.h>
//string
#include <xstring>
//filestream
#include <iosfwd>
#include <fstream>
//the namespace macro
#define   BEGIN_NAMSPACE(x)                  namespace x { 
#define   BEGIN_NAMSPACE_EX(prefix,sub)     namespace prefix##sub {
#define   BEGIN_NAMSPACE_DB(g,sub)            BEGIN_NAMSPACE(g) \
	                                            BEGIN_NAMSPACE(sub)
#define   END_NAMSPACE                    } 
//for the double end 
#define   END_NAMSPACE_DB                  } \
										 }

/*++++++++++++++++++++++++++++++++++++++++++++++
namespace macro:
WEB:SOCKET LIB
APP：software LIB
MUI：UI base on MFC
SUI：UI base on SDK in other word，it is base on the LeeSDK lib，which just likes the MFC
+++++++++++++++++++++++++++++++++++++++++++++++++++*/

//网络通信库,需求少，无更新，整合至GDK中
#define   BEGIN_NAMSPACE_WEB             BEGIN_NAMSPACE(WEB)
//MFC类库,停止更新
#define   BEGIN_NAMSPACE_MUI             BEGIN_NAMSPACE(MUI)
//未使用,处于原始状态，未开发
#define   BEGIN_NAMSPACE_SUI             BEGIN_NAMSPACE(SUI)
//应用程序库,与SUI合并为APP，框架形成，犹豫中，整合至LeeSDK中
#define   BEGIN_NAMSPACE_APP             BEGIN_NAMSPACE(APP)
//图形引擎，处于初开发阶段,整合至LeeSDK中
#define   BEGIN_NAMSPACE_ENGINE             BEGIN_NAMSPACE(ENGINE)
//获取命名空间,改名NAMESPACEACCEPT
#define   NAMSPACEEX(g,sub)              g##::##sub
#define   NAMSPACE(g)              g

#define KEYISDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
// 设置位状态
#define SETBIT(Data, Mask, IsSet)	((IsSet) ? (Data |= Mask) : (Data &= ~Mask))
							   
#ifdef LEESDK_DLL
#define LEESDK_API __declspec(dllexport)
#else
#define LEESDK_API __declspec(dllimport)
#endif
#ifdef UNICODE
#define STRING std::wstring
#else
#define STRING std::string
#endif
//std::string /std::wstring
typedef std::basic_string<TCHAR, std::char_traits<TCHAR>,std::allocator<TCHAR> > TSTRING;
//std;;fstream/std::wfstream
typedef std::basic_fstream<TCHAR, std::char_traits<TCHAR> > TFSTREAM;
//输出函数定义帮助函数
#define  CFTR_BEGIN extern "C" {
#define  CFTR_END   }

//为了易读决定改名
#define CFUN_BEGIN CFTR_BEGIN 
#define CFUN_END CFTR_END  
//
/*
两辆之间可以组成更多参数
*/
#define  Dbgc_Param2(p1,p2) p1##,##p2
#define  Dbgc_Param3(p1,p2,p3) p1##,##p2##,##p3
#define  Dbgc_Param4(p1,p2,p3,p4) p1##,##p2##,##p3##,##p4
#define  Dbgc_Param5(p1,p2,p3,p4,p5) p1##,##p2##,##p3##,##p4##,##p5
#define  Dbgc_Param6(p1,p2,p3,p4,p5,p6) p1##,##p2##,##p3##,##p4##,##p5##,##p6
#define  Dbgc_Param7(p1,p2,p3,p4,p5,p6,p7) p1##,##p2##,##p3##,##p4##,##p5##,##p6##,##p7
//调试帮助函数
#define __Ansi2W(x)    L##x
#define _Ansi2WSTR(X)     __Ansi2W(X)
#define LEEWIDEN(x) _Ansi2WSTR(x)
#ifdef UNICODE
#define Dbgc_FILE _Ansi2WSTR(__FILE__)
#define Dbgc_FUNCTION _Ansi2WSTR(__FUNCTION__)
#define Dbgc_DATE _Ansi2WSTR(__DATE__)
#define Dbgc_FUNCDNAME   _Ansi2WSTR(__FUNCDNAME__)
#define Dbgc_FUNCSIG _Ansi2WSTR(__FUNCSIG__)
#else
#define Dbgc_FILE __FILE__
#define Dbgc_FUNCTION __FUNCTION__
#define Dbgc_DATE __DATE__
#define Dbgc_FUNCDNAME   __FUNCDNAME__
#define Dbgc_FUNCSIG __FUNCSIG__
#endif
#define Dbgc_LINE __LINE__
#define DBGC_NEW new(_NORMAL_BLOCK,__FILE__,__LINE__)

////////////////////////

#ifndef ENU_OS_VERSION_GUAID
#define ENU_OS_VERSION_GUARD
enum ENU_OS_VERSION
{
	ENU_OS_UNKNOWD =0,
	ENU_OS_XP ,
	ENU_OS_32s,
	ENU_OS_2000,
	ENU_OS_2003,
	ENU_OS_95,
	ENU_OS_98,
	ENU_OS_ME,
	ENU_OS_WIN7,
	ENU_OS_WIN8,
	ENU_OS_WIN9,
	ENU_OS_WIN10
};
#endif
#define LEEPURE =0
#endif //end of file 