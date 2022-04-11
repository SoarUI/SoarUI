#pragma once
#include "Soarwnd.h"
#include "../SoarHeader/ISoarItemWnd.h"
#include "LeeHeaderWnd.h"
#include "../SoarHeader/SoarExport.h"
/*
һ��list�ؼ��ɿؼ�ͷ���ؼ�������item�������2016.9.6 23:57����ɽ�а��ɵ������޹�˾
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
	
	
	
	
	//������
	virtual int addRow(CLeeString str,int nID,LPVOID data);
	virtual void clear(void);
	virtual int removeCol(int iRow,int nCol);
	//�Ƴ�����
	virtual int removeRow(int iRow);
	//������
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
	ItemSegmentMap d_commItems;//���õ���
	LeeGCItemWndMap d_gcitems;//����������
	CLeeHeaderWnd * d_headerCtrl;
	int d_cntRow;//������
	int d_nItemHeight;//�и�
	int d_nItemWidth;//���
	ISoarWnd * d_OwnerWnd;//������
	int d_curStartRowIndex;//�ʼ��ʾ����
	int d_curStartColIndex;//�п�ʼ��ʾ����
	int d_selectedRowIndex;//ѡ����������
	int d_selectedColIndex;//ѡ������е�����
};
