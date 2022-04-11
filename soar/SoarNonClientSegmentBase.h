#pragma once
#include "Soarwnd.h"
/*处理作为子控件嵌入控件地区域获取问题*/
class CSoarNonClientSegmentBase 
{
public:
	typedef	std::map<int, SoarSegmentItem>	LeeItemWndMap;
	CSoarNonClientSegmentBase(ISoarWnd *parent,ISoarEngine* Eng);
	virtual ~CSoarNonClientSegmentBase(void);
	virtual RECT getClientRect(void) ;//获取相对于父窗口的矩形
	virtual RECT getWindowRect(void) ;
	virtual RECT getCliperRect(void);
protected:
	bool d_lbuttondown;
	ISoarWnd* d_OwnerWnd;
	CLeeString d_wndlook;//对应皮肤控件
	CLeeString d_wndlookState;//皮肤状态
	ComponentArea d_area;//保存统一坐标信息
	ISoarEngine *d_DrawEng;//作图组件
	SEGMENTITEMS d_items;
	LWNDT d_wndtype;//窗口类型
};
