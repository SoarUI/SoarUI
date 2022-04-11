#pragma once
/********************************************************************
    Descibe:      ��¼���ü���
    created:      2012/8/15 9:46
    FullName:     MYFeel\MYFeel\objectReferentBase.h
    Path:         MYFeel\MYFeel
    ClassName:    objectReferentBase
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
class objectReferentBase
{
public:
	objectReferentBase(void);
public:
	virtual ~objectReferentBase(void);
	void addRef(void);
	void release(void);
	size_t getRef(void)const;
private:
	size_t d_cref;
};
