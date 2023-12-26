#pragma once
#include "Soarwnd.h"

class CLeeInfoDialog :
	public CSoarWnd
{
public:
	CLeeInfoDialog(HWND root,ISoarEngine* Eng);
	~CLeeInfoDialog(void);
	virtual void InitializeSegments(void);
	BOOL HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
protected:
	ISoarWnd* d_ok;
	ISoarWnd* d_cancel;
};
