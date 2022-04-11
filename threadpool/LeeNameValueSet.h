#pragma once
 /********************************************************************
     created:    2015/02/08
     created:    2015/2/8::   18:46
     filename:     E:\2015it\LeeSDK\headers\leesdk.ultity\LeeNameValueSet.h
     file path:    E:\2015it\LeeSDK\headers\leesdk.ultity
     file base:    LeeNameValueSet
     file ext:    h
     author:      nicklisir/lee
      copyright:nicklisir@gmail.com QQ327681967
     purpose:    the ultity for leesdk.xml.paser 
 *********************************************************************/
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
#include <map>
#include <tchar.h>
#include "leestring.h"
/*storge :name --value */
class CLeeNameValueSet
{
public:

	CLeeNameValueSet(void){}
	virtual ~CLeeNameValueSet(void){}
	//interfaces
public:
	void add(CLeeString & attrName,CLeeString& attrValue);
	void remove(CLeeString& attrName);
	bool isExist(CLeeString& attrName) const;
	size_t getCount(void) const;
	CLeeString getName(size_t Idx) const;
	CLeeString getValue(size_t Idx) const;
	CLeeString getValue(const CLeeString & attName) const;
	//ultity
	CLeeString getValueAsString(const CLeeString& attrName ,const CLeeString& def=_T(""));
	bool getValueAsBool(const CLeeString& attrName, bool def  = false );
	int getValueAsInteger(const CLeeString& attrName, int def  = 0 );
	float getValueAsFloat(const CLeeString& attrName, float def  = 0.0f ) const;
//data field
protected:
	typedef std::map<CLeeString, CLeeString, CLeeString::FastLessCompare> AttributeMap;
	AttributeMap    d_attrs;
};
inline void CLeeNameValueSet::add(CLeeString & attrName,CLeeString& attrValue)
{
	if (isExist(attrName))
	{
		return ;
	}
	d_attrs[attrName] = attrValue ;
}

inline void CLeeNameValueSet::remove(CLeeString& attrName)
{
	if (isExist(attrName))
	{
		AttributeMap::iterator pos = d_attrs.find(attrName);
		if (pos !=d_attrs.end())
		{
			d_attrs.erase(pos);
		}
	}
	
}
inline bool CLeeNameValueSet::isExist(CLeeString& attrName) const
{
	return d_attrs.find(attrName) != d_attrs.end() ;
}
inline size_t CLeeNameValueSet::getCount(void) const
{
	return d_attrs.size();
}
inline CLeeString CLeeNameValueSet::getName(size_t Idx) const
{
	if (Idx >= d_attrs.size())
	{
		return CLeeString(_T(""));
	}
	AttributeMap::const_iterator iter =d_attrs.begin();
	std::advance(iter,Idx);
	return (*iter).first;
}
inline CLeeString CLeeNameValueSet::getValue(size_t Idx) const
{
	if (Idx >= d_attrs.size())
	{
		return CLeeString(_T(""));
	}
	AttributeMap::const_iterator iter =d_attrs.begin() ;
	std::advance(iter,Idx);
	return (*iter).second;
}
inline CLeeString CLeeNameValueSet::getValue(const CLeeString & attrName) const
{
	AttributeMap::const_iterator pos = d_attrs.find(attrName);
	if (pos != d_attrs.end())
	{
		return (*pos).second;
	}
	return CLeeString(_T(""));
}
//ultity
inline CLeeString CLeeNameValueSet::getValueAsString(const CLeeString& attrName ,const CLeeString& def/*=_T("")*/)
{
	return(isExist(attrName)?getValue(attrName):def);
}
inline bool CLeeNameValueSet::getValueAsBool(const CLeeString& attrName, bool def /* = false*/ )
{
	if (!isExist(attrName))
	{
		return def ;
	}
	const CLeeString& val = getValue(attrName);
	if (val==_T("false") ||
		val==_T("0"))
	{
		return false;
	}
	else if (val == _T("true") ||
		     val == _T("1"))
	{
		return true ;
	}
	else
		return def ;
	
}
inline int CLeeNameValueSet::getValueAsInteger(const CLeeString& attrName, int def /* = 0 */)
{
	if (!isExist(attrName))
	{
		return def ;
	}
	int val =0;
	val = _ttoi(getValue(attrName).c_str());
	return val ;
}
inline float CLeeNameValueSet::getValueAsFloat(const CLeeString& attrName, float def /* = 0.0f */) const
{
	if (!isExist(attrName))
	{
		return def ;
	}
	float val =0.0 ;
	val =_tstof(getValue(attrName).c_str());
	return val ;
}
#if defined(_MSC_VER)
#	pragma warning(pop)
#endif