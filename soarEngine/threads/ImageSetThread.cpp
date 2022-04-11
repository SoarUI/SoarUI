#include "ImageSetThread.h"
#include "..\XMLHandler\XMLAnalyser.h"
#include "..\XMLHandler\Imageset_xmlHandler.h"
#include "..\Mouduls\parsers\ExpatParser.h"
#include "../../SoarHeader/leelog.h"
#include <process.h>

CImageSetThread::CImageSetThread(void)
{
}

CImageSetThread::~CImageSetThread(void)
{
	CloseHandle(d_hStartEvt);
}
unsigned int CImageSetThread::threadWorker(void* param)
{
	CImageSetThread * thrd =(CImageSetThread*)param;
	if (!thrd)
	{
		return -1;
	}
	CLeeString file =thrd->d_file;
	HANDLE hThread =thrd->d_hthread;
	
	// create handler object
	Imageset_xmlHandler handler;
	// perform parse of XML data
	try
	{
		IXMLAnalyser* FeelParser = new ExpatParser();
		FeelParser->parseXMLFile(
			handler, file );
		delete FeelParser;
	}
	catch(...)
	{
		throw(_T("null pointer found"));
		Dbgc_DumpX(_T("dump")
			,_T("imageset thread - loading imagset data from file  has failed."), 0);
	}
	//等到通知开始
	thrd->setSafeExit();
    //关闭现场
	CloseHandle(hThread);
	return 0;
}
void CImageSetThread::startThread(const CLeeString &filename)
{
	d_file =filename;
	DWORD threadID =0;
	d_hStartEvt =CreateEvent(NULL,false,false,NULL);
	d_hthread= (HANDLE)_beginthreadex(NULL, 0, CImageSetThread::threadWorker, this,  
		0, (UINT*)&threadID);
	 WaitForSingleObject(d_hStartEvt,INFINITE);
}
void CImageSetThread::setSafeExit(void)
{
	SetEvent(d_hStartEvt);
}