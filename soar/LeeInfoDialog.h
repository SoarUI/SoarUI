#pragma once
#include "Soarwnd.h"

class CLeeInfoDialog :
	public CSoarWnd
{
public:
	CLeeInfoDialog(HWND root,ISoarEngine* Eng);
	~CLeeInfoDialog(void);
	virtual void InitializeSegments(void);
	LRESULT HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam);
protected:
	ISoarWnd* d_ok;
	ISoarWnd* d_cancel;
};
