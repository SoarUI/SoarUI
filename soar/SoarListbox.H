#pragma once
#include "Soarwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "../SoarHeader/SoarExport.h"

class CLeeListbox :
	public CSoarWnd
{
public:
	CLeeListbox(HWND root,ISoarEngine* Eng);
	~CLeeListbox(void);
	typedef	std::map<int, ISoarItemWnd*>	LeeItemWndMap;
	typedef	std::map<ISoarItemWnd*, ISoarItemWnd*> LeeGCItemWndMap;
	/*
	DWORD d_ID ;
	CLeeString d_string;
	LPVOID d_data;
	*/
	virtual LRESULT HandleEvent ( UINT ,WPARAM ,LPARAM );
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual int getSelectedItemIndex(void);
	virtual void setCurSel(int islot);
	virtual int addItem(const CLeeString & str,int nID,LPVOID data);
	virtual bool delItem(int iIndex);
	//移除整行
	virtual int removeItem(int iRow);
	virtual DWORD getItemIndex(int iIndex) ;
	virtual DWORD getItemID(int iIndex) ;
	virtual CLeeString getItemString(int iIndex) ;
	virtual LPVOID getItemData(int iIndex) ;
	virtual void setItemString(int iIndex,CLeeString &str );
	virtual void setItemData(int iIndex,LPVOID data) ;
	virtual void setItemID(int iIndex,INT nID) ;
	int getItemHeight(void) ;
	int setItemHeight(int nHeight) ;
	virtual int getItemCount(void);
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
