#pragma once
#include "Soarwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "LeeHeaderWnd.h"
#include "../SoarHeader/SoarExport.h"
/*
一个list控件由控件头、控件容器、item容器组成2016.9.6 23:57于中山市爱纳电器有限公司
*/
class CLeeListViewWnd :
	public CSoarWnd
{
public:
	CLeeListViewWnd(HWND root,ISoarEngine* Eng);
	~CLeeListViewWnd(void);
	typedef	std::map<int, ISoarItemBase*>	ItemSegmentMap;
	//typedef std::map<int,LeeItemWndMap> LeeITEMMAP;
	typedef	std::map<ISoarItemBase*, ISoarItemBase*> LeeGCItemWndMap;

	virtual void InitializeSegments(void);
	virtual int setHeaderWidth(int nWidth) ;
	virtual int setHeaderHeight(int nHeight) ;
	virtual int addHeaderItem(int iColWidth,CLeeString str);
	
	
	
	
	//增加行
	virtual int addRow(CLeeString str,int nID,LPVOID data);
	virtual void clear(void);
	virtual int removeCol(int iRow,int nCol);
	//移除整行
	virtual int removeRow(int iRow);
	//增加行
	virtual int insertCol(int iItem,int subItem,CLeeString str,int nID,LPVOID data);
	virtual void DrawSelf(ILeeDrawInterface *DrawFuns);
	virtual CLeeString getItemString(int nRow,int nCol);
	virtual LPVOID getItemData(int nRow,int nCol);
	virtual DWORD getItemID(int nRow,int nCol);
	virtual void setItemString(int iIndex,int subItem,CLeeString &str );
	virtual void setItemData(int iIndex,int subItem,LPVOID data) ;
	virtual void setItemID(int iIndex,int subItem,INT nID) ;
    virtual void setState(LWNDST state) ;
	virtual LRESULT HandleEvent( UINT ,WPARAM ,LPARAM ) ;
	virtual RECT getClientRect(void) ;
	virtual int getSelectedItemIndex(void);
	virtual void setCurSel(int islot);
	virtual int getItemCount(void);
	virtual void setOwnerWnd(ISoarWnd* pOwnerWnd){ d_OwnerWnd = pOwnerWnd;}
	virtual ISoarWnd* getOwnerWnd(void){ return d_OwnerWnd;}
	
private:
	void sortRow(int iStart,int itargetStart);
	void setItemPosition(ISoarItemBase* lpWnd,Dimension& X,Dimension  &Y,Dimension& W,Dimension& H);
	void EnsureColVisible(int iStartRow,int iEndRow,int iStartCol,int iEndCol);
	bool getVsibleRange(int & nRow,int& nCol);
	int CalcColumnStartX(int nCol);
protected:
	ItemSegmentMap d_commItems;//在用的项
	LeeGCItemWndMap d_gcitems;//垃圾回收器
	CLeeHeaderWnd * d_headerCtrl;
	int d_cntRow;//行总数
	int d_nItemHeight;//行高
	int d_nItemWidth;//项宽
	ISoarWnd * d_OwnerWnd;//处理窗口
	int d_curStartRowIndex;//项开始显示索引
	int d_curStartColIndex;//列开始显示索引
	int d_selectedRowIndex;//选择的项的索引
	int d_selectedColIndex;//选择的项列的索引
};
