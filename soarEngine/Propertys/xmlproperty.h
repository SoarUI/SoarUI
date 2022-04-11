
#ifndef _XmlProperty_h_
#define _XmlProperty_h_
#include "../../SoarHeader/LeeString.h"
/********************************************************************
    Descibe:      ���Ի��ֻ࣬Ҫ����XML���Զ��壬����
    created:      2012/8/27 17:13
    FullName:     d:\������Ŀ\MYFeel\MYFeel\UI\Feel\Propertys\XmlProperty.h
    Path:         d:\������Ŀ\MYFeel\MYFeel\UI\Feel\Propertys
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
	CLeeString	d_name;		//������
	CLeeString	d_default;	//Ĭ��ֵ
};

#endif	// end of guard _XmlProperty_h_
