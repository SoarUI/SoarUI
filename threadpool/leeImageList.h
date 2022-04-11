#ifndef __CLeeImageList__h__
#define __CLeeImageList__h_
#include<windows.h>
#include"Commctrl.h"
class CLeeImageList
{
public:
	CLeeImageList(void);
public:
	virtual ~CLeeImageList(void);
	bool Attach(HIMAGELIST hsm);
	bool DeleteImageList();
	HIMAGELIST GetHandle(void);
	bool GetImageInfo(int i,IMAGEINFO *ImageInfo);
private:
	HIMAGELIST m_hsm;
};
#endif