#include "SoarSheet.h"
#include"SoarLayer.h"
#include "SoarRoot.h"

CSoarSheet::CSoarSheet(HWND root,bool bAutoDestroy):
d_rootWnd(root),d_bAutoDestroy(bAutoDestroy),d_sheetmainWnd(NULL),d_PrevSheetfocusWnd(NULL),d_SheetfocusWnd(NULL),
	d_baseZindex(0),d_zPopupIndexOld(0),d_zIndexOld(0),d_zIndexMax(1),d_popupzIndexMax(1),
	d_bModalState(false),d_baseTempZindex(0),d_bEnableAlphaBlend(false)//Ĭ��Ϊ��ͨ��,
{
	d_popuplayers.clear();
	d_layers.clear();
}


CSoarSheet::~CSoarSheet(void)
{
	//����
	if(d_bAutoDestroy)
	{
		LayerMap::reverse_iterator it;
		it =d_popuplayers.rbegin();
		while (it != d_popuplayers.rend() )//popup
		{
			ISoarLayer* layer = it->second;
			layer->eraseAllWnd();
			++it ;
		}
		it =d_layers.rbegin();
		while (it != d_layers.rend() )//popup
		{
			ISoarLayer* layer = it->second;
			layer->eraseAllWnd();
			++it ;
		}
	}
	LayerMap::iterator it;
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		delete layer;
		it= d_popuplayers.erase(it) ;
	}
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		delete layer;
		it= d_layers.erase(it) ;
	}
	
}
ISoarWnd* CSoarSheet::getPrevFocusWnd(void)
{
	return d_PrevSheetfocusWnd;
}
ISoarWnd* CSoarSheet::setPrevFocusWnd(ISoarWnd* pFocus)
{
	ISoarWnd * tmp = d_PrevSheetfocusWnd;
	d_PrevSheetfocusWnd = pFocus;
	return tmp;
}
void CSoarSheet::setModalState(bool bModal){
	d_bModalState=bModal;
}
ISoarWnd* CSoarSheet::getFocusWnd(void)
{
	return d_SheetfocusWnd;
}
ISoarWnd* CSoarSheet::setMainWnd(ISoarWnd* mainW)
{
	ISoarWnd * ptmp =d_sheetmainWnd;
	d_sheetmainWnd =mainW;
	if(d_sheetmainWnd->isPopupWnd()){
		
		d_sheetmainWnd->setPopupZindex(d_basePopupZindex);
		d_zIndexOld=d_basePopupZindex;
	}
	else{
		d_sheetmainWnd->setZindex(d_baseZindex);
		d_zIndexOld=d_baseZindex;
	}
	
	return ptmp;
}
ISoarWnd* CSoarSheet::getMainWnd(void)
{
	return d_sheetmainWnd;
}
void CSoarSheet::addWndToZlayer(int zpos,ISoarWnd* pWnd){
	LayerMap::iterator it;
	if (pWnd->isPopupWnd() )//popup
	{
		pWnd->setPopupZindex(zpos);
		it =d_popuplayers.find(zpos);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(pWnd);
		}
		else//�����Ƿ��Ѿ�������ͼ��
		{
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(pWnd);
			d_popuplayers[zpos]=layer;
		}
	}
	else
	{
		pWnd->setZindex(zpos);
		it =d_layers.find(zpos);
		if(it != d_layers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(pWnd);
		}
		else
		{
			it=d_layers.begin();
			while(it != d_layers.end() )
			{
				if(it->second->seekWnd(pWnd) )
				{	
					it->second->removeWnd(pWnd);
					break;
				}
				++it;
			}
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(pWnd);
			d_layers[zpos]=layer;
		}

	}
}
void CSoarSheet::addWnd(ISoarWnd* newWnd) {
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if (newWnd->isPopupWnd() )//popup
	{
		
		if(findWndfast(thisParent)&& thisParent->isPopupWnd()){
			tIndex= ensurePopupZindex(thisParent->getPopupZindex()+1);
		}
		else{
			tIndex=generatePopupZindex();
		}
	}
	else
	{
		if(findWndfast(thisParent)&&!thisParent->isPopupWnd() ){
			tIndex= ensureZindex(thisParent->getZindex()+1);
		}
		else{
			tIndex=generateZindex();
		}
	}
	addWndToZlayer(tIndex,newWnd);
}
void CSoarSheet::removeWnd(ISoarWnd* newWnd){
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			return;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			break;
		}
		++it;
	}
}
void CSoarSheet::eraseWnd(ISoarWnd* pWnd){
	removeWnd(pWnd);
}
void CSoarSheet::addWndRecursive(ISoarWnd* newWnd)
{
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if (newWnd->isPopupWnd() )//popup
	{
		
		if(findWndfast(thisParent)&& thisParent->isPopupWnd()){
			tIndex= ensurePopupZindex(newWnd->GetParent()->getPopupZindex()+1);
		}
		else{
			tIndex=generatePopupZindex();
		}
	}
	else
	{
		if(findWndfast(thisParent)&&!thisParent->isPopupWnd() ){
			tIndex= ensureZindex(thisParent->getZindex()+1);
		}
		else{
			tIndex=generateZindex();
		}
	}
	addWndToZlayer(tIndex,newWnd);
	//�������Ӵ��ڽ�����Ӵ���
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			addWndRecursive(itChild->second);
			++itChild;
		}
}
bool CSoarSheet::findWndfast(ISoarWnd* pWnd){
	if(pWnd==NULL)
		return false;
	if (pWnd->isPopupWnd() )//popup
	{
		LayerMap::iterator it =d_popuplayers.find(pWnd->getPopupZindex());
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			return layer->seekWnd(pWnd);
		}
	}
	else
	{
		LayerMap::iterator it =d_popuplayers.find(pWnd->getZindex());
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			return layer->seekWnd(pWnd);
		}

	}
	return false;
}
BOOL CSoarSheet::seekWnd(ISoarWnd* newWnd) 
{
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			return true;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			return true;
		}
		++it;
	}
	
	return false;
}
bool CSoarSheet::seekWnd(const CLeeString & name)
{
	LayerMap::iterator it;
	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(name) )
		{	
			return true;
		}
		++it;
	}
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(name) )
		{	
			return true;
		}
		++it;
	}
	
	return false;
}
ISoarWnd* CSoarSheet::getWnd(const CLeeString & name){
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	ISoarWnd* pFound =NULL;
	while(it != d_popuplayers.end() )
	{
		pFound=it->second->getWnd(name);
		if(pFound )
		{	
			return pFound;
		}
		++it;
	}
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		pFound=it->second->getWnd(name);
		if(pFound )
		{	
			return pFound;
		}
		++it;
	}
	
	return NULL;
}
void CSoarSheet::removeWndRecursive(ISoarWnd* newWnd) 
{
	LayerMap::iterator it;

	it=d_popuplayers.begin();
	while(it != d_popuplayers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			goto doremovechild;
		}
		++it;
	}
	
	it=d_layers.begin();
	while(it != d_layers.end() )
	{
		if(it->second->seekWnd(newWnd) )
		{	
			it->second->removeWnd(newWnd);
			break;
		}
		++it;
	}
	doremovechild:
	//�������Ӵ��ڽ�����Ӵ���
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			removeWndRecursive(itChild->second);
			++itChild;
		}
}
void CSoarSheet::eraseWndRecursive(ISoarWnd* newWnd) 
{
	//��ǰ�����Ƿ�Ϊ���㴰��
	if (d_SheetfocusWnd == newWnd)
	{
		setFocusWnd(d_sheetmainWnd);
	}
	removeWndRecursive(newWnd);
}
void CSoarSheet::clearAllLinkWnd(){
	LayerMap::iterator it;
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		layer->eraseAllWnd();
		++it;
	}
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		layer->eraseAllWnd();
		++it;
	}
}
BOOL CSoarSheet::SoarUIMsgProc ( UINT uMsg,WPARAM wParam ,LPARAM lParam,LRESULT& lr )
{
	//����WM_PAINT��Ϣ
	if (filter_PaintMessage(uMsg,wParam,lParam) )
	{
		lr = 0;
		return TRUE;
	}
	//��������ƶ���Ϣ����UI-�Ѿ��Ƴ�����Ⱦ��ˮ����
	///filter_MouseMoveUI(uMsg,wParam,lParam);
	
	//����ڵȵ����Ϣ����(Ԥ����)
	if( filter_ClickMsg(uMsg,wParam,lParam,lr) )
	{
		return TRUE;
	}
	//�ü���Ĵ��ڴ�����Ϣ
	if( filter_allMessage(uMsg,wParam,lParam,lr) )
	{
		return TRUE;
	}
	return false;
}
bool CSoarSheet::filter_ClickMsg(UINT uMsg ,WPARAM wParam,LPARAM lParam,LRESULT& lr)
{
	UINT tempMsg = uMsg;
#ifdef _WIN64
	if (uMsg == WM_SETCURSOR)
	{
		tempMsg = HIWORD(lParam);
	}
#endif
	if (tempMsg < WM_MOUSEFIRST )
	 {
		 return false;
	 }
	if (tempMsg >WM_MOUSELAST)
	{
		return false;
	}
#ifdef _WIN64
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(), &pt);
	lParam = MAKELPARAM(pt.x,pt.y);
