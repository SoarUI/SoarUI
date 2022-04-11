#pragma once
/********************************************************************
    Descibe:      widget����
    created:      2012/9/1 11:57
    FullName:     d:\������Ŀ\MYFeel\MYFeel\IWidgetLook.h
    Path:         d:\������Ŀ\MYFeel\MYFeel
    ClassName:    IWidgetLook
    FileType:     h
    Author:       nicklisir@163.com
    NeededLib:    
    requires:      
*********************************************************************/
#include "../../SoarHeader/LeeString.h"
#include "../StateImagery.h"
#include "../ImagerySection.h"
#include "../NamedArea.h"
#include <map>
#include <Windows.h>
class IWidgetLook
{
public:
	//
	virtual const CLeeString& getName(void) const =0;
	virtual const CLeeString& getWindowType(void) const=0;
	//ͼ�񼯺�----------------------------------------//
	//��ȡͼ������
	virtual ImagerySection* getImagerySection(const CLeeString& section)=0;
	virtual void addImagerySection(ImagerySection& section)=0;
	virtual void clearImagerySections(void)=0;
	virtual RectF getImagerySectionBoundingRect(const CLeeString& section,const RectF& ControlRect)=0;
	virtual bool isImagerySectionPresent(const CLeeString& section) const=0;
	//״̬����---------------------------------------.//
	//�ÿؼ��Ƿ����˸�״̬
	virtual bool isStateImageryPresent(const CLeeString& state) const=0;
	virtual StateImagery* getStateImagery(const CLeeString& state)=0;
	virtual void addStateImagery( StateImagery& state)=0;
	//�������״̬����
	virtual void clearStateImagerys(void)=0;
	//��������ϵ������
	virtual RectF getBoundingRect(const RectF& ControlRect) const =0;
	//��������ռ�
	virtual void addNamedArea(const NamedArea& area)=0;
	//������������ؿռ�
	virtual void clearNamedAreas()=0;
	//��ȡ�����ռ�
	virtual const NamedArea* getNamedArea(const CLeeString& name) const=0;
	//�Ƿ��Ѿ������������ռ�
	virtual bool isNamedAreaDefined(const CLeeString& name) const=0;

};
