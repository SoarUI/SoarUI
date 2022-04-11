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
	DWORD usageMSK;//��;
	CLeeString stringText;//�ı�
	bool isVertFont;//�Ƿ�ֱ����
	DWORD d_fmt;//�ı���ʽ
	DWORD d_aRGB;//��ɫARGB˳��
	DWORD d_windowColor;//RGBA��ʽ���
};
typedef struct MSControlContext 
{
	MSControlContext():styleEx(0),style(0),hWindow(0),MenuState(0),bActivateMenuState(0),
		bPopupmenu(0),bMenubar(0),bMenuBar_MDI(0),isMdiCloseEnabled(0)
	{}
	//titlebar
	bool bhasTitlebar;//�б�����
	//sysmenu
	bool bhasSysMenu;//��ϵͳ�˵�
	bool isZoomed;//���
	bool isIconiced;//��С��
	bool isMaxEnabled;//������󻯰�ť
	bool isMinEnabled;//������С����ť
	bool isHelpEnabled;//���������ť
	bool isIconEnabled;//
	bool isControlEnabled;//ȫ�ּ���
	bool isMdiCloseEnabled;//ʹ��MDI close button
	//
	DWORD style;
	DWORD styleEx;
    HWND  hWindow;
	//menu--menubar
	DWORD MenuState;
	bool  bActivateMenuState;
	bool  bPopupmenu;
	bool  bMenubar;
	bool  bMenuBar_MDI;//MDI�Ӵ��������,����isMaxEnabled �ȼ�������
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