#include "Soar.h"
#include "SoarRoot.h"
#include "SoarCore_xmlHandler.h"
#include "resource.h"
#include "xmlparser\Interpreter.h"
static HMODULE g_staticHmodule= NULL;
CSoarEnv* CSoar::theSoarEnv =NULL;
CLeeString g_SoarUI_modulePath;
typedef HRESULT (*DLLCREATE)(LPVOID *ppReturn);
CSoar::CSoar(HMODULE hModule):d_hModule(hModule),d_handle(NULL),
theDrawEngine(NULL)
{
	g_staticHmodule = hModule;
	theSoarEnv = new CSoarEnv;
}

CSoar::~CSoar(void)
{
	delete theSoarEnv;
	delete theDrawEngine;//注意顺序
	::FreeLibrary((HMODULE)d_handle);
	delete d_parser;
}
bool CSoar::InitEnv(void)
{
	char	szLibPath[MAX_PATH];	// Full DLL path
	// Get full path of the DLL not the exe path
	if (!GetModuleFileName( d_hModule , szLibPath, MAX_PATH))
		return 0 ;
	char* p=_tcsrchr(szLibPath,TEXT('\\') ) + 1 ;
	CLeeString szFullName;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	_splitpath( szLibPath, drive, dir, fname, ext );
	szFullName.Format("%s%s\\%s",drive,dir,"SoarEngine.dll");
	g_SoarUI_modulePath.Format("%s%s\\%s",drive,dir,"SoarUI.bmp");
	DLLCREATE dllCreate;
	d_handle=(HANDLE)::LoadLibrary(szFullName.c_str());
	DWORD d =GetLastError();
	if (!d_handle)
	{
		::MessageBoxA(NULL,"渲染引擎加载失败","",MB_OK);
		return false;
	}
	dllCreate =(DLLCREATE)::GetProcAddress((HMODULE)d_handle,"create");
	if (dllCreate)
	{
		dllCreate((void**)&theDrawEngine);
	}
	//初始化内建XML解析器
	d_parser =new CInnerInterpreter();
	theDrawEngine->SetDefaultPaser(d_parser);
	theDrawEngine->SetThreadPool(NULL);
	return theDrawEngine;
}
void CSoar::UnLoadEnv(void)
{
	
}
ISoarRoot* CSoar::createRootWindow(HWND hmainwindow,const CLeeString& cfgfile)
{
	CSoarCore_xmlHandler  xnl(theDrawEngine);
	d_parser->parseFile(xnl,cfgfile,0);
	return theSoarEnv->createRoot(hmainwindow,theDrawEngine);
}
//通过已有窗口创建SoarUI附件窗口(嵌入)
ISoarRoot* CSoar::createSoarUIRootWindow(HWND hmainwindow,const CLeeString& cfgfile,int x,int y,int W ,int H)
{
	CSoarCore_xmlHandler  xnl(theDrawEngine);
	d_parser->parseFile(xnl,cfgfile,0);
	return theSoarEnv->createSoarUIRoot(hmainwindow,theDrawEngine,x,y,W,H);
}
ISoarRoot* CSoar::getRootWindow(HWND hmainwindow)
{
	return theSoarEnv->getRootFromHandle(hmainwindow) ;
}
LRESULT CALLBACK CSoar::sLeeRootWndProc( HWND hWnd , UINT uMsg ,
										 WPARAM wParam , LPARAM lParam )
{
	ISoarRoot * lpWindow = NULL ;
	lpWindow = theSoarEnv->getRootFromHandle(hWnd ) ;
	if ( lpWindow != NULL )
	{
		switch ( uMsg )
		{
			case WM_NCDESTROY :
			{
				//删除全局MAP,必须先删除
				theSoarEnv->RemoveFromTrackMap(hWnd) ;
				goto LROOTPROC;
			}
			case WM_ACTIVATE:
			{
				if(LOWORD(wParam)!=WA_INACTIVE) 
				{ 
					//通知窗口重新绘制
					InvalidateRect(hWnd,NULL,false);
					::SendMessage(hWnd,WM_NULL,0,0);
				}
				break;
			}
			case WM_ACTIVATEAPP:
			{
				//通知窗口重新绘制
				InvalidateRect(hWnd,NULL,false);
				::SendMessage(hWnd,WM_NULL,0,0);
				break;
			}
			//S WM_POWERBROADCAST (long)dwPowerEvent:PBT_APMRESUMEAUTOMATIC dwData:00000000
			case WM_POWERBROADCAST:
			{
				//不在休眠
				if(wParam ==PBT_APMRESUMEAUTOMATIC)
				{
					::PostMessage(hWnd,WM_SYSCOMMAND,SC_MINIMIZE,0);
					break;
				}
				//设备恢复(D3D)
				break;
			}

		}
		
		//其余留处理函数分流
		return lpWindow->SoarUIMsgProc(uMsg , wParam , lParam ) ;
		//goto LROOTDEFPROC;
	}
LROOTPROC:
	if(uMsg == WM_PAINT)
	{
		RECT rc;
		GetClientRect(hWnd,&rc);
		HBITMAP hBmp = ::LoadBitmap(g_staticHmodule, g_SoarUI_modulePath.c_str());
		HDC hDC = ::GetDC(hWnd);//显示图像的窗口HWND.
		HDC hMemDC = ::CreateCompatibleDC(hDC);
		HBITMAP hBmpOld = (HBITMAP)::SelectObject(hMemDC, hBmp);
		::BitBlt(hDC, 0, 0, rc.right-rc.left, rc.bottom-rc.top, hMemDC, 0, 0, SRCCOPY);
		::SelectObject(hMemDC, hBmpOld);
		::DeleteObject(hBmp);
		::DeleteObject(hMemDC);
		::ReleaseDC(hWnd, hDC);
	}
LROOTDEFPROC:
	return ::DefWindowProc( hWnd , uMsg , wParam , lParam ) ;
}
ISoarRoot* CSoar::createRootWindow(const CLeeString& cfgfile,CLeeString szTitle,DWORD style,int x,int y,int W ,int H)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW|CS_DBLCLKS;
	wcex.lpfnWndProc	=  CSoar::sLeeRootWndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= GetModuleHandle(NULL);
	wcex.hIcon			= LoadIcon( NULL, IDI_APPLICATION );
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	//wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_FRYDREAMTEST);
	wcex.lpszMenuName	= 0;
	CLeeString sTitleName;
	sTitleName.Format("Soar_lowLevelWnd_%p",this);
	wcex.lpszClassName	= sTitleName.c_str();
	wcex.hIconSm		= NULL;
   //注册窗口类
	if(!RegisterClassEx(&wcex) )
	{
		return NULL;
	}
	RECT rect={x,y,0,0};
	rect.right=W;
	rect.bottom=H;
	
	style |=WS_POPUP|/*WS_OVERLAPPEDWINDOW|*/WS_VISIBLE;
	AdjustWindowRect(&rect,style,false);
	HWND hWnd = CreateWindow(sTitleName.c_str(), szTitle.c_str(), style,
		rect.left, rect.top,rect.right-rect.left, rect.bottom-rect.top, NULL, NULL, GetModuleHandle(NULL), NULL);

	if (!hWnd)
	{
		return NULL;
	}
	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);
	return createRootWindow(hWnd,cfgfile);
}
int CSoar::DoMessgeloop(void)
{
	DWORD dwTick =GetTickCount();
	DWORD dwTickprev =GetTickCount();
	MSG msg ={0};
	// 主消息循环:
	while( WM_QUIT != msg.message )
	{
		dwTick =GetTickCount();
		if (CSoarRoot::getSingletonPtr())
		{
			CSoarRoot::getSingletonPtr()->RunSoarMessageSytem(); 
		}
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			RenderScene_Clear();
			RenderScene_Begin();
			if (CSoarRoot::getSingletonPtr())
			{
				// Do some rendering
				CSoarRoot::getSingletonPtr()->SoarUIRenderLoop(dwTick-dwTickprev);
			}
			RenderScene_End();
			RenderScene_Present();
		}
		
		dwTickprev =dwTick ;
		
	}
	return (int) msg.wParam;
}
int CSoar::DoMessageUILoop(ISoarLoopRunner* pGameLoop)
{
	DWORD dwTick =GetTickCount();
	DWORD dwTickprev =GetTickCount();
	MSG msg ={0};
	// 主消息循环:
	while( WM_QUIT != msg.message )
	{
		dwTick =GetTickCount();
		if (CSoarRoot::getSingletonPtr())
		{
			CSoarRoot::getSingletonPtr()->RunSoarMessageSytem(); 
		}
		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}
		else
		{
			DWORD t =dwTick-dwTickprev;
			RenderScene_Clear();
			RenderScene_Begin();
			if(pGameLoop)
				pGameLoop->RunLoop(t);
			if (CSoarRoot::getSingletonPtr())
			{
				// Do some rendering
				CSoarRoot::getSingletonPtr()->SoarUIRenderLoop(t);
			}
			RenderScene_End();
			RenderScene_Present();
		}
		
		dwTickprev =dwTick ;
		
	}
	return (int) msg.wParam;
}
bool CSoar::CloneXmlParser(ILeeXmlParser** pRet)
{
	if (!theDrawEngine)
	{
		*pRet =NULL;
		return false;
	}
	return theDrawEngine->CloneXmlParser(pRet);
}
void CSoar::RunSoarMessageSystem(void) 
{
	if (CSoarRoot::getSingletonPtr())
	{
		// Do some rendering
		CSoarRoot::getSingletonPtr()->RunSoarMessageSytem();
	}
}
void CSoar::RunSoarUIRenderSystem(float t) 
{
	if (CSoarRoot::getSingletonPtr())
	{
		// Do some rendering
		CSoarRoot::getSingletonPtr()->SoarUIRenderLoop(t);
	}
}
int CSoar::RunSoarUIModelDialogSystem(ISoarWnd* hMainWnd,ISoarLoopRunner* pGameLoop)
{
	if (CSoarRoot::getSingletonPtr())
	{
		// Do some rendering
		return CSoarRoot::getSingletonPtr()->DoModal(hMainWnd,pGameLoop);
	}
	return 0;
}
//开始屏幕绘制
void CSoar::RenderScene_Begin(void) 
{
	if(theDrawEngine)
		theDrawEngine->beginScene();
}
	//结束屏幕绘制
