#pragma once
#include "Soarwnd.h"
/*处理作为子控件嵌入控件地区域获取问题*/
class CLeeNonClientCtrlBase :
	public CSoarWnd
{
public:
	CLeeNonClientCtrlBase(HWND root,ISoarEngine* Eng);
	virtual ~CLeeNonClientCtrlBase(void);
	virtual RECT getClientRect(void) ;//获取相对于父窗口的矩形
	virtual RECT getWindowRect(void) ;
	virtual RECT getCliperRect(void);
};
