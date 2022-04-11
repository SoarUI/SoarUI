#include "WidgetlookThread.h"
#include "../../SoarHeader/leelog.h"
#include "..\Handler\widgetlook_xmlhandler.h"
#include "..\XMLHandler\XMLAnalyser.h"
#include "..\Mouduls\parsers\ExpatParser.h"
#include <process.h>

WidgetlookThread::WidgetlookThread(void)
{
}

WidgetlookThread::~WidgetlookThread(void)
{
	CloseHandle(d_hStartEvt);
}
unsigned int WidgetlookThread::threadWorker(void* param)
{
	WidgetlookThread * thrd =(WidgetlookThread*)param;
	if (!thrd)
	{
		return -1;
	}
	CLeeString file =thrd->d_file;
	HANDLE hThread =thrd->d_hthread;
	
	// create handler object
	CWidgetlook_xmlHandler handler;

	// perform parse of XML data
	try
	{
		IXMLAnalyser* FeelParser = new ExpatParser();
		FeelParser->parseXMLFile(handler, file);
		delete FeelParser;
	}
	catch(...)
	{
		throw(_T("null pointer found"));
		Dbgc_DumpX(_T("dump")
			,_T("widgetlook thread - loading of look and feel data from file  has failed."), 0);
	}
	//等到通知开始
	thrd->setSafeExit();
    //关闭现场
	CloseHandle(hThread);
	return 0;
}
void WidgetlookThread::startThread(const CLeeString &filename)
{
	d_file =filename;
	DWORD threadID =0;
	d_hStartEvt =CreateEvent(NULL,false,false,NULL);
	d_hthread= (HANDLE)_beginthreadex(NULL, 0, WidgetlookThread::threadWorker, this,  
		0, (UINT*)&threadID);
	 WaitForSingleObject(d_hStartEvt,INFINITE);
}
void WidgetlookThread::setSafeExit(void)
{
	SetEvent(d_hStartEvt);
}