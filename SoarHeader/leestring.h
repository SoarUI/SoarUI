#pragma once
#include "leemacro.h"
#define WIN32_LEAN_AND_MEAN //用于解决winsock2.h 重定义
#include <Windows.h>
#include <algorithm>
#include <cctype>
#include <iterator>
#pragma warning(disable:4996)
class CLeeString :
	public TSTRING
{
public:
	CLeeString(void){}
	CLeeString(const TCHAR* cstr)
	{
		if (cstr != NULL)
		{
			TSTRING::assign(cstr);
		}		
	}
	CLeeString(const CLeeString& str)
	{
		TSTRING::assign(str.c_str());
	}
	CLeeString(const TSTRING& str)
	{
		TSTRING::assign(str.c_str());
	}
public:
	virtual ~CLeeString(void)
	{
		
	}
	//用于比较map/vector
	struct FastLessCompare
	{
		bool operator() (const CLeeString& a, const CLeeString& b) const
		{
			const size_t la = a.length();
			const size_t lb = b.length();
			if (la == lb)
				return ( a.compare(b.data() )<0 );
			return (la < lb);
		}
	};
	CLeeString&	operator+=(const CLeeString& str)
	{
		 TSTRING::append(str.c_str());
		 return *this;
	}
	CLeeString&	operator=(const CLeeString& str)
	{
		 TSTRING::assign(str.c_str());
		 return *this;
	}
	CLeeString&	operator+(const CLeeString& str)
	{
		TSTRING::append(str);
		return *this;
	}
	CLeeString&	operator+(const TCHAR* cstr)
	{
		TSTRING::append(cstr);
		return *this;
	}
	CLeeString&	operator+=(const TSTRING& std_str)
	{
		TSTRING::append(std_str.c_str());
		 return *this;
	}
	CLeeString&	operator+=(const TCHAR* cstr)
	{
		TSTRING::append(cstr, lstrlen(cstr));
		 return *this;
	}
	bool	operator!=(const TCHAR* cstr)
	{
		const size_t la= lstrlen(cstr);
		if (la==length())
		{
			return lstrcmpi(data(),cstr)!=0;
		}
		return la != length();
	}
	bool	operator!=(const CLeeString &cstr)
	{
		const size_t la= cstr.length();
		if (la==length())
		{
			return lstrcmpi(data(),cstr.data())!=0;
		}
		return la != length();
	}
	CLeeString& append(const TCHAR* cstr)
	{
		 TSTRING::append(cstr);
		 return *this;
	}
	CLeeString& append(const TCHAR* cstr,size_t len)
	{
		TSTRING::append(cstr, len);
		return *this;
	}
	CLeeString substr(size_type _Off , size_type _Count )
	{
		return TSTRING::substr(_Off,_Count);
	}
	CLeeString substr(size_type _Off , size_type _Count ) const
	{
		return TSTRING::substr(_Off,_Count);
	}
	friend CLeeString operator+(const CLeeString &lhs, const CLeeString& rhs)
	{ 
		CLeeString tmp;
		tmp +=lhs;
		tmp+=rhs;
		return tmp;
	}
	friend CLeeString operator+(const CLeeString &lhs,  CLeeString& rhs)
	{ 
		CLeeString tmp;
		tmp +=lhs;
		tmp+=rhs;
		return tmp;
	}
	friend CLeeString operator+(const CLeeString &lhs, const TCHAR* rhs)
	{ 
		CLeeString tmp;
		tmp +=lhs;
		tmp+=rhs;
		return tmp;
	}
	friend CLeeString operator+(const CLeeString &lhs,  TCHAR*rhs)
	{ 
		CLeeString tmp;
		tmp +=lhs;
		tmp+=rhs;
		return tmp;
	}
	friend bool operator==(const CLeeString &lhs,const TCHAR* rhs)
	{
		const size_t la = lstrlen(rhs);
		const size_t lb = lhs.length();
		if (la == lb)
			return ( lhs.compare(rhs )==0 );
		return false;
	}
	
	friend bool operator==(const CLeeString &lhs,const CLeeString& rhs)
	{
		const size_t la = rhs.length();
		const size_t lb = lhs.length();
		if (la == lb)
			return ( lhs.compare(rhs.data() )==0 );
		return false;
	}
       void Format(TCHAR* fmt,...)
       {
          TCHAR  Buffer[MAX_PATH*100];
           memset(Buffer,0,sizeof(Buffer));
          va_list va;
          va_start(va, fmt);
          _vsntprintf(Buffer,sizeof(Buffer),fmt, va);
          va_end(va);
          TSTRING::assign(Buffer);
        }
	   void Trim(void)
	   {
		   TrimLeft();
		   TrimRight();
	   }
	   void TrimLeft(void)
	   {
		   const TCHAR* p =c_str();
		    const TCHAR* p1 =c_str();
		   // 找到第一个不是空格的字符
		   while(*p==_T(' '))
		   {
			   p++;
		   }
		 erase(0,p-p1);
	   }
	   void TrimRight(void)
	   {
		   const TCHAR* p =c_str();
		   const  TCHAR* rp =c_str();
		   const  TCHAR* rp1 =c_str();
		   // 找到最后一个不是空格的字符
		   while(*rp!=_T('\0'))
		   {
			   rp++;
		   }
		   rp--;
		   p=rp;
		   while(*rp==TEXT(' '))
		   {
			   rp--;
		   }

		   erase(rp+1-rp1,p-rp);
	   }
	   CLeeString Upcase(void)
	   {
		   CLeeString dst;
		   transform(begin(), end(), back_inserter(dst), ::toupper);
			return dst;

	   }
	   CLeeString Lowcase(void)
	   {
		   CLeeString dst;
		   transform(begin(), end(), back_inserter(dst), ::tolower);
			   return dst;

	   }
};
