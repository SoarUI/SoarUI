#pragma once
#include "../../SoarHeader/leestring.h"

class WidgetlookThread
{
public:
	WidgetlookThread(void);
	~WidgetlookThread(void);
	void startThread(const CLeeString &filename);
	void setSafeExit(void);
	static unsigned int CALLBACK threadWorker(void* param);
public:
	CLeeString d_file;
	HANDLE d_hthread;
private:
	HANDLE d_hStartEvt;
	
};