#endif
	MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),tempMsg ,wParam,lParam};
	  //�����л�
	if(tempMsg ==WM_LBUTTONDOWN ||
		tempMsg ==WM_LBUTTONDBLCLK||
		tempMsg == WM_RBUTTONDOWN ||
		tempMsg == WM_RBUTTONDBLCLK ||
		tempMsg ==WM_XBUTTONDOWN ||
		tempMsg == WM_XBUTTONDBLCLK )
	{ 
		handlerClickMsg(msg);
      //������Ϣ
	 fireUIEvent_COMMAND(0.0,msg);
	}
	if(tempMsg ==WM_LBUTTONUP ||
		tempMsg == WM_RBUTTONUP ||
		tempMsg == WM_MBUTTONUP ||
		tempMsg == WM_XBUTTONUP )
	{ 
      //������Ϣ
	  fireUIEvent_COMMAND(0.0,msg);
	}
	//�ü���ڴ�����Ϣ
	CSoarRoot::getSingletonPtr()->RunSoarMsg( msg ,lr);
	return true;
}
bool CSoarSheet::handlerClickMsg(MSG msg)
{
	POINT pt;
	::GetCursorPos(&pt);
	::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(),&pt);
	LayerMap::reverse_iterator it =d_layers.rbegin();
	//����Ȩ��
	it =d_popuplayers.rbegin();
	while( it != d_popuplayers.rend() )
	{
		if( it->second->handlerClickMsg(msg,pt))
			return true;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		if( it->second->handlerClickMsg(msg,pt))
			return true;
		++it;
	}
	return false;
}
bool CSoarSheet::filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	LayerMap::reverse_iterator it =d_popuplayers.rbegin();
	
	while( it != d_popuplayers.rend() )
	{
		 it->second->filter_MouseMoveUI( uMsg , wParam , lParam ) ;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		 it->second->filter_MouseMoveUI( uMsg , wParam , lParam ) ;
		++it;
	}
	return false;
}
bool CSoarSheet::filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam)
 {
	LayerMap::reverse_iterator it =d_popuplayers.rbegin();
	while( it != d_popuplayers.rend() )
	{
		it->second->filter_PaintMessage( uMessage , wParam , lParam ) ;
		++it;
	}
	it =d_layers.rbegin();
	while( it != d_layers.rend() )
	{
		it->second->filter_PaintMessage( uMessage , wParam , lParam ) ;
		++it;
	}
	return false;
 }
 //ֱ����ROOT���м��ɣ���ζ��ֻ������Ĵ������ж���
