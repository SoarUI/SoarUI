
#ifndef _XMLAttributes_h_
#define _XMLAttributes_h_
#include "../SoarHeader/LeeString.h"
#include <map>
#include <sstream>
#include <iterator>
#include <tchar.h>
#if defined(_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable : 4251)
#endif
    class XMLAttributes
    {
    public:
		XMLAttributes(void){}
		virtual ~XMLAttributes(void){}
		void add(const CLeeString& attrName, const CLeeString& attrValue);
		void remove(const CLeeString& attrName);
		bool exists(const CLeeString& attrName) const;
		size_t getCount(void) const;
		const CLeeString getName(size_t index) const;
		const CLeeString getValue(size_t index) const;
		const CLeeString getValue(const CLeeString& attrName) const;
		const CLeeString getValueAsString(const CLeeString& attrName, const CLeeString& def = _T("")) const;
		bool getValueAsBool(const CLeeString& attrName, bool def = false) const;
		int getValueAsInteger(const CLeeString& attrName, int def = 0) const;
		float getValueAsFloat(const CLeeString& attrName, float def = 0.0f) const;

    protected:
        typedef std::map<CLeeString, CLeeString, CLeeString::FastLessCompare> AttributeMap;
        AttributeMap    d_attrs;
    };

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif
	inline void XMLAttributes::add(const CLeeString& attrName, const CLeeString& attrValue)
	{
		d_attrs[attrName] = attrValue;
	}

	inline void XMLAttributes::remove(const CLeeString& attrName)
	{
		AttributeMap::iterator pos = d_attrs.find(attrName);

		if (pos != d_attrs.end())
			d_attrs.erase(pos);
	}

	inline bool XMLAttributes::exists(const CLeeString& attrName) const
	{
		return d_attrs.find(attrName) != d_attrs.end();
	}

	inline size_t XMLAttributes::getCount(void) const
	{
		return d_attrs.size();
	}

	inline const CLeeString XMLAttributes::getName(size_t index) const
	{
		if (index >= d_attrs.size())
		{
			return CLeeString(_T(""));
		}

		AttributeMap::const_iterator iter = d_attrs.begin();
		std::advance(iter, index);

		return (*iter).first;
	}

	inline const CLeeString XMLAttributes::getValue(size_t index) const
	{
		if (index >= d_attrs.size())
		{
			return CLeeString(_T(""));
		}

		AttributeMap::const_iterator iter = d_attrs.begin();
		std::advance(iter, index);

		return (*iter).second;
	}

	inline const CLeeString XMLAttributes::getValue(const CLeeString& attrName) const
	{
		AttributeMap::const_iterator pos = d_attrs.find(attrName);

		if (pos != d_attrs.end())
		{
			return (*pos).second;
		}
		else
		{
			return CLeeString(_T(""));
		}
	}

	inline const CLeeString XMLAttributes::getValueAsString(const CLeeString& attrName, const CLeeString& def)const
	{
		return (exists(attrName)) ? getValue(attrName) : def;
	}


	inline bool XMLAttributes::getValueAsBool(const CLeeString& attrName, bool def) const
	{
		if (!exists(attrName))
		{
			return def;
		}

		const CLeeString& val = getValue(attrName);

		if (val == _T("false") || val == _T("0"))
		{
			return false;
		}
		else if (val == _T("true") || val == _T("1"))
		{
			return true;
		}
		else
		{
			return def;
		}
	}

	inline int XMLAttributes::getValueAsInteger(const CLeeString& attrName, int def) const
	{
		if (!exists(attrName))
		{
			return def;
		}

		int val;
		val =_ttoi(getValue(attrName).c_str());
		return val;
	}

	inline float XMLAttributes::getValueAsFloat(const CLeeString& attrName, float def) const
	{
		if (!exists(attrName))
		{
			return def;
		}

		float val;
		val =_tstof(getValue(attrName).c_str());

		return val;
	}

#endif  // end of guard _XMLAttributes_h_
