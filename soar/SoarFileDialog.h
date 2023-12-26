#pragma once
#include "leeframewnd.h"
#include "../SoarHeader/leestring.h"
#include "SoarListViewWnd.h"
#include "SoarEditWnd.h"
#include "SoarComboWnd.h"
#include "SoarButtonWnd.h"
#include "SoarStaticWnd.h"
#include "../SoarHeader/isoarmenuwnd.h"
 #include<stack>
#include "../SoarHeader/soarExport.h"
class CLeeFileDialogWnd :
	public CSoarWnd
{
public:
	CLeeFileDialogWnd(HWND root,ISoarEngine* Eng);
	~CLeeFileDialogWnd(void);
	typedef std::stack<CLeeString> PATHSTACK;
	virtual CLeeString getSelectString(void);
	virtual void InitializeSegments(void);
	virtual BOOL  HandleEvent ( ISoarWnd* pOwner,UINT uMsg ,WPARAM wp ,LPARAM lp, LRESULT& lr);
    virtual int find_all_files(const char * lpPath,const CLeeString &filter);
	
private:
	BOOL IsFile(CLeeString szFileName);
	BOOL IsDisco(CLeeString szFileName);
	BOOL IsFolder(CLeeString szFileName);
	BOOL IsFolderAndExit(CLeeString szFileName);
	void FindString(const CLeeString& szPath,CLeeString &szRet);
	void OnCommandRClick(SOARMSG* pMsg);
private:
	ISoarComboWnd * d_combDir;
	ISoarComboWnd * d_combFilter;
	ISoarEditWnd * d_fileEdit;
	ISoarListViewWnd * d_fileViewWnd;
	ISoarButtonWnd* d_openButton;
	ISoarButtonWnd * d_cancelButton;
	ISoarButtonWnd * d_actionButton;
	CLeeString d_filterstring;
	CLeeString d_Curdirstring;
	CLeeString d_Prevdirstring;
	CLeeString d_selectString;
	ISoarStaticWnd * d_dirStatic;
	ISoarStaticWnd * d_dirEditStatic;
	ISoarMenuWnd *d_menu;
	PATHSTACK d_actionPath;
	int d_quit;

};
