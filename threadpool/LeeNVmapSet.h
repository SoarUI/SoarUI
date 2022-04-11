#pragma once
 /********************************************************************
     created:    2015/02/08
     created:    2015/2/8::   18:46
     filename:     E:\2015it\LeeSDK\headers\leesdk.ultity\LeeNVmapSet.h
     file path:    E:\2015it\LeeSDK\headers\leesdk.ultity
     file base:    LeeNVmapSet
     file ext:    h
     author:      nicklisir/lee
      copyright:nicklisir@gmail.com QQ327681967
     purpose:    the ultity for leesdk
 *********************************************************************/
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
#include <map>
#include <tchar.h>
#include "leestring.h"
/*storge :N --V */
template<typename Less_Name,typename Tvalue>
class CLeeNVmapSet
{
public:

	CLeeNVmapSet(void){}
	virtual ~CLeeNVmapSet(void){}
	//interfaces
public:
	void add(Less_Name & attrName,Tvalue& attrValue);
	void remove(Less_Name& attrName);
	bool isExist(Less_Name& attrName) const;
	size_t getCount(void) const;
	Tvalue getName(size_t Idx) const;
	Tvalue getValue(size_t Idx) const;
	Tvalue getValue(const CLeeString & attName) const;
	
//data field
protected:
	typedef std::map<Less_Name, Tvalue,less<Less_Name> > AttributeMap;
	AttributeMap    d_attrs;
};
template<typename Less_Name,typename Tvalue>
inline void CLeeNVmapSet::add(CLeeString & attrName,Tvalue& attrValue)
{
	if (isExist(attrName))
	{
		return ;
	}
	d_attrs[attrName] = attrValue ;
}
template<typename Less_Name,typename Tvalue>
inline void CLeeNVmapSet::remove(Less_Name& attrName)
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
template<typename Less_Name,typename Tvalue>
inline bool CLeeNVmapSet::isExist(CLeeString& attrName) const
{
	return d_attrs.find(attrName) != d_attrs.end() ;
}
template<typename Less_Name,typename Tvalue>
inline size_t CLeeNVmapSet::getCount(void) const
{
	return d_attrs.size();
}
template<typename Less_Name,typename Tvalue>
inline Tvalue CLeeNVmapSet::getName(size_t Idx) const
{
	if (Idx >= d_attrs.size())
	{
		Tvalue t;
		return t;
	}
	AttributeMap::const_iterator iter =d_attrs.begin();
	std::advance(iter,Idx);
	return (*iter).first;
}
template<typename Less_Name,typename Tvalue>
inline Tvalue CLeeNVmapSet::getValue(size_t Idx) const
{
	if (Idx >= d_attrs.size())
	{
		Tvalue t;
		return t;
	}
	AttributeMap::const_iterator iter =d_attrs.begin() ;
	std::advance(iter,Idx);
	return (*iter).second;
}
template<typename Less_Name,typename Tvalue>
inline Tvalue CLeeNVmapSet::getValue(const CLeeString & attrName) const
{
	AttributeMap::const_iterator pos = d_attrs.find(attrName);
	if (pos != d_attrs.end())
	{
		return (*pos).second;
	}
	Tvalue t;
	return t;
}

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif