#pragma once
#include "XmlProperty.h"
class Property :
	public XmlProperty
{
public:
	//Property(void);
	Property(const CLeeString& name, const CLeeString& value);
	Property(const Property& other);
	Property& operator=(const Property& other);
public:
	virtual ~Property(void);
	// abstract members from Property
	const CLeeString& get() const;
	void set(const CLeeString& value);
};