bool CSoarSheet::filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT &lr)
{
	MSG msg={CSoarRoot::getSingletonPtr()->getMsWindow(),uMsg ,wParam,lParam};
	//�ü���ڴ�����Ϣ
	return CSoarRoot::getSingletonPtr()->RunSoarMsg( msg ,lr);
}
//���ڲ�θı�
BOOL CSoarSheet::WndZorderChanged(ISoarWnd* FocusWnd,int newFocusZpos,int oldZindex)
{
	//���ҵ�֮ǰ�Ĳ㣬�����Ƴ�
	if( newFocusZpos==oldZindex)
		return false;
	int tIndex = oldZindex;
	LayerMap::iterator it;
	if (FocusWnd &&FocusWnd->isPopupWnd() )//popup
	{
		it =d_popuplayers.find(tIndex);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->removeWnd(FocusWnd);
		}
		else
		{
			it=d_popuplayers.begin();
			while(it != d_popuplayers.end() )
			{
				if(it->second->seekWnd(FocusWnd) )
				{	
					it->second->removeWnd(FocusWnd);
				}
				++it;
			}
		}
	}
	else
	{
			it =d_layers.find(tIndex);
			if(it != d_layers.end())
			{
				ISoarLayer* layer = it->second;
				layer->removeWnd(FocusWnd);
			}else
			{
				it=d_layers.begin();
				while(it != d_layers.end() )
				{
					if(it->second->seekWnd(FocusWnd) )
					{	
						it->second->removeWnd(FocusWnd);
					}
					++it;
				}
			}
	}
	//���ӵ��²�
	 tIndex = newFocusZpos;
	if (FocusWnd &&FocusWnd->isPopupWnd() )//popup
	{
		it =d_popuplayers.find(tIndex);
		if(it != d_popuplayers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(FocusWnd);
		}
		else
		{
			ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(FocusWnd);
				d_popuplayers[tIndex]=layer;
		}
	}
	else
	{
			it =d_layers.find(tIndex);
			if(it != d_layers.end())
			{
				ISoarLayer* layer = it->second;
				layer->addWnd(FocusWnd);
			}
			else{
				ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(FocusWnd);
				d_layers[tIndex]=layer;
			}
	}
	return true;
}
	//������ʽ�ı�
