#include "Interpreter.h"

CInnerInterpreter::CInnerInterpreter(void)
{
}

CInnerInterpreter::~CInnerInterpreter(void)
{
}
void CInnerInterpreter::Release(void)
{
	delete this;
}
bool CInnerInterpreter::parseFile(IXMLHandler& handler, const CLeeString& filename,DWORD dwThunkSize) 
{
	//��ȡ�ļ���С��ѭ����ȡ
	DWORD dwThunSize=dwThunkSize;
	size_t totalsize =0;
	size_t cursize =0;
	
	FILE* file = _tfopen(filename.c_str(),_T("rb") );
	if (file == 0)
	{
		::MessageBoxA(NULL,filename.c_str(),"",MB_OK);
		return false;
	}
	fseek(file, 0, SEEK_END);
	totalsize= ftell(file);
	if (dwThunkSize ==0)
	{
		dwThunSize =totalsize;
	}
	void *pThunkData =::malloc(dwThunSize);
	do 
	{
		::ZeroMemory(pThunkData,dwThunSize);
		//������������
		//////////////////////////////////////////////////////////////
		fseek(file,cursize, SEEK_SET);
		//���õ���Ӧ��λ��
		const size_t size_read = fread(pThunkData, sizeof(BYTE), dwThunSize, file);
		/////////////////////////////////////////////////////////////////
		cursize +=size_read;
		//����һ�ν���һ��
		if ( ! d_parser.Parse(&handler,reinterpret_cast<BYTE*>(pThunkData), size_read) )
		{
			fclose(file);
			return false;
		}
	} while (cursize< totalsize);
	fclose(file);
	::free(pThunkData);
	return true;
}
bool CInnerInterpreter::parseData(IXMLHandler& handler, void* pData,size_t dataSize)
{
	return d_parser.Parse(&handler,reinterpret_cast<BYTE*>(pData),dataSize);
}