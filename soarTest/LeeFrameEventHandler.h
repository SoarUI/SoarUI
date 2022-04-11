#pragma once
#include "../SoarHeader/iSoareventhandler.h"

class CLeeFrameEventHandler :
	public ISoarEventHandler
{
public:
	CLeeFrameEventHandler(void);
public:
	virtual ~CLeeFrameEventHandler(void);
	virtual LRESULT CallbackEvent ( ISoarWnd* pOwner,UINT ,WPARAM ,LPARAM ,LRESULT&);
};
