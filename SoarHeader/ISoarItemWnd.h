#pragma once
#include "ISoarWnd.h"
/*
一个抽象，用于combobox，list 的项
*/
class ISoarItemWnd :
	public ISoarWnd
{
public:
	
    virtual DWORD getIndex(void) =0;
	virtual DWORD getsubIndex(void) =0;
	virtual DWORD getID(void) =0;
	virtual LPCTSTR getString(void) =0 ;
	virtual LPVOID getData(void) = 0;
	virtual int getHeight(void) =0;
	virtual int getWidght(void) =0 ;
	virtual void setIndex(DWORD dwIndex) =0;
	virtual void setsubIndex(DWORD dwIndex) =0;
	virtual void setID(DWORD dwId) =0;
	virtual void setString(const CLeeString & str) =0;
	virtual void setData(LPVOID data) =0;
	virtual void setHeight(int h) =0;
	virtual void setWidght(int w) =0;
	virtual bool getCheck(void) =0;
	virtual void setcheck(bool bcheck) =0;
    virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd)=0;

};
