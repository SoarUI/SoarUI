#pragma once
#include "ISoarWnd.h"
class ISoarWnd;
class ISoarEventHandler
{
public:
	ISoarEventHandler(void){}
	virtual ~ISoarEventHandler(void){}
	virtual LRESULT CallbackEvent ( ISoarWnd* pOwner,UINT ,WPARAM ,LPARAM ,LRESULT& )=0 ;
};
