#include "SoarEnv.h"
#include "SoarRoot.h"

CSoarEnv::CSoarEnv(void)
{
}

CSoarEnv::~CSoarEnv(void)
{
	LeeRootMap::iterator it = d_TrackRootWinList.begin();
	while (it != d_TrackRootWinList.end())
	{
		ISoarRoot *root = (*it).second;
		d_TrackRootWinList.erase(it);
		delete root;
		it = d_TrackRootWinList.begin();
	}
}
ISoarRoot* CSoarEnv::getRootFromHandle (HWND hWind) 
{
	d_HwndCcSection.Enter();
	ISoarRoot *root =NULL;
	LeeRootMap::iterator it = d_TrackRootWinList.find(hWind);
	if (it != d_TrackRootWinList.end())
	{
		root= (*it).second;
	}
	d_HwndCcSection.Leave();
	return root;
}
ISoarRoot* CSoarEnv::createRoot(HWND h,ISoarEngine *Eng)
{
	d_HwndCcSection.Enter();
	ISoarRoot *root =NULL;
	LeeRootMap::iterator it = d_TrackRootWinList.find(h);
	if (it == d_TrackRootWinList.end())
	{
		root =new CSoarRoot(Eng);
		if (root->attachMsWindow(h))
		 d_TrackRootWinList[h]=root;
	}
	else
	{
		root= (*it).second;
	}
	d_HwndCcSection.Leave();
	//加载默认资源
	Eng->engine_loadDefaultTheme();
	return root;
}
ISoarRoot* CSoarEnv::createSoarUIRoot(HWND h,ISoarEngine *Eng,int x,int y,int W ,int H)
{
	d_HwndCcSection.Enter();
	ISoarRoot *root =NULL;
	LeeRootMap::iterator it = d_TrackRootWinList.find(h);
	if (it == d_TrackRootWinList.end())
	{
		root =new CSoarRoot(Eng);
		if (root->CreateSoarUI(h,x,y,W,H))
		 d_TrackRootWinList[h]=root;
	}
	else
	{
		root= (*it).second;
	}
	d_HwndCcSection.Leave();
	//加载默认资源
	Eng->engine_loadDefaultTheme();
	return root;
}
ISoarRoot* CSoarEnv::CreateSoarUI(HWND hWind,ISoarEngine *Eng,Dimension& x,Dimension& y,Dimension& w,Dimension& h)
{
	d_HwndCcSection.Enter();
	ISoarRoot *root =NULL;
	LeeRootMap::iterator it = d_TrackRootWinList.find(hWind);
	if (it == d_TrackRootWinList.end())
	{
		root =new CSoarRoot(Eng);
		if (root->CreateSoarUI(hWind,x,y,w,h))
		 d_TrackRootWinList[hWind]=root;
	}
	else
	{
		root= (*it).second;
	}
	d_HwndCcSection.Leave();
	//加载默认资源
	Eng->engine_loadDefaultTheme();
	return root;
}
void CSoarEnv::RemoveFromTrackMap ( HWND hWnd ) 
{
	ISoarRoot *root =NULL;
	d_HwndCcSection.Enter();
	LeeRootMap::iterator it = d_TrackRootWinList.find(hWnd);
	if (it == d_TrackRootWinList.end())
	{
		d_HwndCcSection.Leave();
		return  ;
	}
	else
	{
		root= (*it).second;
		d_HwndCcSection.Leave();
        d_TrackRootWinList.erase(it);
		delete root;
	}
	
}