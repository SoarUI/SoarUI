#pragma once
#include "../../SoarHeader/leestring.h"

class CImageSetThread
{
public:
	CImageSetThread(void);
	~CImageSetThread(void);
	void startThread(const CLeeString &filename);
	void setSafeExit(void);
	static unsigned int CALLBACK threadWorker(void* param);
public:
	CLeeString d_file;
	HANDLE d_hthread;
private:
	HANDLE d_hStartEvt;
	
};
