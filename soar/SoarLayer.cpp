#include "SoarLayer.h"
#include "SoarRoot.h"
#include "ISoarSheet.h"

CSoarLayer::CSoarLayer(ISoarSheet* sheet):d_pSheet(sheet)
{
}


CSoarLayer::~CSoarLayer(void)
{
}
bool CSoarLayer::seekWnd(ISoarWnd* newWnd)
{
	if(!newWnd)
		return false;
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while(it !=d_WndPtrs.end())
	{
		ISoarWnd * pfWnd =it->second ;
		if(newWnd==pfWnd){
			return true;
		}
		++it;
	}
	return false;
}
bool CSoarLayer::seekWnd(const CLeeString & name)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(name );
	if (it !=d_WndPtrs.end())
	{
		return true;
	}
	return false;
}
void CSoarLayer::removeWnd(ISoarWnd* newWnd)
{
	eraseWnd(newWnd);
}
ISoarWnd* CSoarLayer::getWnd(const CLeeString & name)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(name);
	if (it !=d_WndPtrs.end())
	{
		return (*it).second;
	}
	return NULL;
}
void CSoarLayer::addWnd(ISoarWnd* newWnd)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.find(newWnd->getWndName());
	if (it !=d_WndPtrs.end())
	{
		return ;
	}
	d_WndPtrs[newWnd->getWndName()] =newWnd;
}
void CSoarLayer::eraseWnd(ISoarWnd* pWnd)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while(it !=d_WndPtrs.end())
	{
		ISoarWnd * pfWnd =it->second ;
		if(pWnd==pfWnd){
			d_WndPtrs.erase(it);
			break;
		}
		++it;
	}
}
void CSoarLayer::eraseAllWnd(void)
{
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while(it !=d_WndPtrs.end())
	{
		ISoarWnd * pWnd =it->second ;
		it =d_WndPtrs.erase(it);
	}
}
bool CSoarLayer::handlerClickMsg(MSG msg,POINT &pt)
{
	LeeWndPtrMap::reverse_iterator it =d_WndPtrs.rbegin();
	
	ISoarWnd * d_prevFocus =d_pSheet->getFocusWnd();
	ISoarWnd * findWnd =NULL;
	while( it != d_WndPtrs.rend())
	{
	////////////////////////////////////////////////////////////////////////////�ȴ���ϵͳ���ⴰ��
	//�ò㴰�����Ƿ��д��ڱ�����
		if ( (it->second)->getState()>LWNDST_DISABLED)
		{
			findWnd =it->second;
			RECT rc=findWnd->getCliperRect();
			RectF rcc =findWnd->getWindowRect();
			rcc=rcc.Intersection(rc);
			rcc.toWindowRect(rc);
			if (::PtInRect(&rc,pt))
			{
				//�ǼǼ���ô��ڣ���������ع���
				d_pSheet->setFocusWnd(findWnd);
				return true;
			}	
		}	
		++it;
	}
	return false;
}
//����Ϣ��Soarר����Ϣ��ֻ����UI״̬���������¼�
 bool CSoarLayer::filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam)
 {
	 if (uMsg != WM_MOUSEMOVE)
	 {
		 return false;
	 } 
	 INT iIndex =0;
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(),&pt);
	 ISoarWnd * lp_tmpWnd =NULL;
	 //�ȼ�ⵯ�����ڵ�״̬
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while (it != d_WndPtrs.end())
	{
		lp_tmpWnd =(it->second);
		//�Ӵ������Ƿ��д��ڱ�����
		if (lp_tmpWnd->getState()>LWNDST_DISABLED ) 
		{
			//���״̬������Ҫ�ڷ�Χ�ڣ�
			lp_tmpWnd->checklookState(pt);
		}	
		++it;
	}
	return false;
 }
 //Ϊ���Ż���SOAR�����Ʊ���
 bool CSoarLayer::filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam)
 {
	 if (uMessage != WM_PAINT)
	 {
		 return false;
	 }
	
	 return 0;
 }
 bool CSoarLayer::RenderLoop(ILeeDrawInterface *DrawFuns,float &t)
{
	bool bPreState =DrawFuns->graphic_AlphaBlendState();
	DrawFuns->graphic_AlphaBlend(d_bEnableAlphaBlend);
	LeeWndPtrMap::iterator it =d_WndPtrs.begin();
	while(it !=d_WndPtrs.end())
	{
		ISoarWnd * pWnd =it->second ;
		if(pWnd && pWnd->getState()>LWNDST_HIDE)
			pWnd->DrawSelf(DrawFuns);
		++it;
	}
	DrawFuns->graphic_AlphaBlend(bPreState);
	return true;
}
  bool CSoarLayer::enableAlphaBlend(bool bEnable)
 {
	 if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 return true;
	 }
	 return false;
 }
