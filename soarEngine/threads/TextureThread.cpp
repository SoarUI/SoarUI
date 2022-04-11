#include "TextureThread.h"
#include "..\XMLHandler\XMLAnalyser.h"
#include "..\Mouduls\parsers\ExpatParser.h"
#include "..\Handler\Texture_xmlHandler.h"
#include "../../SoarHeader/leelog.h"
#include <process.h>

TextureThread::TextureThread(void)
{
}

TextureThread::~TextureThread(void)
{
	CloseHandle(d_hStartEvt);
}
unsigned int TextureThread::threadWorker(void* param)
{
	TextureThread * thrd =(TextureThread*)param;
	if (!thrd)
	{
		return -1;
	}
	CLeeString file =thrd->d_file;
	HANDLE hThread =thrd->d_hthread;
	
	// create handler object
	Texture_xmlHandler handler;

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

		Dbgc_DumpX(_T("dump_err")
			,_T("Texture Thread - loading texture data from file  has failed."), 0);
		throw(-1);
	}
	//等到通知开始
	thrd->setSafeExit();
    //关闭现场
	CloseHandle(hThread);
	return 0;
}
void TextureThread::startThread(const CLeeString &filename)
{
	d_file =filename;
	DWORD threadID =0;
	d_hStartEvt =CreateEvent(NULL,false,false,NULL);
	d_hthread= (HANDLE)_beginthreadex(NULL, 0, TextureThread::threadWorker, this,  
		0, (UINT*)&threadID);
	 WaitForSingleObject(d_hStartEvt,INFINITE);
}
void TextureThread::setSafeExit(void)
{
	SetEvent(d_hStartEvt);
}