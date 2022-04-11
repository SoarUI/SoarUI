#ifndef LEE_CLeePluginMgr_H__
#define LEE_CLeePluginMgr_H__
/*
���������������ڲ�����������������ļ�ע��
*/
#include "../SoarHeader/leeString.h"
#include "leeDynamicLink.h"
#include <map>
struct PluginPacket 
{
	CLeeString sName;
	GUID guID;
};
class CLeePluginMgr
{
public:
	CLeePluginMgr(void);
public:
	virtual ~CLeePluginMgr(void);
	//ͨ�����ֻ�ȡ������
	CLeeDynamicLink* getPlug(const CLeeString & name);
	void* getFunction(const CLeeString & name,LPCSTR func);
	CLeeDynamicLink* load(const CLeeString & pathname,const CLeeString & name);
	void unload(const CLeeString & name);
	//��Դ���Ƿ����
	bool isloaded(const CLeeString& plugName) const;
	void clearAllPlugins(void);
private:
	CLeePluginMgr(const CLeePluginMgr & rhs );
protected:
	//���ļ���ȡ���
	//�����-Any
	typedef std::map<CLeeString,CLeeDynamicLink*,CLeeString::FastLessCompare> DynamicLinkMap;
	DynamicLinkMap d_plugins;
};
#endif