BOOL CSoarSheet::WndStyleChanged(ISoarWnd* newWnd) 
{
	return true;
}
bool CSoarSheet::RenderLoop(ILeeDrawInterface *DrawFuns,float &t)
{
	//���� һ������������Ⱦ�����Ӵ��ڣ�Z˳���׼ȷ����Ҫ��������
	//��������������Ⱦ��Ч�ʸߣ�����Ҫ����Z˳���׼ȷ��
	LayerMap::iterator it;
	it =d_layers.begin();
	while (it != d_layers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		if(layer)
		layer->RenderLoop(DrawFuns,t);
		++it;
	}
	it =d_popuplayers.begin();
	while (it != d_popuplayers.end() )//popup
	{
		ISoarLayer* layer = it->second;
		if(layer)
		layer->RenderLoop(DrawFuns,t);
		++it;
	}
	return true;
}
 bool CSoarSheet::enableAlphaBlend(bool bEnable)
 {
	 if(d_bEnableAlphaBlend != bEnable){
		 d_bEnableAlphaBlend =bEnable;
		 return true;
	 }
	 return false;
 }
/************************************************************************/
/* pFocus/d_focusWnd ������ΪNULL                                       */
/************************************************************************/
ISoarWnd* CSoarSheet::setFocusWnd(ISoarWnd* pFocus)
{
	//û�иı䣬ֱ�ӷ���
	if(pFocus == d_SheetfocusWnd)
	{
		//���ͼ�����Ϣ
		fireUIEvent_Focus(NULL);
		return pFocus;
	}
	ISoarWnd * d_prevFocus =d_SheetfocusWnd;
	d_SheetfocusWnd =pFocus;//�ǼǼ���ô���
	//�ָ�ԭ�����(�װ��Sheet��MainWnd��������)
	if (d_prevFocus && 
		d_prevFocus!=d_SheetfocusWnd && 
		(d_prevFocus != d_sheetmainWnd )//&& //ʧȥ����Ĳ��ǵװ�
		///d_SheetfocusWnd !=d_sheetmainWnd //�õ�����Ĳ��ǵװ�
		)
	{
		if (d_prevFocus->isPopupWnd() )//popup
		{
			//���м����sheet ������Ӧ��layer���
			//d_prevFocus->setPopupZindex(d_zPopupIndexOld);
			degradeLevelWnd(d_prevFocus,d_zIndexOld);
		}
		else
		{
			//���м����sheet ������Ӧ��layer���
			//d_prevFocus->setZindex(d_zIndexOld);
			degradeLevelWnd(d_prevFocus,d_zIndexOld);
		}
		
	}
	//Ϊ���㴰���������
	if (d_SheetfocusWnd && 
		(d_SheetfocusWnd != d_sheetmainWnd )
		)
	{
		bringWndToTopLevel(d_SheetfocusWnd);
		//if (d_SheetfocusWnd->isPopupWnd() )//popup
		//{
		//	//���м����sheet ������Ӧ��layer���
		//	d_zPopupIndexOld=d_SheetfocusWnd->setPopupZindex(d_popupzIndexMax);
		//	
		//}
		//else
		//{
		//	//���м����sheet ������Ӧ��layer���
		//	d_zIndexOld=d_SheetfocusWnd->setZindex(d_zIndexMax);
		//}
	}
	//���ͼ�����Ϣ
	fireUIEvent_Focus(d_prevFocus);
	return d_prevFocus;
}


