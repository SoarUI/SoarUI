#pragma once
#include "../../SoarHeader/leestring.h"

class TextureThread
{
public:
	TextureThread(void);
	~TextureThread(void);
	void startThread(const CLeeString &filename);
	void setSafeExit(void);
	static unsigned int CALLBACK threadWorker(void* param);
public:
	CLeeString d_file;
	HANDLE d_hthread;
private:
	HANDLE d_hStartEvt;
	
};
