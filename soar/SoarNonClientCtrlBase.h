#pragma once
#include "Soarwnd.h"
/*������Ϊ�ӿؼ�Ƕ��ؼ��������ȡ����*/
class CLeeNonClientCtrlBase :
	public CSoarWnd
{
public:
	CLeeNonClientCtrlBase(HWND root,ISoarEngine* Eng);
	virtual ~CLeeNonClientCtrlBase(void);
	virtual RECT getClientRect(void) ;//��ȡ����ڸ����ڵľ���
	virtual RECT getWindowRect(void) ;
	virtual RECT getCliperRect(void);
};
