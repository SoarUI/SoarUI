#pragma once
#include "soarExport.h"
#include "ISoarItemWnd.h"

class ISoarTabWnd :
	public ISoarWnd
{
public:
	virtual bool addTab(DWORD islot,const CLeeString &szTab,LPVOID data)=0;
	virtual bool linkTab(DWORD islot,ISoarWnd* dataWnd) =0;
	virtual int setTabHeight(int h)=0;
	virtual int setTabWidth(int w) =0;
	virtual int setButtonsInoneLine(int n) =0;
	virtual int getCurSel(void) =0;
	virtual void setCurSel(int islot) =0;
	
};
class ISoarListViewWnd :
	public ISoarWnd
{
public:
	virtual int setHeaderHeight(int nHeight) =0;
	virtual int setHeaderWidth(int nWidth) =0;
	virtual int addHeaderItem(int iColWidth, const CLeeString& str)=0;
	virtual int addRow(const CLeeString& str,int nID,LPVOID data) =0;
	virtual void clear(void) =0;
	virtual int removeCol(int iRow,int nCol) =0;
	//移除整行
	virtual int removeRow(int iRow) =0;
	virtual int insertCol(int iRow,int nCol, const CLeeString& str,int nID,LPVOID data)=0;
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
	virtual LPCTSTR getItemString(int nRow,int nCol)=0;
	virtual LPVOID getItemData(int nRow,int nCol)=0;
	virtual DWORD getItemID(int nRow,int nCol)=0;
	virtual void setItemString(int iIndex,int subItem, const CLeeString&str ) =0;
	virtual void setItemData(int iIndex,int subItem,LPVOID data) =0;
	virtual void setItemID(int iIndex,int subItem,INT nID) =0;
    virtual int getSelectedItemIndex(void)=0;
	virtual void setCurSel(int islot) =0;
	virtual int getItemCount(void) =0 ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd) =0;
	virtual ISoarWnd* getOwnerWnd(void) =0;
};
class ISoarComboWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& )=0 ;
    virtual int addItem(const CLeeString & str,int nID,LPVOID data) =0;
	virtual LPCTSTR getText(void)=0;
	virtual void setText(const CLeeString &name)=0;
	virtual DWORD getItemID(int iIndex)=0;
	virtual LPCTSTR getItemString(int iIndex)=0;
	virtual LPVOID getItemData(int iIndex)=0;
	virtual int getSelectedItemIndex(void)=0;
	virtual bool getButtonState(void) =0;
	virtual void setCurSel(int islot) =0;
	virtual int getItemCount(void) =0 ;

};
class ISoarListbox :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
	virtual int getSelectedItemIndex(void)=0;
	virtual int addItem(const CLeeString & str,int nID,LPVOID data) =0;
	virtual bool delItem(int iIndex) =0;
	virtual DWORD getItemIndex(int iIndex) =0;
	virtual DWORD getItemID(int iIndex) =0;
	virtual LPCTSTR getItemString(int iIndex) =0;
	virtual LPVOID getItemData(int iIndex) =0;
	virtual void setItemString(int iIndex, const CLeeString&str ) =0;
	virtual void setItemData(int iIndex,LPVOID data) =0;
	virtual void setItemID(int iIndex,INT nID) =0;
	virtual int getItemHeight(void) =0;
	virtual int setItemHeight(int nHeight) =0;
	virtual void setCurSel(int islot) =0;
	//移除整行
	virtual int removeItem(int iRow) =0;
	virtual int getItemCount(void) =0 ;
};
class ISoarButtonWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&)=0 ;
};
class ISoarCheckButtonWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) =0 ;
	virtual bool getCheck(void) =0 ;
	virtual void setCheck(bool bChecked) =0 ;
};
class ISoarGroupBox :
	public ISoarWnd
{
public:
	virtual bool AddControl(int topleft,int topY,ISoarWnd* pControl) =0;
};
class ISoarEditWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) =0;
	virtual void isPassword(bool bpwd) =0;
};
class ISoarFrameWnd :
	public ISoarWnd
{
public:
	virtual void setText(const CLeeString & name)=0;
};
class ISoarHeaderWnd :
	public ISoarWnd
{
public:
	//头的项数目
	virtual int getItemCount(void) =0;
	virtual int addItem(int nItemWidth, const CLeeString& str) =0;
	virtual void setItemString(const CLeeString& str,int islot) =0;
	virtual LPCTSTR getItemString(int iSlot) =0;
	virtual int getHeight(void) =0;
	virtual int setHeight(int nHeight) =0;
	virtual int getItemWidth(int iSlot) =0;
	virtual ISoarItemWnd* getItem(int icol) =0;
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
};

class ISoarInternetWnd :
	public ISoarWnd
{
public:
	//CLeeString getAddress(void);
	virtual DWORD getAddress(void) =0;
	virtual void setAddress(const CLeeString& addr) =0;
	virtual void setAddress(DWORD addr)=0;
	virtual void setAddress(int prt0,int prt1,int prt2,int prt3) =0;

};
class ISoarRadioWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT&) =0;
	virtual bool getCheck(void) =0;
	virtual void setCheck(bool bChecked) =0;
	virtual void setRadioGroup(const CLeeString& group)=0;
};
class ISoarProgress:
	public ISoarWnd
{
public:
	virtual float getPos(void) =0 ;
	virtual void setPos(float pos) =0 ;
	virtual DWORD setRange(int nMin,int nMax) =0 ;
	virtual bool  getRange(int &nMin,int &nMax) =0 ;
	virtual float  setStep(float stp) =0 ;
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns) =0;
};
class ISoarSpinWnd:
	public ISoarWnd
{
public:
	virtual DWORD setRange(int nMin,int nMax) =0 ;
	virtual bool  getRange(int &nMin,int &nMax) =0 ;
	virtual void setlinkWnd(ISoarWnd* pTargetWnd)=0;
};
class ISoarSliderWnd:
	public ISoarWnd
{
public:
	virtual float getPos(void)=0;
	virtual void setPos(float pos)=0;
	virtual DWORD setRange(int nMin,int nMax)=0;
	virtual bool  getRange(int &nMin,int &nMax)=0;
};
class ISoarStaticWnd :
	public ISoarWnd
{
public:
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns)=0;
};
//class ISoarScrollWnd :
//	public CSoarNonClientCtrlBase
//{
//public:
//	//ISoarScrollWnd(HWND root,ISoarEngine* Eng):CSoarNonClientCtrlBase(root,Eng){}
//	//virtual ~ISoarScrollWnd(void){}
//};
class CSoarFrameWnd :
	public ISoarFrameWnd
{
public:
	virtual void setText(const CLeeString & name);
public:
	virtual void InitializeSegments(void);
	virtual BOOL HandleEvent(UINT uMsg ,WPARAM wParam ,LPARAM lParam, LRESULT& lr);
protected:
	ISoarWnd *p_minbutton;
	ISoarWnd *p_maxbutton;
	ISoarWnd * p_closebutton;
	ISoarWnd* p_title;
};
//////////////////////////////////////////////////////////////////////////
class ISoarFileDialogWnd :
	public ISoarWnd
{
public:
	virtual CLeeString& getSelectString(void) =0;
};


	 