//////////////////////////////////////////////////////////////////////////
/*
msg.message==WM_LBUTTONDOWN ||msg.message==WM_LBUTTONDBLCLK||
msg.message == WM_RBUTTONDOWN ||
msg.message == WM_RBUTTONDBLCLK
*/
//////////////////////////////////////////////////////////////////////////
bool CSoarSheet::fireUIEvent_Focus(const ISoarWnd * d_prevFocus)
{
	//��װ巢��������Ϣ,������ʾ
	if ( d_SheetfocusWnd != d_prevFocus)
	{
		//���ͼ�����Ϣ�������
		if (d_SheetfocusWnd)
		{
			SOARMSG leeMsg;
			leeMsg.message =SOAR_SETFOCUS;
			leeMsg.mouseEvent =0;
			leeMsg.sourceWnd =(ISoarWnd*)d_prevFocus;
			leeMsg.targetWnd =d_SheetfocusWnd;
			leeMsg.routeWnd=NULL;
			leeMsg.wParam =0;
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		//����ʧȥ������Ϣ���Ǽ����
		if (d_prevFocus)
		{
			SOARMSG leeMsg;
			leeMsg.message =SOAR_LOSTFOCUS;
			leeMsg.mouseEvent =0;
			leeMsg.sourceWnd =d_SheetfocusWnd;
			leeMsg.targetWnd =(ISoarWnd*)d_prevFocus;
			leeMsg.routeWnd=NULL;
			leeMsg.wParam =0;
			leeMsg.lParam =0;
			CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
		return true;
	}
	return false;
}
//�������л����Զ�����SOAR_COM���������ڣ�����ROOT�㣩
bool CSoarSheet::fireUIEvent_COMMAND(float t,MSG msg)
{
	//�������ڷ���FYLD_Command��Ϣ��WM_LBUTTONDOWN WM_LBUTTONUP��
    if (msg.message !=WM_MOUSEMOVE &&
		msg.message>= WM_MOUSEFIRST && 
		msg.message <= WM_MOUSELAST )
    {
		//���㴰�ڲ��跢
		if (d_SheetfocusWnd &&d_SheetfocusWnd->GetParent())
		{
			bool bNCMsg=false;
			POINT pt;
			::GetCursorPos(&pt);
			::ScreenToClient(CSoarRoot::getSingletonPtr()->getMsWindow(),&pt);
			RECT rcClient =d_SheetfocusWnd->getClientRect();
			if(!::PtInRect(&rcClient,pt))
			{
				bNCMsg=true;
			}
			SOARMSG leeMsg ;
		    leeMsg.sourceWnd =d_SheetfocusWnd;
		    leeMsg.targetWnd =d_SheetfocusWnd->GetParent();
			leeMsg.routeWnd =d_SheetfocusWnd;
		    leeMsg.message =SOAR_COMMAND ;
		    leeMsg.mouseEvent=CSoarRoot::getSingletonPtr()->translateMouseMessage(msg,bNCMsg);
		    leeMsg.wParam =msg.wParam;
		    leeMsg.lParam = msg.lParam;
			leeMsg.Data=0;
		   CSoarRoot::getSingletonPtr()->addOfflineMsg(leeMsg);
		}
    }
	return false;
}
//Z����
long CSoarSheet::generateZindex()
{
	d_zIndexMax = d_zIndexMax +1;
	return d_zIndexMax;
}
long CSoarSheet::generatePopupZindex()
{
	d_popupzIndexMax = d_popupzIndexMax +1;
	return d_popupzIndexMax;
}
long CSoarSheet::ensureZindex(int nwantedZindex){
	if(d_zIndexMax>nwantedZindex){
		return nwantedZindex;
	}
	return generateZindex();
}
long CSoarSheet::ensurePopupZindex(int nwantedZindex){
	if(d_popupzIndexMax>nwantedZindex){
		return nwantedZindex;
	}
	return generatePopupZindex();
}
void CSoarSheet::setStartZindex(long iStart){
	d_zIndexMax = iStart+1;
	d_baseZindex=iStart;
}
void CSoarSheet::setStartPopupZindex(long iStart){
	d_popupzIndexMax = iStart +1;
	d_basePopupZindex=iStart;
}
long CSoarSheet::getMaxZindex(){
	return d_zIndexMax;
}
long CSoarSheet::getMaxPopupZindex(){
	return d_popupzIndexMax;
}
long CSoarSheet::bringWndToTopLevel(ISoarWnd* pFocusWnd)
{
	//������������N
	ISoarWnd * pParent = FindTopLevelParent(pFocusWnd);
	//�������ǵװ岻����
	if(pParent==d_sheetmainWnd){
		return 0;
	}
	int topmax = d_baseTempZindex;
	if(pParent){
		if (d_SheetfocusWnd->isPopupWnd() )//popup
		{
			//d_zPopupIndexOld=pParent->setPopupZindex(d_popupzIndexMax);
		 d_baseTempZindex=d_popupzIndexMax;
		}
		else{
			//d_zIndexOld=pParent->setZindex(d_zIndexMax);
			 d_baseTempZindex=d_zIndexMax;
		}
		topmax=topmax>d_baseTempZindex?topmax:d_baseTempZindex;
	}
	//������ǰ������N+1�������Ӵ���Ĭ��������Ӧ
	
	//�������Ӵ��ڽ�����Ӵ���
	if (pParent->isPopupWnd() )//popup
	{
		d_zIndexOld=pParent->getPopupZindex();
		if(d_popupzIndexMax<=pParent->getPopupZindex()){
			d_baseTempZindex=pParent->getPopupZindex();
			
			return 0;
		}
	}
	else{
		d_zIndexOld=pParent->getZindex();
		if(topmax<=pParent->getZindex()){
			d_baseTempZindex=pParent->getZindex();
			
			return 0;
		}
	}
		modifyTempZindexWnd(pParent,false);
	return 0;
}
long CSoarSheet::degradeLevelWnd(ISoarWnd* pFocusWnd,long targetlevel){
	//������������N
	ISoarWnd * pParent =FindTopLevelParent(pFocusWnd);
	//�������ǵװ岻����������
	if(pParent==d_sheetmainWnd){
		return 0;
	}
	
	//�����Ĵ��ڽ�����Ӵ���
	if (pParent->isPopupWnd() )//popup
	{
		if(d_zIndexOld>=pParent->getPopupZindex()){
			return 0;
		}
		d_baseTemppopupZindex=targetlevel;
	}
	else{
		if(d_zIndexOld>=pParent->getZindex()){
			return 0;
		}
		d_baseTempZindex=targetlevel;
	}
	
	modifyTempZindexWnd(pParent,true);
		
	return 0;
}
long CSoarSheet::generateTempZindex()
{
	d_baseTempZindex = d_baseTempZindex +1;
	return d_baseTempZindex;
}
ISoarWnd* CSoarSheet::FindTopLevelParent(ISoarWnd* pChildWnd){
	if(pChildWnd->getWndStyle() & LWNDSTY_POPUP){
			return pChildWnd;
		}
	if(pChildWnd==d_sheetmainWnd ){
		return d_sheetmainWnd;
		}
	ISoarWnd * pParent =pChildWnd->GetParent();
	//�������ǵװ岻����������
	while(pParent){
		if(pParent==d_sheetmainWnd ){
		return d_sheetmainWnd;
		}
		if(pParent->getWndStyle() & LWNDSTY_POPUP){
			return pParent;
		}
		//����
	 pParent =pParent->GetParent();
	}
	if(pParent ==NULL){
		return d_sheetmainWnd;
	}
}
/*
����ֻ��popup�������Ӵ��ڶ���popup�������������popup��������ͨ�ģ������ܵģ�
*/
void CSoarSheet::modifyTempZindexWnd(ISoarWnd* newWnd,bool isDownlevel){
	int tIndex = 0;
	tIndex = d_baseTempZindex;
	LayerMap::iterator it;
	if (newWnd->isPopupWnd() )//popup
	{
		
		//������ͼ��ɾ��
		it=d_popuplayers.begin();
			while(it != d_popuplayers.end() )
			{
				if(it->second->seekWnd(newWnd) )
				{	
					it->second->removeWnd(newWnd);
					break;
				}
				++it;
			}
		//����Zindex
			newWnd->setPopupZindex(tIndex);
			it =d_popuplayers.find(tIndex);
			if(it != d_popuplayers.end())
			{
				ISoarLayer* layer = it->second;
				layer->addWnd(newWnd);
			}
			else//�����Ƿ��Ѿ�������ͼ��
			{
			
				ISoarLayer* layer = new CSoarLayer(this);
				layer->addWnd(newWnd);
				d_popuplayers[tIndex]=layer;
			}
	}
	else
	{
		//������ͼ��ɾ��
		it=d_layers.begin();
		while(it != d_layers.end() )
		{
			if(it->second->seekWnd(newWnd) )
			{	
				it->second->removeWnd(newWnd);
				break;
			}
			++it;
		}
		//����Zindex
		newWnd->setZindex(tIndex);
		it =d_layers.find(tIndex);
		if(it != d_layers.end())
		{
			ISoarLayer* layer = it->second;
			layer->addWnd(newWnd);
		}
		else
		{
			
			ISoarLayer* layer = new CSoarLayer(this);
			layer->addWnd(newWnd);
			d_layers[tIndex]=layer;
		}

	}
	//�������Ӵ��ڽ�����Ӵ���
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			modifyTempZindexWndRecursive(itChild->second,isDownlevel);
			++itChild;
		}
}
/*
���parent����ͨ���ڵĻ�����Ҫ����popup��˳��
�����popup�Ļ���Ҫȫ������
*/
void CSoarSheet::modifyTempZindexWndRecursive(ISoarWnd* newWnd,bool isDownlevel){
	int tIndex = 0;
	ISoarWnd* thisParent =newWnd->GetParent();
	//step 1:find if it 's paren in current sheet ,found that use it's parent index +1
	if ( thisParent->isPopupWnd() )//popup
	{
		if(findWndfast(thisParent) && newWnd->isPopupWnd())
		{//ͬ����
			//if down leve
			if(isDownlevel)
			{
				tIndex = thisParent->getPopupZindex()+1;
			}
			else
			{
				tIndex = thisParent->getPopupZindex()+1;
				if(d_baseTempZindex<tIndex)
				{
					//�����µĲ�
					d_baseTempZindex++;
					tIndex =d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
			
		}
		else if(newWnd->isPopupWnd())
		{
			if(isDownlevel)
			{
				tIndex = d_baseTempZindex;
			}
			else{
				tIndex = d_baseTempZindex+1;
				if(d_baseTempZindex<tIndex)
				{
					//�����µĲ�
					d_baseTempZindex++;
					tIndex =d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
		}
		else
		{//must be some error 
			;
		}
		
	}
	else
	{
		if(!newWnd->isPopupWnd())
		{ //ֻ����ǵ�������
			if(findWndfast(thisParent))
			{///�ҵ�������
				if(isDownlevel)
				{
					tIndex = thisParent->getZindex()+1;
				}
				else
				{
					tIndex = thisParent->getZindex()+1;
					if(d_baseTempZindex<tIndex)
					{
						//�����µĲ�
						d_baseTempZindex++;
						tIndex =d_baseTempZindex;
					}
				}
			}
			else{
				if(isDownlevel)
				{
				tIndex = d_baseTempZindex;
				}
				else
				{
					d_baseTempZindex++;
					tIndex = d_baseTempZindex;
				}
			}
			removeWnd(newWnd);
			addWndToZlayer(tIndex,newWnd);
		}
		else 
		{
			;
		}
		
	}
	
	//�������Ӵ��ڽ�����Ӵ���
		LeeWndPtrMap &childmap=newWnd->getChidWndSet();
		LeeWndPtrMap::iterator itChild=childmap.begin();
		while(itChild != childmap.end()){
			modifyTempZindexWndRecursive(itChild->second,isDownlevel);
			++itChild;
		}
}
