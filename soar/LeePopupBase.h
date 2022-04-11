#pragma once
#include "Soarwnd.h"
/*弹出窗口类，用于实现弹出效果 2016.8.31 0：41于中山市爱纳电器有限公司 */
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
