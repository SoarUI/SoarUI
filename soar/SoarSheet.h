#pragma once
/*SoarϵͳSheet��ʵ�ֳַ�����ͬ��Cocox2d Scene
������������ͨ���������ͼ�㣬û�е����ֲ�
��Soarϵͳ��ģ̬�Ի���֮����һ��Sheet����ģ̬����ָ������ͨ���ڣ�������������ʾ����Ȩ
*/
#include"ISoarSheet.h"
#include "SoarBindSheetWnd.h"
#include"ISoarLayer.h"
class CSoarSheet:
	public ISoarSheet
{
public:
	CSoarSheet(HWND root,bool bAutoDestroy =false);
	virtual ~CSoarSheet(void);
	typedef	std::map<INT, ISoarLayer*>	LayerMap;
	virtual ISoarWnd* getPrevFocusWnd(void);
	virtual ISoarWnd* setPrevFocusWnd(ISoarWnd* pFocus);
	virtual ISoarWnd* getFocusWnd(void);
	virtual ISoarWnd* setFocusWnd(ISoarWnd* pFocus);

	virtual ISoarWnd* setMainWnd(ISoarWnd* maidW);
	virtual ISoarWnd* getMainWnd(void);
	virtual void setModalState(bool bModal);
	///////////////////////////
	virtual void addWndRecursive(ISoarWnd* newWnd) ;
	virtual BOOL seekWnd(ISoarWnd* newWnd) ;
	virtual bool seekWnd(const CLeeString & name);
	virtual ISoarWnd* getWnd(const CLeeString & name);
	virtual void removeWndRecursive(ISoarWnd* newWnd);
	virtual void eraseWndRecursive(ISoarWnd* pWnd);
	virtual void clearAllLinkWnd() ;
	//���ڲ�θı�
	virtual BOOL WndZorderChanged(ISoarWnd* FocusWnd,int newFocusZpos,int oldZindex);
	//������ʽ�ı�
	virtual BOOL WndStyleChanged(ISoarWnd* newWnd);
	virtual BOOL SoarUIMsgProc ( UINT ,WPARAM ,LPARAM,LRESULT& );
	virtual bool filter_ClickMsg(UINT uMessage,WPARAM wParam,LPARAM lParam,LRESULT&);
	virtual bool RenderLoop(ILeeDrawInterface *DrawFuns,float &t);
	//Graphic operation
	virtual bool enableAlphaBlend(bool bEnable);
	//Z����
	virtual long generateZindex();
	virtual long generatePopupZindex();
	virtual void setStartZindex(long iStart);
	virtual void setStartPopupZindex(long iStart);
	virtual long getMaxZindex();
	virtual long getMaxPopupZindex();
	virtual long bringWndToTopLevel(ISoarWnd* pFocusWnd);
	virtual long degradeLevelWnd(ISoarWnd* pFocusWnd,long targetlevel);
	//
	
private:
	virtual bool handlerClickMsg(MSG msg);
	//�����¼�
    virtual bool fireUIEvent_COMMAND(float t,MSG msg);
	virtual bool fireUIEvent_Focus(const ISoarWnd * d_prevFocus);
	virtual bool filter_MouseMoveUI(UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual bool filter_PaintMessage(UINT uMessage,WPARAM wParam,LPARAM lParam);
	virtual bool filter_allMessage(UINT uMsg,WPARAM wParam,LPARAM lParam,LRESULT&);
	//Z 
	virtual long generateTempZindex();
	virtual void modifyTempZindexWnd(ISoarWnd* newWnd,bool isDownlevel) ;
	virtual void modifyTempZindexWndRecursive(ISoarWnd* newWnd,bool isDownlevel);
	virtual  ISoarWnd* FindTopLevelParent(ISoarWnd* newWnd);
	virtual void addWndToZlayer(int zpos,ISoarWnd* pWnd);
	virtual long ensureZindex(int nwntedZindex);
	virtual long ensurePopupZindex(int nwntedZindex);
	bool findWndfast(ISoarWnd* pWnd);
	virtual void addWnd(ISoarWnd* newWnd) ;
	virtual void removeWnd(ISoarWnd* newWnd);
	virtual void eraseWnd(ISoarWnd* pWnd);
protected:
	LayerMap d_layers;
	LayerMap d_popuplayers;//��������Ȩ����Ĵ���;infodlg modeldialog non-modeldialog systemDialog 
	 //-Sheet���� ��ϵͳ���𵯳�������
	HWND d_rootWnd;//ԭʼ����
	ISoarWnd* d_sheetmainWnd;//��Sheet�ĵװ�
	ISoarWnd* d_PrevSheetfocusWnd;//�����sheet֮ǰ�ļ����
	ISoarWnd* d_SheetfocusWnd;//���浱ǰsheet�����
	long d_popupzIndexMax;//��¼Sheet���������ڵ����ZIndex
	long d_zIndexMax;//��¼��ͨ���ڵ����Zindex
	long d_zIndexOld;//��¼��ǰSheet�Ĵ��ڼ���ǰ��Zλ��
	long d_zPopupIndexOld;//��¼����ǰ��Zλ��
	long d_baseZindex;//��Sheet�Ļ���Zֵ(��ʵ��ȫ�ֵ��Ѳ���)
	long d_basePopupZindex;//��Sheet�Ļ���Zֵ(��ʵ��ȫ�ֵ��Ѳ���)
	long d_baseTempZindex;//��ʱindex
	long d_baseTemppopupZindex;//��ʱindex
	//ȫ��Z= d_baseZindex+d_zIndexMax;
	bool d_bAutoDestroy;//�Ƿ��Զ�����
	bool d_bModalState;//�ò�Ϊmodal�㣬��ζ�Ų�������Ϣ���ᴫ���ϲ�
	bool d_bEnableAlphaBlend;//�������
};

