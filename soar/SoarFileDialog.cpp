#include "SoarFileDialog.h"
#include "SoarRoot.h"
#include <shellapi.h>
#include<Shlobj.h>
#include <shlwapi.h> 

CLeeFileDialogWnd::CLeeFileDialogWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),
d_combFilter(NULL),
d_combDir(NULL),
d_fileEdit(NULL),
d_fileViewWnd(NULL),
d_openButton(NULL),
d_cancelButton(NULL),
d_dirEditStatic(NULL),
d_menu(NULL),
d_quit(0)
{
	d_wndStyle=LWNDSTY_POPUP;
}
CLeeFileDialogWnd::~CLeeFileDialogWnd(void)
{
}
void CLeeFileDialogWnd::InitializeSegments(void)
{

	d_wndStyle = (LWNDSTY)(d_wndStyle|(LWNDSTY_SYSTITLEBAR|LWNDSTY_VSCROLL|LWNDSTY_HSCROLL));
	CSoarWnd::InitializeSegments();
	AbsoluteDim aDimx(0);
	AbsoluteDim aDimy(0);
	AbsoluteDim aDimH(25);
	AbsoluteDim aDimW(85);
	UDim dmin(0,0);
	Dimension w(aDimW,DT_WIDTH);
	Dimension h(aDimH,DT_HEIGHT);
	Dimension y(aDimy,DT_TOP_EDGE);
	UnifiedDim ux(dmin,DT_X_POSITION);
	Dimension x(ux,DT_LEFT_EDGE);
	CLeeString itemname ;
	itemname.Format("SOARREAM__%p__%s",this,"staticDir");
	d_dirStatic=(ISoarStaticWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,x,y,w,h,LWNDT_STATIC,this,d_zIndex+1);
	d_dirStatic->setText("路  径：");
	UDim udw(0.6,0);
	UnifiedDim w1(udw,DT_WIDTH);
	Dimension ww1(w1,DT_WIDTH);
	aDimH.setValue(30);
	Dimension h1(aDimH,DT_HEIGHT);
	UDim dx(0,100);
	UnifiedDim udx(dx,DT_WIDTH);
	Dimension uddx(udx,DT_X_POSITION);
	itemname.Format("SOARREAM__%p__%s",this,"ccomboxDir");
	d_combDir=(ISoarComboWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,uddx,y,ww1,h1,LWNDT_COMBOBOX,this,d_zIndex+1);
	d_combDir->setText("请选择：");
	UDim uxw(0.7,60);
	UnifiedDim wxxx1(uxw,DT_WIDTH);
	Dimension uddx1(wxxx1,DT_X_POSITION);
	itemname.Format("SOARREAM__%p__%s",this,"backupDir");
	d_actionButton=(ISoarButtonWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,uddx1,y,w,h,LWNDT_BUTTON,this,d_zIndex+1);
	d_actionButton->setText("后 退");

	UDim udw1(1.0,-2);
	UnifiedDim uw(udw1,DT_WIDTH);
	Dimension w2(uw,DT_WIDTH);
	UDim udh1(1.0,-60);
	UnifiedDim udh(udh1,DT_HEIGHT);
	Dimension h2(udh,DT_HEIGHT);
	UDim udy(0,26);
	UnifiedDim y1(udy,DT_TOP_EDGE);
	Dimension y2(y1,DT_Y_POSITION);
	itemname.Format("SOARREAM__%p__%s",this,"sysFileViewWnd");
	d_fileViewWnd=(ISoarListViewWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,x,y2,w2,h2,LWNDT_LISTVIEWWND,this,
			d_zIndex+1,(LWNDSTY)(LWNDSTY_CHILD|LWNDSTY_SYSHEADERCTRL|LWNDSTY_HSCROLL|LWNDSTY_VSCROLL) );
	d_fileViewWnd->setOwnerWnd(this);
	d_fileViewWnd->setHeaderHeight(24);
	d_fileViewWnd->addHeaderItem(500,"文件名称");
	d_fileViewWnd->addHeaderItem(400,"文件大小");
	itemname.Format("SOARREAM__%p__%s",this,"staticEditDir");
	UDim udby(1.0,-30);
	UnifiedDim yb(udby,DT_HEIGHT);
	Dimension dyb(yb,DT_Y_POSITION);
	d_dirEditStatic=(ISoarStaticWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,x,dyb,w,h,LWNDT_STATIC,this,d_zIndex+1);
	d_dirEditStatic->setText("文件过滤：");
	UDim udwb(0.4,0);
	UnifiedDim w1b(udwb,DT_WIDTH);
	Dimension ww1b(w1b,DT_WIDTH);
	itemname.Format("SOARREAM__%p__%s",this,"ccomboxfile");
	d_combFilter=(ISoarComboWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,uddx,dyb,ww1b,h1,LWNDT_COMBOBOX,this,d_zIndex+1);
	
	d_combFilter->addItem("*.*",0,NULL);
	d_combFilter->addItem("*.exe",1,NULL);
	itemname.Format("SOARREAM__%p__%s",this,"openbutton");
	UDim udbx(1.0,-220);
	UnifiedDim xb(udbx,DT_WIDTH);
	Dimension dxb(xb,DT_X_POSITION);
	aDimW.setValue(80);
	aDimH.setValue(25);
	Dimension wb(aDimW,DT_WIDTH);
	Dimension hb(aDimH,DT_HEIGHT);
	d_openButton=(ISoarButtonWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,dxb,dyb,wb,hb,LWNDT_BUTTON,this,d_zIndex+1);
	
	d_openButton->setText("打 开");
	UDim udbxx(1.0,-90);
	UnifiedDim xbx(udbxx,DT_WIDTH);
	Dimension dxbx(xbx,DT_X_POSITION);
	itemname.Format("SOARREAM__%p__%s",this,"closebutton");
	d_cancelButton=(ISoarButtonWnd*)CSoarRoot::getSingletonPtr()->createWnd(itemname,dxbx,dyb,wb,hb,LWNDT_BUTTON,this,d_zIndex+1);
	
	d_cancelButton->setText("取 消");
	//////////////////////////////////////////////////////////////////////////
	
	int d=0;
	CLeeString szTmpDir;
	//fill the scopeComBo with special path
	TCHAR tmpDir[_MAX_PATH];
	ZeroMemory(tmpDir,sizeof(tmpDir));
	SHGetSpecialFolderPath(d_rootWnd,tmpDir,CSIDL_DESKTOP/*桌面*/,FALSE/*不创建*/);
	szTmpDir.empty();
	szTmpDir.Format(TEXT("桌面(%s)"),tmpDir);
	 d_combDir->addItem(szTmpDir.c_str(),0,NULL);
	 d++;
	ZeroMemory(tmpDir,sizeof(tmpDir));
	SHGetSpecialFolderPath(d_rootWnd,tmpDir,CSIDL_PERSONAL/*我的文档*/,FALSE/*不创建*/);
	szTmpDir.empty();
	szTmpDir.Format(TEXT("我的文档(%s)"),tmpDir);
	 d_combDir->addItem(szTmpDir.c_str(),1,NULL);
	 d++;
	ZeroMemory(tmpDir,sizeof(tmpDir));
	SHGetSpecialFolderPath(d_rootWnd,tmpDir,CSIDL_RECENT/*我最近的文档*/,FALSE/*不创建*/);
	szTmpDir.empty();
	szTmpDir.Format(TEXT("我最近的文档(%s)"),tmpDir);
	 d_combDir->addItem(szTmpDir.c_str(),2,NULL);
	 d++;
	 DWORD dwDriver=GetLogicalDrives();
	CLeeString   szDrive   =   TEXT("?:\\"); 
	int i=0;
	/*
	0x0000000000000001  1  1  1   1   1     1    1     1
	                 H   G   F    E   D    C      B     A
	*/
	while(dwDriver) 
	{  
		if (1 & dwDriver)
		{
			szDrive.clear();
			szDrive   =   TEXT("?:\\"); 
			szDrive.Format("%c:\\",(char)(0x41+i));
			//检查磁盘类型
			int uType =::GetDriveType(szDrive.c_str());
			if(uType==DRIVE_FIXED)//只有固定的才行
			  d_combDir->addItem(szDrive.c_str(),3,NULL);
			d++;
		}
	  dwDriver>>=1;
      i++;
	} 
	d_combDir->setCurSel(0);
	d_combDir->setText(d_combDir->getItemString(0));
	CLeeString str=" ";
	FindString(d_combDir->getItemString(d),str);
	d_combFilter->setCurSel(0);
	
	find_all_files(str.c_str(),d_combFilter->getItemString(0));
	d_Prevdirstring =" ";
	d_Curdirstring=str;
	d_actionPath.push(d_Prevdirstring);
}
int CLeeFileDialogWnd::find_all_files(const char * lpPath,CLeeString &filter)
{
	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	strcpy(szFind,lpPath);
	strcat(szFind,"\\");
	strcat(szFind,filter.c_str() );
	HANDLE hFind=::FindFirstFile(szFind,&FindFileData);
	if(INVALID_HANDLE_VALUE == hFind)   
		return -1;

	do
	{
		if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//目录
		{
			if(strcmp(FindFileData.cFileName,".")!=0 && strcmp(FindFileData.cFileName, "..")!=0)//根目录
			{
				//发现子目录，递归之
				char szFile[MAX_PATH] = {0};
				strcpy(szFile,lpPath);
				strcat(szFile,"\\");
				strcat(szFile,FindFileData.cFileName);
				//find_all_files(szFile);
				d_fileViewWnd->addRow(FindFileData.cFileName,0,NULL);
				d_fileViewWnd->insertCol(d_fileViewWnd->getItemCount()-1,1,"Folder",0,0);
			}
		}
		else//文件
		{
			//找到文件，处理之
			d_fileViewWnd->addRow(FindFileData.cFileName,0,NULL);
			DWORDLONG FileSize = (FindFileData.nFileSizeHigh * (MAXDWORD+1)*1.0) + FindFileData.nFileSizeLow;
			CLeeString szSize;
			szSize.Format("%.1fKB",FileSize*1.0/1024 );
			d_fileViewWnd->insertCol(d_fileViewWnd->getItemCount()-1,1,szSize,0,0);
		}
	}while(::FindNextFile(hFind,&FindFileData));

	::FindClose(hFind);
	return 0;
}
//事件处理
LRESULT  CLeeFileDialogWnd::HandleEvent ( ISoarWnd* pOwner,UINT uMsg ,WPARAM wp ,LPARAM lp )
{
	
	LRESULT lr= CSoarWnd::HandleEvent(pOwner,uMsg,wp,lp);
	
	if (uMsg == SOAR_COMMAND )
	{
		SOARMSG* leeMsg=(SOARMSG*)wp;
		//点击打开按钮
		if(leeMsg->mouseEvent==SOAR_LCLICK_UP && leeMsg->sourceWnd==d_openButton)
		{
			if (IsFolderAndExit(d_selectString))
			{
				int d=d_combFilter->getSelectedItemIndex();
				d_fileViewWnd->clear();
				find_all_files(d_selectString.c_str(),d_combFilter->getItemString(d));
			}
			else if (IsFile(d_selectString))
			{
				d_quit =2;
				::ShellExecute(NULL,"open",d_selectString.c_str(),NULL,NULL,SW_SHOW);
			}
			return lr ;
		}
		//点击后退按钮
		if (leeMsg->mouseEvent == SOAR_LCLICK_UP && leeMsg->sourceWnd==d_actionButton)
		{
			if (d_actionPath.empty()==false)
			{
				CLeeString path =d_actionPath.top();
				d_actionPath.pop();
				int d=d_combDir->getSelectedItemIndex();
				d_fileViewWnd->clear();
				CLeeString str;
				FindString(path,str);
				d=d_combFilter->getSelectedItemIndex();
				find_all_files(str.c_str(),d_combFilter->getItemString(d));
				d_combDir->setText(str);
				d_Curdirstring=str;
			}
			return lr ;
			
		}
		//点击退出按钮
		if(leeMsg->mouseEvent==SOAR_LCLICK_UP && leeMsg->sourceWnd==d_cancelButton)
		{
			d_quit =1;
			//作为主窗口的话，退出整个程序
			if( CSoarRoot::getSingletonPtr()->CheckIsSoarMainWnd(this) )
				::PostMessage(d_rootWnd,WM_QUIT,0,0);
			if(d_bModalState){//是模态兑换框
				CSoarRoot::getSingletonPtr()->EndModal();
			}
			//正常的话：0-设置自动销毁 delete 1-没有设置，则隐藏
			if(d_bAutoDestroy){
				CSoarRoot::getSingletonPtr()->destroyWnd( this);
			}
			else{
				this->setState(LWNDST_HIDE);
			}
			return lr ;
		}
		//
		
		//右键消息
		if(leeMsg->mouseEvent==SOAR_RCLICK_UP && leeMsg->sourceWnd==d_fileViewWnd)
		{
			OnCommandRClick(leeMsg);
			return lr ;
		}
		
			//双击listview
			if (leeMsg->mouseEvent==SOAR_LDBCLICK && leeMsg->sourceWnd==d_fileViewWnd)
			{
				CLeeString str;
				str.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(d_fileViewWnd->getSelectedItemIndex(),0).c_str());
				if (IsFolderAndExit(str))
				{
					d_selectString.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(d_fileViewWnd->getSelectedItemIndex(),0).c_str());
					d_combDir->setText(d_selectString);
					int d=d_combFilter->getSelectedItemIndex();
					d_fileViewWnd->clear();
					find_all_files(d_selectString.c_str(),d_combFilter->getItemString(d));
					d_Prevdirstring =d_Curdirstring;
					d_Curdirstring=d_selectString;
					d_actionPath.push(d_Prevdirstring);
				}
				else if (IsFile(str))
				{
					ShellExecute(NULL,"open",str.c_str(),NULL,NULL,SW_SHOW);
				}
				return lr;
			}
			if (leeMsg->mouseEvent == SOAR_LCLICK_UP && leeMsg->sourceWnd==d_fileViewWnd)
		{
			//点击项
			d_selectString.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(d_fileViewWnd->getSelectedItemIndex(),0).c_str());
			return lr ;
			
		}
		return lr ;
	   }
	//接受到某反馈
		if (uMsg == SOAR_SELCHANGED)
		{
			SOARMSG* leeMsg=(SOARMSG*)wp;
			if (leeMsg->sourceWnd == d_combDir)
			{
				int d=d_combDir->getSelectedItemIndex();
				d_fileViewWnd->clear();
				CLeeString str;
				FindString(d_combDir->getItemString(d),str);
				d=d_combFilter->getSelectedItemIndex();
				find_all_files(str.c_str(),d_combFilter->getItemString(d));
				d_Prevdirstring =d_Curdirstring;
				d_Curdirstring=str;
				d_actionPath.push(d_Prevdirstring);
				d_actionPath.push(str);
				return lr;
			}
			if (leeMsg->sourceWnd==d_combFilter)
			{
				int d=d_combDir->getSelectedItemIndex();
				d_fileViewWnd->clear();
				CLeeString str;
				FindString(d_combDir->getItemString(d),str);
				d=d_combFilter->getSelectedItemIndex();
				find_all_files(str.c_str(),d_combFilter->getItemString(d));
				return lr ;
			}
			//list项改变
			if (leeMsg->sourceWnd==d_fileViewWnd)
			{
				d_selectString.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(leeMsg->wParam,0).c_str());
				return lr ;
			}
			return lr ;
		}
		//菜单会返回这个消息
		if (uMsg == SOAR_ITEMSELECTED)
		{
			SOARMSG* leeMsg=(SOARMSG*)wp;
			if (leeMsg->mouseEvent==SOAR_LCLICK_UP)
			{
				//点击项
				d_selectString.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(leeMsg->wParam,0).c_str());
				return lr ;
			}
			//双击在Item上 
			if (leeMsg->mouseEvent==SOAR_LDBCLICK)
			{
				CLeeString str;
				str.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(leeMsg->wParam,0).c_str());
				if (IsFolderAndExit(str))
				{
					d_selectString.Format("%s\\%s" ,d_Curdirstring.c_str(),d_fileViewWnd->getItemString(leeMsg->wParam,0).c_str());
					d_combDir->setText(d_selectString);
					int d=d_combFilter->getSelectedItemIndex();
					d_fileViewWnd->clear();
					find_all_files(d_selectString.c_str(),d_combFilter->getItemString(d));
					d_Prevdirstring =d_Curdirstring;
					d_Curdirstring=d_selectString;
					d_actionPath.push(d_Prevdirstring);
				}
				else if (IsFile(str))
				{
					::ShellExecute(NULL,"open",str.c_str(),NULL,NULL,SW_SHOW);
				}
				return lr;
			}
			
			//右击listview
			if(leeMsg->mouseEvent==SOAR_RCLICK_UP && leeMsg->sourceWnd==d_fileViewWnd)
			{
				OnCommandRClick(leeMsg);
				return lr;
			}
			return lr;
		}
		
		if (uMsg == SOAR_MENUITEMSELECTED)
		{
			SOARMSG* leeMsg=(SOARMSG*)wp;
			if (leeMsg->mouseEvent==SOAR_LCLICK_DOWN)
			{
				if (leeMsg->lParam==0x110)//菜单后退项
				{
					if (d_actionPath.empty()==false)
					{
						CLeeString path =d_actionPath.top();
						d_actionPath.pop();
						int d=d_combDir->getSelectedItemIndex();
						d_fileViewWnd->clear();
						CLeeString str;
						FindString(path,str);
						d=d_combFilter->getSelectedItemIndex();
						find_all_files(str.c_str(),d_combFilter->getItemString(d));
						d_combDir->setText(str);
						d_Curdirstring=str;
					}
				}
				if (leeMsg->lParam==0x112 ||
					leeMsg->lParam==0x111)//菜单打开项
				{
					if (IsFolderAndExit(d_selectString))
					{
						d_combDir->setText(d_selectString);
						int d=d_combFilter->getSelectedItemIndex();
						d_fileViewWnd->clear();
						find_all_files(d_selectString.c_str(),d_combFilter->getItemString(d));
						d_Prevdirstring =d_Curdirstring;
						d_Curdirstring=d_selectString;
						d_actionPath.push(d_Prevdirstring);
					}
					else if (IsFile(d_selectString))
					{
						ShellExecute(NULL,"open",d_selectString.c_str(),NULL,NULL,SW_SHOW);
					}
					return lr;
				}
				return lr ;
			}
		}
	return lr;
}
void CLeeFileDialogWnd::OnCommandRClick(SOARMSG* pMsg){
	if(pMsg->mouseEvent==SOAR_RCLICK_UP && pMsg->sourceWnd==d_fileViewWnd)
	{
		POINT pt;
		::GetCursorPos(&pt);
		::ScreenToClient(d_rootWnd,&pt);
		AbsoluteDim aDimy(100);
		AbsoluteDim aDimx(50);
		AbsoluteDim aDimH(200);
		AbsoluteDim aDimW(300);


		Dimension w(aDimW,DT_WIDTH);
		Dimension h(aDimH,DT_HEIGHT);
		Dimension y(aDimy,DT_Y_POSITION);
		Dimension x(aDimx,DT_X_POSITION);
		CLeeString itemname ;
		itemname.Format("LeeFileDialogWnd_systemTabButtonMenu%d",0);
		if (d_menu)
		{
			d_menu->setState(LWNDST_SHOW);
			CSoarRoot::getSingletonPtr()->setFocusWnd(d_menu);
			return ;
		}
		d_menu =(ISoarMenuWnd*)CSoarRoot::getSingletonPtr()->createPopupWnd(itemname,x,y,w,h,LWNDT_POPMENU,this,SOAR_WNDINDEX_TOP);
				
		d_menu->setOwnerWnd(this);
		d_menu->AppendMenu(0x110,NULL,"后退");
		d_menu->AppendMenu(0x111,NULL,"前进");
		d_menu->AppendMenu(0x112,NULL,"打开");
		d_menu->AppendMenu(0x113,NULL,"待定项");
		//调整菜单窗口出现位置
		CSoarRoot::getSingletonPtr()->setFocusWnd(d_menu);
		return ;
	}
}
BOOL  CLeeFileDialogWnd::IsFile(CLeeString szFileName)
{
	//存在并且是文件
	if(PathFileExists(szFileName.c_str()) && !IsFolder(szFileName))
		return TRUE;
	else//文件夹
	{
		return FALSE;
	}
}
BOOL CLeeFileDialogWnd::IsDisco(CLeeString szFileName)
{
	return ::PathIsRoot(szFileName.c_str());
}
BOOL CLeeFileDialogWnd::IsFolder(CLeeString szFileName)//不判断文件是否存在
{ 
	//是文件夹
	if(PathIsDirectory( szFileName.c_str() ) != 0 
		&& !PathIsRoot(szFileName.c_str() ))
		return TRUE;
	else//文件夹
	{
		return FALSE;
	}
}
BOOL CLeeFileDialogWnd::IsFolderAndExit(CLeeString szFileName)//存在并是文件夹SHELLAPI实现
{
	//存在并且是文件
	if(PathFileExists(szFileName.c_str() )!=0 
		&& PathIsDirectory( szFileName.c_str()  ) != 0 
		&& !PathIsRoot(szFileName.c_str() ) )
		return TRUE;
	else//文件夹
	{
		return FALSE;
	}
}
void CLeeFileDialogWnd::FindString(CLeeString& szPath,CLeeString &szRet)
{
	int nleftPos=0;
	int nRightPos=0;
	nleftPos=szPath.find(TEXT('('));
	nRightPos=szPath.find(TEXT(')'));
	if (nleftPos&&nRightPos>nleftPos)
	{
		szRet=szPath.substr(nleftPos+1,nRightPos-nleftPos-1);
	}
	else
	{
		szRet=szPath;
	}
}
CLeeString CLeeFileDialogWnd::getSelectString(void)
{
	return d_selectString;
}