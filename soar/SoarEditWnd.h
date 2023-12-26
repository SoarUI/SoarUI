#pragma once
#include "Soarwnd.h"
#include "../SoarHeader/SoarExport.h"
#include"../SoarHeader/leestring.h"
class CLeeEditWnd :
	public CSoarWnd
{
public:
	CLeeEditWnd(HWND root,ISoarEngine* Eng);
public:
	virtual ~CLeeEditWnd(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) ;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) ;
	virtual void isPassword(bool bpwd =false) ;
	int IsGBK(LPTSTR str);
protected:
	std::wstring d_unicodestrings;
	CLeeString d_maskstring;
	bool d_isPwd;
};
