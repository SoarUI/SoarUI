#pragma once
#include "LeePopupBase.h"
#include "../SoarHeader/ISoarItemWnd.h"

class CLeePopupListbox :
	public  CLeePopupBase
{
public:
	CLeePopupListbox(HWND root,ISoarEngine* Eng);
	~CLeePopupListbox(void);
	typedef	std::map<int, ISoarItemWnd*>	LeeItemWndMap;
	typedef	std::map<ISoarItemWnd*, ISoarItemWnd*> LeeGCItemWndMap;
	/*
	DWORD d_ID ;
	CLeeString d_string;
	LPVOID d_data;
	*/
	virtual void setState(LWNDST state) ;
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual int getSelectedItemIndex(void);
	virtual void setCurSel(int islot);
	virtual bool addItem(const CLeeString & str,int nID,LPVOID data);
	virtual bool delItem(int iIndex);
	//移除整行
	virtual int removeItem(int iRow);
	virtual DWORD getItemIndex(int iIndex) ;
	virtual DWORD getItemID(int iIndex) ;
	virtual void setItemString(int iIndex,CLeeString &str );
	virtual void setItemData(int iIndex,LPVOID data) ;
	virtual void setItemID(int iIndex,INT nID) ;
	virtual LPCTSTR getItemString(int iIndex) ;
	virtual LPVOID getItemData(int iIndex) ;
	virtual int getHeight(int iIndex) ;
	virtual int getWidght(int iIndex) ;
	virtual int getItemCount(void);
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_OwnerWnd = pOwnerWnd;}
	virtual ISoarWnd* getOwnerWnd(void){ return d_OwnerWnd;}
private:
	void sortRow(int iStart,int itargetStart);
	void setItemPosition(ISoarItemWnd* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H);
	void EnsureColVisible(int iStartRow,int iEndRow);
	bool getVsibleRange(int & nRow);
protected:
	LeeItemWndMap d_commItems;
	LeeGCItemWndMap d_gcitems;//垃圾回收器
	int d_iIndex;
	int d_nItemHeight;//行高
	DWORD d_selectedIndex;
	int d_curStartRowIndex;//项开始显示索引
	ISoarWnd* d_OwnerWnd;//消息处理窗口
};
