#include "StdAfx.h"
#include "FrameSampleUsing.h"
#include "../SoarHeader/SoarExport.h"
extern ISoarProgress* d_dymanicProgress; 
extern ISoarProgress* d_dymanicProgressV; 
extern ISoarSpinWnd* d_spinWnd;
extern ISoarSliderWnd* d_sliderWnd;
CFrameSampleUsing::CFrameSampleUsing(ISoarRoot* Root,ISoarWnd* mainWnd):d_frameWnd(mainWnd)
{
	ISoarRoot* root =Root;
	d_userStatic=root->createWnd("erttqw",30,20,60,30,LWNDT::LWNDT_STATIC,d_frameWnd,102,LWNDSTY_CHILD);
	d_userStatic->setText("������");
	
	d_userEdit=root->createWnd("erttqwq2e",100,30,250,30,LWNDT::LWNDT_EDIT,d_frameWnd,112,LWNDSTY_CHILD);
	d_passEdit=root->createWnd("erttqwqe",100,60,250,30,LWNDT::LWNDT_EDIT,d_frameWnd,102,LWNDSTY_CHILD);
	((ISoarEditWnd*)d_passEdit)->isPassword(true);
	d_passStatic=root->createWnd("erttqwmi",30,60,60,30,LWNDT::LWNDT_STATIC,d_frameWnd,102,LWNDSTY_CHILD);
	
	d_passStatic->setText("���룺");
	d_confirmButton=root->createWnd("butt",30,110,100,30,LWNDT::LWNDT_BUTTON,d_frameWnd,110,LWNDSTY_CHILD);
	d_confirmButton->setText("ȷ  ��");
	ISoarWnd *p2=root->createWnd("radio",140,110,100,30,LWNDT::LWNDT_RADIOBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p2->setText("��ѡ��0");
	ISoarWnd *p3=root->createWnd("radio1",140,140,100,30,LWNDT::LWNDT_RADIOBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p3->setText("��ѡ��1");
	ISoarWnd *p4=root->createWnd("checkbtn",30,140,100,30,LWNDT::LWNDT_CHECKBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p4->setText("��ѡ��");
	d_confirmButton->RegisterEventHandler(&hand);
	d_dymanicProgress =(ISoarProgress*)root->createWnd("Progressbar",280,150,400,25,LWNDT::LWNDT_PROGRESSBAR,d_frameWnd,110,LWNDSTY_HORZWND);
	d_dymanicProgress->setPos(53.0);
	d_dymanicProgress->setRange(0,100);
	//spin
	d_spinWnd=(ISoarSpinWnd*)root->createWnd("spinwnd",280,100,20,15,LWNDT::LWNDT_SPINWND,d_frameWnd,110,LWNDSTY_HORZWND);
	d_spinWnd->setlinkWnd(d_userEdit);
	d_spinWnd->setRange(0,100);
	//
	d_sliderWnd=(ISoarSliderWnd*)root->createWnd("SliderH",300,100,200,30,LWNDT::LWNDT_SLIDERWND,d_frameWnd,110,LWNDSTY_HORZWND);
	
	d_sliderWnd->setRange(0,100);
	d_sliderWnd->setPos(53.0);

     d_dymanicProgressV =(ISoarProgress*)root->createWnd("ProgressbarV",580,50,25,400,LWNDT::LWNDT_PROGRESSBAR,d_frameWnd,110,LWNDSTY_VERTWND);
	d_dymanicProgressV->setPos(40.0);
	 ISoarWnd* p16 =root->createWnd("ipctrl",280,230,200,30,LWNDT::LWNDT_INTERETWND,d_frameWnd,110,LWNDSTY_CHILD);
	
	ISoarComboWnd *p5=(ISoarComboWnd*)root->createWnd("combobox",30,180,200,30,LWNDT::LWNDT_COMBOBOX,d_frameWnd,110,LWNDSTY_CHILD);
	//�б�list
	p5->addItem("��һ��Ŀ",0,0);
	p5->addItem("�ڶ�����Ŀ",1,0);
	p5->addItem("��������Ŀ",2,0);
	p5->addItem("���ĸ���Ŀ",3,0);
	p5->addItem("�������Ŀ",4,0);
	p5->addItem("��������Ŀ",5,0);
	p5->addItem("������Ŀ",6,0);
	p5->addItem("�ڰ˸���Ŀ",7,0);
	ISoarTabWnd* p6 =(ISoarTabWnd*)root->createWnd("tabctrl",80,280,400,230,LWNDT::LWNDT_TABCTRL,d_frameWnd,110,LWNDSTY_CHILD);
	p6->addTab(0,"tab1",0);
	p6->addTab(1,"tab2",0);
	p6->addTab(2,"tab3",0);
	p6->addTab(3,"tab4",0);
	
	ISoarListViewWnd* p17 =(ISoarListViewWnd*)root->createWnd("listVNctrl",280,250,400,300,LWNDT::LWNDT_LISTVIEWWND,p6,110,(LWNDSTY)(LWNDSTY_HSCROLL|LWNDSTY_VSCROLL));
	
	p17->setHeaderHeight(144);
	p17->addHeaderItem(100,"����");
	p17->addHeaderItem(100,"����");
	p17->addRow("�̰�����",0,0);
	p17->addRow("����ţ��",1,0);
	p17->addRow("������ţ��",2,0);
	p17->addRow("С�а趹��",3,0);
	
	p17->insertCol(0,1,"ţ��������",0,0);
	p17->insertCol(0,2,"ţ��������2",0,0);
	p17->insertCol(1,1,"�ƶ�������",0,0);
	p17->insertCol(1,2,"����������",0,0);
	p17->insertCol(2,1,"���鼦��",0,0);
	p17->insertCol(2,2,"�����е���Ŀ",0,0);
	p17->insertCol(3,1,"����˫˿",0,0);
	p17->insertCol(3,2,"����˫˿2",0,0);
	RECT rc;
	rc=p6->getClientRect();
	p17->setArea(rc);
	p17->setParent(p6);
	p6->linkTab(0,p17);
	//��ʼ��ģ̬�Ի���״̬
	d_FileDialogWnd=(ISoarFileDialogWnd*) (root->createWnd("ProFileDialog",180,50,625,400,LWNDT::LWNDT_CMM_FILEDIALOG,d_frameWnd,410,LWNDSTY_CHILD));
	d_FileDialogWnd->setTitle("I am a FileDialog window");
	d_AlphaTestWnd=root->createWnd("MDWnd",250,150,200,200,LWNDT::LWNDT_WND,d_frameWnd,510,(LWNDSTY)(LWNDSTY_SYSTITLEBAR|LWNDSTY_POPUP));
	d_AlphaTestWnd->setTitle("I am a  alpha blend test window");
	d_EnableAlphaButton=root->createWnd("MDWnd12",0,10,80,20,LWNDT::LWNDT_BUTTON,d_AlphaTestWnd,510,LWNDSTY_CHILD);
	d_EnableAlphaButton->setText("Enable Alpha Blend");
	 //��Soarϵͳ������Ϣ
	
	//�Զ��崰�ڴ���
	d_frameWnd->RegisterEventHandler(&hand);
	//����ģ̬�Ի���
	//int tt=root->DoModal(p99);
	//p99->RegisterEventHandler(&hand);
	d_EnableAlphaButton->RegisterEventHandler(&hand);
	//d_sliderWnd->RegisterEventHandler(&hand);
}

CFrameSampleUsing::~CFrameSampleUsing(void)
{
	
}