void CSoar::RenderScene_End(void)
{
	if(theDrawEngine)
		theDrawEngine->endScene();
}
void CSoar::RenderPatch_Begin(void) {
	if(theDrawEngine)
		theDrawEngine->beginPatch();
}
	//结束批量绘制
void CSoar::RenderPatch_End(void){
	if(theDrawEngine)
		theDrawEngine->endPatch();
}
bool CSoar::Render_AlphaBlendEnable(bool bEnable){
	if(theDrawEngine)
		return theDrawEngine->EnableAlphaBlend(bEnable);
	return false;
}
bool CSoar::Render_GetAlphaBlendState()
{
	if(theDrawEngine)
		return theDrawEngine->GetAlphaBlendState();
	return false;
}
void CSoar::RenderScene_Clear(void)
{
	if(theDrawEngine)
		theDrawEngine->clearScene();
}
void CSoar::RenderScene_Present(void) 
{
	if(theDrawEngine)
		theDrawEngine->Present();
}
void CSoar::InitRenderEx( LPVOID d3dDevice  )
{
	if(theDrawEngine)
		theDrawEngine->InitRenderEx(d3dDevice);
}
void CSoar::InitRender( HWND hwnd)
{
	if(theDrawEngine)
		theDrawEngine->InitRender(hwnd);
}
