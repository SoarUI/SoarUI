#include "stdInterpreter.h"

CSTDInterpreter::CSTDInterpreter(HMODULE hModule)
{
	d_hModule =hModule ;
}

CSTDInterpreter::~CSTDInterpreter(void)
{
}
void CSTDInterpreter::Release()
{
	delete this;
}
bool CSTDInterpreter::parseFile(IXMLHandler& handler, const CLeeString& filename,DWORD dwThunkSize) 
{
	//获取文件大小，循环获取
	DWORD dwThunSize=dwThunkSize;
	size_t totalsize =0;
	size_t cursize =0;
	
	FILE* file = _tfopen(filename.c_str(),_T("rb") );
	if (file == 0)
	{
		return false;
	}
	fseek(file, 0, SEEK_END);
	totalsize= ftell(file);
	if (dwThunkSize ==0)
	{
		dwThunSize =totalsize;
	}
	void *pThunkData =::malloc(dwThunSize);
	d_parser.setmoudle(d_hModule);
	do 
	{
		::ZeroMemory(pThunkData,dwThunSize);
		//分批加载内容
		//////////////////////////////////////////////////////////////
		fseek(file,cursize, SEEK_SET);
		//设置到相应的位置
		const size_t size_read = fread(pThunkData, sizeof(TCHAR), dwThunSize*sizeof(TCHAR), file);
		/////////////////////////////////////////////////////////////////
		cursize +=size_read;
		//加载一段解析一段
		if ( ! d_parser.Parse(&handler,reinterpret_cast<TCHAR*>(pThunkData), size_read) )
		{
			fclose(file);
			return false;
		}
	} while (cursize< totalsize);
	fclose(file);
	::free(pThunkData);
	return true;
}
bool CSTDInterpreter::parseData(IXMLHandler& handler, void* pData,size_t dataSize)
{
	d_parser.setmoudle(d_hModule);
	return d_parser.Parse(&handler,reinterpret_cast<TCHAR*>(pData),dataSize);
}