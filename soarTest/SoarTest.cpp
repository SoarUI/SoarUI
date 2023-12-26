// FrydreamTest.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "SoarTest.h"

#define MAX_LOADSTRING 100
// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

#include "../threadpool\leePluginMgr.h"
#include "../SoarHeader/ISoar.h"
//#include "CmyBut.h"
#include "LeeFrameEventHandler.h"
#include "FrameSampleUsing.h"
#include "../SoarHeader/SoarExport.h"
CLeePluginMgr Plugs;
typedef HRESULT  (/*__cdecl */* DUIFUNC)(LPVOID *r);
HWND hMain =NULL;
CFrameSampleUsing *s;
ISoarRoot * root= NULL;
ISoar * pDream =NULL;
ISoarProgress* d_dymanicProgress; 
ISoarProgress* d_dymanicProgressV; 
ISoarSpinWnd* d_spinWnd;
ISoarSliderWnd* d_sliderWnd;
//#define EMBEDSYSTEM
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	
#ifdef _DEBUG
	Plugs.load("Soard.dll", "Soar");
#else
	Plugs.load("Soar.dll", "Soar");
#endif
	DUIFUNC dui;
	dui = (DUIFUNC)Plugs.getFunction("Soar", "create");
	assert(dui != NULL);

	dui((void**)&pDream);
#ifdef EMBEDSYSTEM
	MSG msg ={0};
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_FRYDREAMTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}
	
	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_FRYDREAMTEST));

	
	if (pDream)
	{
		pDream->InitEnv();
#ifdef _DEBUG
		root = pDream->createSoarUIRootWindow(hMain, "./LeeUIConfig/Soard.core", 0, 0, 800, 600);
#else
		root = pDream->createSoarUIRootWindow(hMain, "./LeeUIConfig/Soar.core", 0, 0, 800, 600);
#endif
		
		 hMain =root->getMsWindow();
		pDream->InitRender(root->getMsWindow());
		s=new CFrameSampleUsing(root,root->getSoarSheet());
	}
	 UpdateWindow(hMain);
	 ShowWindow(hMain,SW_SHOW);
	 DWORD dwTick =0;
	 const int Min_Val=1;
	 const int Max_Val=99;
	 int srandPos=0;
	// 主消息循环:
	while( WM_QUIT != msg.message )
	{
		if (root)
			root->RunSoarMessageSytem();
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			if (::GetTickCount()-dwTick>32)
			{
				if (root)
				{

					//设置随机进程条
					srandPos=rand()%(Max_Val-Min_Val+1)+Min_Val;
					d_dymanicProgress->setPos(srandPos);
					d_dymanicProgressV->setPos(srandPos);
					pDream->RenderScene_Clear();
					root->beginScene();
					pDream->RenderScene_Begin();
					//root->RunSoarMessageSytem();
					root->SoarUIRenderLoop(1.0);
					pDream->RenderScene_End();
					root->endScene();
					pDream->RenderScene_Present();
				}
			   dwTick =::GetTickCount();
			}
		}
	}
#else
	if (pDream)
	{
		pDream->InitEnv();
#ifdef _DEBUG
		root = pDream->createRootWindow("./LeeUIConfig/Soard.core","Soar",0,150,150,800,600);
		//root = pDream->createRootWindow("./LeeUIConfig/Soard.core", "Soar", WS_POPUP, 150, 150, 800, 600);
#else
		root = pDream->createRootWindow("./LeeUIConfig/Soar.core","Soar",WS_POPUP,150,150,800,600);
		//root = pDream->createRootWindow("./LeeUIConfig/Soar.core","Soar",0,150,150,800,600);
#endif
		hMain = root->getMsWindow();
		pDream->InitRender(root->getMsWindow());
		s = new CFrameSampleUsing(root, root->getSoarSheet());
		int r = pDream->DoMessgeloop();
	}
#endif
	//
   pDream->UnLoadEnv();
   delete pDream;
	return (int) 1;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FRYDREAMTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FRYDREAMTEST);
	wcex.lpszMenuName	= 0;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中
  DWORD style =WS_POPUP|WS_VISIBLE|WS_OVERLAPPEDWINDOW;
   hWnd = CreateWindow(szWindowClass, szTitle, style,
      CW_USEDEFAULT, 0,800, 600, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   hMain = hWnd;
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		{PAINTSTRUCT ps;
		RECT rc;
		GetClientRect(hWnd,&rc);
		HDC hdc=BeginPaint(hWnd,&ps);
		char pHello[]="hello world ! welcome to FlyDream world !";
		DrawText(hdc,pHello,sizeof (pHello),&rc,DT_CENTER|DT_VCENTER);
		EndPaint(hWnd,&ps);
		break;}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		if(root )
			root->InJectMsg(message, wParam, lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
