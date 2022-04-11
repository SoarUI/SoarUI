#include "SoarInternetWnd.h"
#include "SoarEditWnd.h"
#include "SoarRoot.h"
#include <commctrl.h>
#define LEE_FIRST_IPADDRESS(x)  (((x) >> 24) & 0xff)
#define LEE_SECOND_IPADDRESS(x) (((x) >> 16) & 0xff)
#define LEE_THIRD_IPADDRESS(x)  (((x) >> 8) & 0xff)
#define LEE_FOURTH_IPADDRESS(x) ((x) & 0xff)
CLeeInternetWnd::CLeeInternetWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng)
{
	d_wndlook="DefaultClient";
	d_wndtype =LWNDT_INTERETWND;
	
}

CLeeInternetWnd::~CLeeInternetWnd(void)
{
}
//CLeeString CLeeInternetWnd::getAddress(void)
//{
//	return "54654";
//}
void CLeeInternetWnd::InitializeSegments(void)
{
	RECT rt ={0,0,0,0};
	//ÁÐ±ílist
	AbsoluteDim listx(0);
	Dimension listX(listx,DT_X_POSITION);
	Dimension listY( listx,DT_TOP_EDGE);

	//
	UDim dminH(1.0,10);
	UnifiedDim listh(dminH,DT_HEIGHT);
	Dimension listH(listh,DT_HEIGHT);
	//
	UDim dminW(0.25,0);
	UnifiedDim listw(dminW,DT_WIDTH);
	Dimension listW(listw,DT_WIDTH);
	CLeeString itemname ;
	itemname.Format("FLYDREAM__%p__%s",this,"systemAddrEd0");
	if (d_isPopupWnd)
	{
		d_firstpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX,listY,
			listW,listH,LWNDT_EDIT,this,d_zPopupIndex+1);
	} 
	else
	{
		d_firstpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,listX,listY,
			listW,listH,LWNDT_EDIT,this,d_zIndex+1);
	}
	
	UDim dminX1(0.25,0);
	UnifiedDim listx1(dminX1,DT_WIDTH);
	Dimension listX1(listx1,DT_X_POSITION);
	itemname.Format("FLYDREAM__%p__%s",this,"systemAddrEd1");
	if (d_isPopupWnd)
	{
		d_secondpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX1,listY,
			listW,listH,LWNDT_EDIT,this,d_zPopupIndex+1);
	} 
	else
	{
		d_secondpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,listX1,listY,
			listW,listH,LWNDT_EDIT,this,d_zIndex);
	}
	
	UDim dminX2(0.5,0);
	UnifiedDim listx2(dminX2,DT_WIDTH);
	Dimension listX2(listx2,DT_X_POSITION);
	itemname.Format("FLYDREAM__%p__%s",this,"systemAddrEd2");
	if (d_isPopupWnd)
	{
		d_thirdpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX2,listY,
			listW,listH,LWNDT_EDIT,this,d_zPopupIndex+1);
	} 
	else
	{
		d_thirdpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,listX2,listY,
			listW,listH,LWNDT_EDIT,this,d_zIndex+1);
	}
	
	UDim dminX3(0.75,0);
	UnifiedDim listx3(dminX3,DT_WIDTH);
	Dimension listX3(listx3,DT_X_POSITION);
	itemname.Format("FLYDREAM__%p__%s",this,"systemAddrEd3");
	if (d_isPopupWnd)
	{
		d_fouthpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,listX3,listY,
			listW,listH,LWNDT_EDIT,this,d_zPopupIndex);
	} 
	else
	{
		d_fouthpart=(CLeeEditWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,listX3,listY,
			listW,listH,LWNDT_EDIT,this,d_zIndex);
	}
	
	setAddress(192,168,1,1);
}
DWORD CLeeInternetWnd::getAddress(void)
{
	CLeeString iby =d_firstpart->getText();
	BYTE prt0=atoi(iby.c_str());
	iby =d_secondpart->getText();
	BYTE prt1=atoi(iby.c_str());
	iby =d_thirdpart->getText();
	BYTE prt2=atoi(iby.c_str());
	iby =d_fouthpart->getText();
	BYTE prt3=atoi(iby.c_str());

	return MAKEIPADDRESS(prt0,prt1,prt2,prt3);
}
void CLeeInternetWnd::setAddress(CLeeString addr)
{
	;
}
void CLeeInternetWnd::setAddress(DWORD dwAddr)
{
	BYTE IPPart1 = LEE_FIRST_IPADDRESS(dwAddr);
	BYTE IPPart2 = LEE_SECOND_IPADDRESS(dwAddr);
	BYTE IPPart3 = LEE_THIRD_IPADDRESS(dwAddr);
	BYTE IPPart4 = LEE_FOURTH_IPADDRESS(dwAddr);
	CLeeString str;
	str.Format("%d",IPPart1);
	d_firstpart->setText(str);
	str.Format("%d",IPPart2);
	d_secondpart->setText(str);
	str.Format("%d",IPPart3);
	d_thirdpart->setText(str);
	str.Format("%d",IPPart4);
	d_fouthpart->setText(str);
}
void CLeeInternetWnd::setAddress(int prt0,int prt1,int prt2,int prt3)
{
	CLeeString str;
	str.Format("%d",prt0);
	d_firstpart->setText(str);
	str.Format("%d",prt1);
	d_secondpart->setText(str);
	str.Format("%d",prt2);
	d_thirdpart->setText(str);
	str.Format("%d",prt3);
	d_fouthpart->setText(str);
}