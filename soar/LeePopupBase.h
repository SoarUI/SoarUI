#pragma once
#include "Soarwnd.h"
/*���������࣬����ʵ�ֵ���Ч�� 2016.8.31 0��41����ɽ�а��ɵ������޹�˾ */
class CLeePopupBase :
	public CSoarWnd
{
public:
	CLeePopupBase(HWND root,ISoarEngine* Eng);
	~CLeePopupBase(void);
	virtual RECT getWindowRect(void);
	virtual RECT getCliperRect(void);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
};
