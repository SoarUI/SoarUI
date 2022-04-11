#pragma once
/*
*FlyDream系统核心绘制函数导出
*
*/
typedef void (*ILeeDrawWidget)(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect,const CLeeString& text,int vertTextFmt,int horzTextFmt);
typedef void (*ILeeDrawWidgetImg)(const CLeeString& elementName,const CLeeString& elementStatus,const RECT& ControlRect);
typedef bool (*ISoarAlphaBlendEnable)(bool bEnable);
typedef bool (*ISoarGraphicGetAlphaBlendState)(void);
struct ILeeDrawInterface
{
public:
	ILeeDrawInterface(void){}
	virtual ~ILeeDrawInterface(void){}
	HDC hdc;
  ILeeDrawWidget LeeDrawWidget;
  ILeeDrawWidgetImg LeeDrawWidgetImg;
  ISoarAlphaBlendEnable graphic_AlphaBlend;
  ISoarGraphicGetAlphaBlendState graphic_AlphaBlendState;
};
