#ifndef _TextContext_h_
#define _TextContext_h_
#include <Windows.h>
#include "leeString.h"
#define MSK_TXT_GDIPLUS 0x00001L
#define MSK_TXT_GDI     0x00002L
#define MSK_TXT_D3D     0x00003L
#define MSK_TXT_GL      0x00004L
struct TextContext
{
	TextContext():cbsize(0),usageMSK(MSK_TXT_GDIPLUS),isVertFont(false),d_fmt(0)
	{}
	DWORD cbsize;
	DWORD usageMSK;//用途
	CLeeString stringText;//文本
	bool isVertFont;//是否垂直字体
	DWORD d_fmt;//文本格式
	DWORD d_aRGB;//颜色ARGB顺序
	DWORD d_windowColor;//RGBA格式存放
};
typedef struct MSControlContext 
{
	MSControlContext():styleEx(0),style(0),hWindow(0),MenuState(0),bActivateMenuState(0),
		bPopupmenu(0),bMenubar(0),bMenuBar_MDI(0),isMdiCloseEnabled(0)
	{}
	//titlebar
	bool bhasTitlebar;//有标题栏
	//sysmenu
	bool bhasSysMenu;//有系统菜单
	bool isZoomed;//最大化
	bool isIconiced;//最小化
	bool isMaxEnabled;//激活最大化按钮
	bool isMinEnabled;//激活最小化按钮
	bool isHelpEnabled;//激活帮助按钮
	bool isIconEnabled;//
	bool isControlEnabled;//全局激活
	bool isMdiCloseEnabled;//使用MDI close button
	//
	DWORD style;
	DWORD styleEx;
    HWND  hWindow;
	//menu--menubar
	DWORD MenuState;
	bool  bActivateMenuState;
	bool  bPopupmenu;
	bool  bMenubar;
	bool  bMenuBar_MDI;//MDI子窗口最大化了,激活isMaxEnabled 等几个参数
	//hittest
	POINT ptLocalCursor;
	//
}MSControlContext,*PMSControlContext;
enum WidgetStatus
{
	en_NORMAL,
	en_HOVER,
	en_Pressed,
	en_Disabled,
	en_SelectNormal,
	en_SelectHover,
	en_SelectPressed,
	en_SelectDisabled
};
#endif