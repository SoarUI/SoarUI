#pragma once
#include "Soarwnd.h"
/*������Ϊ�ӿؼ�Ƕ��ؼ��������ȡ����*/
class CSoarNonClientSegmentBase 
{
public:
	typedef	std::map<int, SoarSegmentItem>	LeeItemWndMap;
	CSoarNonClientSegmentBase(ISoarWnd *parent,ISoarEngine* Eng);
	virtual ~CSoarNonClientSegmentBase(void);
	virtual RECT getClientRect(void) ;//��ȡ����ڸ����ڵľ���
	virtual RECT getWindowRect(void) ;
	virtual RECT getCliperRect(void);
protected:
	bool d_lbuttondown;
	ISoarWnd* d_OwnerWnd;
	CLeeString d_wndlook;//��ӦƤ���ؼ�
	CLeeString d_wndlookState;//Ƥ��״̬
	ComponentArea d_area;//����ͳһ������Ϣ
	ISoarEngine *d_DrawEng;//��ͼ���
	SEGMENTITEMS d_items;
	LWNDT d_wndtype;//��������
};
