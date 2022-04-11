#pragma once
#include "../SoarHeader/iSoaritemwnd.h"
#include "SoarWnd.h"
class CLeeStaticItemWnd :
	public CSoarWnd
{
public:
	CLeeStaticItemWnd(HWND root,ISoarEngine* Eng);
	~CLeeStaticItemWnd(void);
	virtual DWORD getIndex(void) ;
	virtual DWORD getsubIndex(void) ;
	virtual DWORD getID(void) ;
	virtual CLeeString getString(void) ;
	virtual LPVOID getData(void) ;
	virtual int getHeight(void) ;
	virtual int getWidght(void) ;
//
	virtual void setIndex(DWORD dwIndex) ;
	virtual void setsubIndex(DWORD dwIndex) ;
	virtual void setID(DWORD dwId) ;
	virtual void setString(const CLeeString & str) ;
	virtual void setData(LPVOID data) ;
	virtual void setHeight(int h) ;
	virtual void setWidght(int w) ;
	virtual bool getCheck(void);
	virtual void setcheck(bool bcheck);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_OwnerWnd = pOwnerWnd;}
	virtual LRESULT HandleEvent( UINT ,WPARAM ,LPARAM ) ;
protected:
	DWORD d_ID ;
	CLeeString d_string;
	LPVOID d_data;
	DWORD d_iIndex;
	DWORD d_subindex;
	int d_width;
	int d_height;
	bool d_checkState;
	ISoarWnd * d_OwnerWnd;
};
