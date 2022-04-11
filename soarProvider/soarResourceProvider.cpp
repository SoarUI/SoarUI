
#include "SoarResourceProvider.h"
#include <stdio.h>
#if defined(__WIN32__) || defined(_WIN32)
#   include <io.h>
#   include <windows.h>
#   include <string>

//----------------------------------------------------------------------------//
std::wstring Utf8ToUtf16(const std::string& utf8text)
{
    const int textLen = MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(),
                                            utf8text.size() + 1, 0, 0);

    std::wstring wideStr(textLen, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8text.c_str(), utf8text.size() + 1,
                        &wideStr[0], wideStr.size());
    return wideStr;
}

//----------------------------------------------------------------------------//
std::string Utf16ToString(const wchar_t* const utf16text)
{
    const int len = WideCharToMultiByte(CP_UTF8, 0, utf16text, -1,
                                        0, 0, 0, 0);

    char* buff = new char[len + 1];
    WideCharToMultiByte(CP_UTF8, 0, utf16text, -1,
                        reinterpret_cast<char*>(buff), len, 0, 0);
	const std::string result(buff);
    delete[] buff;

    return result;
}

//----------------------------------------------------------------------------//
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <dirent.h>
#   include <fnmatch.h>
#endif

//----------------------------------------------------------------------------//
CSoarResourceProvider::CSoarResourceProvider(void)
{
	d_identityString =_T("soarProviderV0.1");
}
CSoarResourceProvider::~CSoarResourceProvider(void)
{

}
void CSoarResourceProvider::Release()
{
	delete this;
}
void CSoarResourceProvider::loadRawDataContainer(const CLeeString& filename,
                                                   IRawDataContainer& output,size_t & total,const _ProviderKeyAny *key)
{
	output.setSize(0);

#ifdef UNICODE
	
    FILE* file = _wfopen(filename.c_str(),_T("rb") );
#else
    FILE* file = fopen(filename.c_str(), _T("rb"));
#endif

    if (file == 0)
	{
		output.setSize(0);
		   return ;
	}

    fseek(file, 0, SEEK_END);
    const long size = ftell(file);
    fseek(file, 0, SEEK_SET);
    const size_t size_read = fread(output.getDataPtr(), sizeof(TCHAR), size*sizeof(TCHAR), file);
    fclose(file);

    if (size_read != size)
    {
		return;
    }
    output.setSize(size);
}

//----------------------------------------------------------------------------//
void CSoarResourceProvider::unloadRawDataContainer(IRawDataContainer& data)
{
   ;
}
size_t CSoarResourceProvider::getThunkSize()  
{
	return 409600;
}
bool CSoarResourceProvider::cleardataState(void)  
{
	return true ;
}
CLeeString CSoarResourceProvider::getIdentityString(void)
{
	return d_identityString ;
}
//----------------------------------------------------------------------------//

