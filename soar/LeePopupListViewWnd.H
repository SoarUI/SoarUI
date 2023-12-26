#pragma once
#include "soarwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "LeeHeaderWnd.h"
#include "../SoarHeader/SoarExport.h"
#include "leePopupBase.h"
/*
一个list控件由控件头、控件容器、item容器组成2016.9.6 23:57于中山市爱纳电器有限公司
*/
class CLeePopupListViewWnd :
	public CLeePopupBase
{
public:
	CLeePopupListViewWnd(HWND root,ISoarEngine* Eng);
	~CLeePopupListViewWnd(void);
	typedef	std::map<int, ISoarItemWnd*>	LeeItemWndMap;
	typedef std::map<int,LeeItemWndMap> LeeITEMMAP;
	typedef	std::map<ISoarItemWnd*, ISoarItemWnd*> LeeGCItemWndMap;


	virtual void InitializeSegments(void);
	int addHeaderItem(int iColWidth,const CLeeString& str);
	int setHeaderHeight(int nHeight) ;
	int setHeaderWidth(int nWidth) ;
	//增加行
	int addRow(const CLeeString& str,int nID,LPVOID data);
	void clear(void);
	int removeCol(int iRow,int nCol);
	//移除整行
	int removeRow(int iRow);
	//增加行
	int insertCol(int iRow,int nCol,const CLeeString& str,int nID,LPVOID data);
	void DrawSelf(ILeeDrawInterface *DrawFuns);
	LPCTSTR getItemString(int nRow,int nCol);
	LPVOID getItemData(int nRow,int nCol);
	DWORD getItemID(int nRow,int nCol);
	virtual void setItemString(int iIndex,int subItem,const CLeeString &str );
	virtual void setItemData(int iIndex,int subItem,LPVOID data) ;
	virtual void setItemID(int iIndex,int subItem,INT nID) ;
	virtual int getSelectedItemIndex(void);
	virtual void setCurSel(int islot);
	virtual int getItemCount(void);
    virtual void setState(LWNDST state) ;
	virtual BOOL HandleEvent( UINT ,WPARAM ,LPARAM, LRESULT& lr) ;
	virtual RECT getClientRect(void) ;
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_OwnerWnd = pOwnerWnd;}
	virtual ISoarWnd* getOwnerWnd(void){ return d_OwnerWnd;}
private:
	void sortRow(int iStart,int itargetStart);
	void setItemPosition(ISoarItemWnd* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H);
	void EnsureColVisible(int iStartRow,int iEndRow,int iStartCol,int iEndCol);
	bool getVsibleRange(int & nRow,int& nCol);
protected:
	LeeITEMMAP d_commItems;//在用的项
	LeeGCItemWndMap d_gcitems;//垃圾回收器
	CLeeHeaderWnd * d_headerCtrl;
	int d_cntRow;//行总数
	int d_nItemHeight;//行高
	int d_nItemWidth;//项宽

	int d_curStartRowIndex;//项开始显示索引
	int d_curStartColIndex;//列开始显示索引
	int d_selectedRowIndex;//选择的项的索引
	ISoarWnd* d_OwnerWnd;//消息处理窗口
};
