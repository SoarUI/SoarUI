
#ifndef _XmlProperty_h_
#define _XmlProperty_h_
#include "../../SoarHeader/LeeString.h"
/********************************************************************
    Descibe:      属性基类，只要用于XML属性定义，连接
    created:      2012/8/27 17:13
    FullName:     d:\测试项目\MYFeel\MYFeel\UI\Feel\Propertys\XmlProperty.h
    Path:         d:\测试项目\MYFeel\MYFeel\UI\Feel\Propertys
    ClassName:    XmlProperty
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
class XmlProperty
{
public:
	XmlProperty(const CLeeString& name, 
		const CLeeString& defaultValue = _T("\0")) :
	  d_name(name),
	  d_default(defaultValue)
	{
	}
    XmlProperty(const XmlProperty& rhs);
	XmlProperty& operator=(const XmlProperty& other);
	virtual ~XmlProperty(void) {}
	const CLeeString& getName(void) const		{return d_name;}
	virtual const CLeeString&	get(void) const = 0;
	virtual void	set(const CLeeString& value) = 0;
	virtual bool	isDefault() const;
	virtual const CLeeString&	getDefault() const;
   // virtual void writeXMLToStream(const PropertyReceiver* receiver, XMLSerializer& xml_stream) const;

protected:
	CLeeString	d_name;		//属性名
	CLeeString	d_default;	//默认值
};

#endif	// end of guard _XmlProperty_h_
