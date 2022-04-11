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
	d_userStatic->setText("姓名：");
	
	d_userEdit=root->createWnd("erttqwq2e",100,30,250,30,LWNDT::LWNDT_EDIT,d_frameWnd,112,LWNDSTY_CHILD);
	d_passEdit=root->createWnd("erttqwqe",100,60,250,30,LWNDT::LWNDT_EDIT,d_frameWnd,102,LWNDSTY_CHILD);
	((ISoarEditWnd*)d_passEdit)->isPassword(true);
	d_passStatic=root->createWnd("erttqwmi",30,60,60,30,LWNDT::LWNDT_STATIC,d_frameWnd,102,LWNDSTY_CHILD);
	
	d_passStatic->setText("密码：");
	d_confirmButton=root->createWnd("butt",30,110,100,30,LWNDT::LWNDT_BUTTON,d_frameWnd,110,LWNDSTY_CHILD);
	d_confirmButton->setText("确  定");
	ISoarWnd *p2=root->createWnd("radio",140,110,100,30,LWNDT::LWNDT_RADIOBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p2->setText("单选项0");
	ISoarWnd *p3=root->createWnd("radio1",140,140,100,30,LWNDT::LWNDT_RADIOBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p3->setText("单选项1");
	ISoarWnd *p4=root->createWnd("checkbtn",30,140,100,30,LWNDT::LWNDT_CHECKBTN,d_frameWnd,110,LWNDSTY_CHILD);
	p4->setText("复选项");
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
	//列表list
	p5->addItem("第一项目",0,0);
	p5->addItem("第二个项目",1,0);
	p5->addItem("第三个项目",2,0);
	p5->addItem("第四个项目",3,0);
	p5->addItem("第五个项目",4,0);
	p5->addItem("第六个项目",5,0);
	p5->addItem("第七项目",6,0);
	p5->addItem("第八个项目",7,0);
	ISoarTabWnd* p6 =(ISoarTabWnd*)root->createWnd("tabctrl",80,280,400,230,LWNDT::LWNDT_TABCTRL,d_frameWnd,110,LWNDSTY_CHILD);
	p6->addTab(0,"tab1",0);
	p6->addTab(1,"tab2",0);
	p6->addTab(2,"tab3",0);
	p6->addTab(3,"tab4",0);
	
	ISoarListViewWnd* p17 =(ISoarListViewWnd*)root->createWnd("listVNctrl",280,250,400,300,LWNDT::LWNDT_LISTVIEWWND,p6,110,(LWNDSTY)(LWNDSTY_HSCROLL|LWNDSTY_VSCROLL));
	
	p17->setHeaderHeight(144);
	p17->addHeaderItem(100,"名称");
	p17->addHeaderItem(100,"数量");
	p17->addRow("奶白鱼汤",0,0);
	p17->addRow("爆炒牛肝",1,0);
	p17->addRow("土豆烧牛腩",2,0);
	p17->addRow("小葱拌豆腐",3,0);
	
	p17->insertCol(0,1,"牛肉炖板栗",0,0);
	p17->insertCol(0,2,"牛肉炖板栗2",0,0);
	p17->insertCol(1,1,"黄豆炖猪蹄",0,0);
	p17->insertCol(1,2,"冬瓜鲫鱼汤",0,0);
	p17->insertCol(2,1,"香麻鸡块",0,0);
	p17->insertCol(2,2,"第三列的项目",0,0);
	p17->insertCol(3,1,"凉拌双丝",0,0);
	p17->insertCol(3,2,"凉拌双丝2",0,0);
	RECT rc;
	rc=p6->getClientRect();
	p17->setArea(rc);
	p17->setParent(p6);
	p6->linkTab(0,p17);
	//初始化模态对话框状态
	d_FileDialogWnd=(ISoarFileDialogWnd*) (root->createWnd("ProFileDialog",180,50,625,400,LWNDT::LWNDT_CMM_FILEDIALOG,d_frameWnd,410,LWNDSTY_CHILD));
	d_FileDialogWnd->setTitle("I am a FileDialog window");
	d_AlphaTestWnd=root->createWnd("MDWnd",250,150,200,200,LWNDT::LWNDT_WND,d_frameWnd,510,(LWNDSTY)(LWNDSTY_SYSTITLEBAR|LWNDSTY_POPUP));
	d_AlphaTestWnd->setTitle("I am a  alpha blend test window");
	d_EnableAlphaButton=root->createWnd("MDWnd12",0,10,80,20,LWNDT::LWNDT_BUTTON,d_AlphaTestWnd,510,LWNDSTY_CHILD);
	d_EnableAlphaButton->setText("Enable Alpha Blend");
	 //向Soar系统发送消息
	
	//自定义窗口处理
	d_frameWnd->RegisterEventHandler(&hand);
	//弹出模态对话框
	//int tt=root->DoModal(p99);
	//p99->RegisterEventHandler(&hand);
	d_EnableAlphaButton->RegisterEventHandler(&hand);
	//d_sliderWnd->RegisterEventHandler(&hand);
}

CFrameSampleUsing::~CFrameSampleUsing(void)
{
	
}
