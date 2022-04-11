#pragma once
//模拟CFile类，2014-03-05 14:29
#include"leemacro.h"
#include"leeString.h"
#include<fstream>
#include<assert.h>
using namespace std;
class CLeeFile
{
public:
	CLeeFile(void){}
	CLeeFile(CLeeString f,int dwflags){d_file.open(f.c_str(),dwflags);}
	DWORD GetLength(void);
	int Read(void* data,DWORD len);
	void Write(void* data,DWORD len);
	int ReadLine(void* data,DWORD len);
	void Flush(void);
	void Close(void);
	//ios::in -- 输入文件，读 ios::out -- 输出文件 写 ios::binary -- 二进制文件
	bool Open(CLeeString f,int dwflags);
	bool Seekg(DWORD pos,DWORD dwflags);
	//以下是写指针方法
	bool Seekp(DWORD pos,DWORD dwflags);
	void SeekToBegin(void);
	void SeekToEnd(void);
	static bool GenerateFile(CLeeString f);
public:
	~CLeeFile(void){}
private:
	TFSTREAM d_file;
};
inline DWORD CLeeFile::GetLength(void)
{
	assert(d_file.is_open());
	d_file.seekg(0,ios::end);
	return d_file.tellg();
}
inline int CLeeFile::Read(void* data,DWORD len)
{
	assert(d_file.is_open());
	d_file.read((TCHAR*)data,len);
	return d_file.gcount();
}
inline void CLeeFile::Write(void* data,DWORD len)
{
	assert(d_file.is_open());
	d_file.write((TCHAR*)data,len);
}
inline void CLeeFile::Flush(void)
{
	assert(d_file.is_open());
	d_file.flush();
}
inline void CLeeFile::Close(void)
{
	assert(d_file.is_open());
	d_file.close();
}
inline bool CLeeFile::Open(CLeeString f,int dwflags)
{
	//assert(d_file.is_open());
	//locale::global(locale(""));//将全局区域设为操作系统默认区域  
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	setlocale(LC_CTYPE, "C");
	d_file.open((char*)f.c_str(),dwflags);
	//locale::global(locale("C"));//还原全局区域设定  (locale在DLL中会出错？？？2018-10-15)
	setlocale(LC_CTYPE, old_locale); //还原语言区域的设置 
	free(old_locale);//还原区域设定
	return d_file.is_open();
}
inline bool CLeeFile::Seekg(DWORD pos,DWORD dwflags)
{
	assert(d_file.is_open());
	d_file.seekg(pos,dwflags);
	return true;
}
inline bool CLeeFile::Seekp(DWORD pos,DWORD dwflags)
{
	assert(d_file.is_open());
	d_file.seekp(pos,dwflags);
	return true;
}
inline void CLeeFile::SeekToBegin(void)
{
	assert(d_file.is_open());
	d_file.seekp(0,ios::beg);
}
inline void CLeeFile::SeekToEnd(void)
{
	assert(d_file.is_open());
	d_file.seekp(0,ios::end);
}
inline int CLeeFile::ReadLine(void* data,DWORD len)
{
	assert(d_file.is_open());
	d_file.getline((TCHAR*)data,len);
	return d_file.good();
}
inline bool CLeeFile::GenerateFile(CLeeString f)
{
	//创建目标文件
	CLeeFile targetFile;
	//创建文件
	if(!targetFile.Open(f,ios::binary|ios::out))
	{
		return -1;
	}
	targetFile.Close();
	return true;
}
