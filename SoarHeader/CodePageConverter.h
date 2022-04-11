#pragma once
#ifndef CCodePageConverter__H_
#define CCodePageConverter__H_
class CCodePageConverter
{
public:
	CCodePageConverter(void){}
	~CCodePageConverter(void){}
	/************************************************************************/
	/* ��gb2312����תΪUTF-8�ַ�                                            */
	/************************************************************************/
	static std::string  GBToUTF8(const char*);
	/************************************************************************/
	/* ��UNICODE����תΪUTF-8�ַ�                                           */
	/************************************************************************/
	static std::string UnicodeToUTF8(const WCHAR*);
	/************************************************************************/
	/* ��UTF-8����תΪgb2312�ַ�                                           */
	/************************************************************************/
	static std::string UTF8ToGB(const char*);
	/************************************************************************/
	/* ��UTF-8����תΪUnicode�ַ�                                           */
	/************************************************************************/
	static std::wstring UTF8ToUnicode(const char*);
	/************************************************************************/
	/* ����ͨMultiByte����תΪUNICODE�ַ�                                   */
	/************************************************************************/
	static std::wstring MultibyteToUnicode(const char*);
	/************************************************************************/
	/* ��UNICODE����תΪ��ͨMultiByte�ַ�                                   */
	/************************************************************************/
	static std::string UnicodeToMultibyte(const WCHAR*);
};
inline std::string  CCodePageConverter::GBToUTF8(const char* str)
{
	std::string  result;
	WCHAR *strSrc;
	CHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_ACP, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}

inline std::string  CCodePageConverter::UTF8ToGB(const char* str)
{
	std::string  result;
	WCHAR *strSrc;
	CHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, str, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, str, -1, strSrc, i);

	//�����ʱ�����Ĵ�С
	i = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i+1];
	WideCharToMultiByte(CP_ACP, 0, strSrc, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []strSrc;
	delete []szRes;

	return result;
}
inline std::string CCodePageConverter::UnicodeToUTF8(const WCHAR* unicodestr)
{
	std::string  result;
	CHAR *szRes;

	//�����ʱ�����Ĵ�С
	int i = WideCharToMultiByte(CP_UTF8, 0, unicodestr, -1, NULL, 0, NULL, NULL);
	szRes = new CHAR[i+1];
	int j=WideCharToMultiByte(CP_UTF8, 0, unicodestr, -1, szRes, i, NULL, NULL);

	result = szRes;
	delete []szRes;

	return result;
}
inline std::wstring CCodePageConverter::UTF8ToUnicode(const char* ansistr)
{
	std::wstring  result;
	WCHAR *strSrc;

	//�����ʱ�����Ĵ�С
	int i = MultiByteToWideChar(CP_UTF8, 0, ansistr, -1, NULL, 0);
	strSrc = new WCHAR[i+1];
	MultiByteToWideChar(CP_UTF8, 0, ansistr, -1, strSrc, i);

	result = strSrc;
	delete []strSrc;

	return result;
}
inline std::wstring CCodePageConverter::MultibyteToUnicode(const char* ansistr)
{
	static std::wstring unicodestrings;
	static WCHAR tmpbuf[1024]; 
    memset(tmpbuf, 0, sizeof(tmpbuf)); 
	int ilen =::MultiByteToWideChar(CP_ACP,0,ansistr,strlen(ansistr),0,0);
	if(ilen <1024){
	::MultiByteToWideChar(CP_ACP,0,ansistr,strlen(ansistr),tmpbuf,1023);
	unicodestrings = tmpbuf;
	}
	else{
	WCHAR * p= new WCHAR[ilen];
		::MultiByteToWideChar(CP_ACP,0,ansistr,strlen(ansistr),p,ilen);
	unicodestrings = p;
	delete p;
	}
	return unicodestrings ;
}
inline std::string CCodePageConverter::UnicodeToMultibyte(const WCHAR* unicodestr)
{
	static std::string ansistr;
	static char tmpbuf[1024]; 
    memset(tmpbuf, 0, sizeof(tmpbuf));
	int ilen =WideCharToMultiByte(CP_ACP, 0, unicodestr, wcslen(unicodestr), 0, 0,0,0);
	if(ilen <1024){
	WideCharToMultiByte(CP_ACP, 0, unicodestr, wcslen(unicodestr), tmpbuf, 1023,0,0);
	ansistr=tmpbuf;
	}
	else{
		CHAR * p= new CHAR[ilen];
		WideCharToMultiByte(CP_ACP, 0, unicodestr, wcslen(unicodestr), p, ilen,0,0);
	ansistr= p;
	delete p;
	}
	return ansistr ;
}
#endif