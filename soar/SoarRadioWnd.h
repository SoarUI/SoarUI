#pragma once
//LWNDT_BUTTON
#include "SoarWnd.h"
#include "../SoarHeader/SoarExport.h"

class CLeeRadioWnd :
	public CSoarWnd
{
public:
	CLeeRadioWnd(HWND root,ISoarEngine* Eng);
	typedef	std::map<CLeeString, ISoarWnd*, CLeeString::FastLessCompare>	LeeGroupWndPtrMap;
public:
	virtual ~CLeeRadioWnd(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual bool getCheck(void);
	virtual void setCheck(bool bChecked);
	virtual void setRadioGroup(CLeeString & group);
protected:
	bool d_checked;
	CLeeString d_groupId;
	static LeeGroupWndPtrMap s_groupCheckedIDs;//已经选中的对象
};
