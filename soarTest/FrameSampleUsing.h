#pragma once
#include "../SoarHeaDER/ISoarWnd.h"
#include "../SOARHEADER/ISoarRoot.h"
#include "LeeFrameEventHandler.h"
class CFrameSampleUsing
{
public:
	CFrameSampleUsing(ISoarRoot* Root,ISoarWnd* mainWnd);
	~CFrameSampleUsing(void);
public:
	ISoarWnd* d_frameWnd;
	ISoarWnd* d_userStatic;
	ISoarWnd* d_passStatic;
	ISoarWnd* d_userEdit;
	ISoarWnd* d_passEdit;
	ISoarWnd* d_confirmButton;
	ISoarWnd* d_AlphaTestWnd;
	ISoarWnd* d_FileDialogWnd;
	ISoarWnd* d_EnableAlphaButton;
	CLeeFrameEventHandler hand;
};
