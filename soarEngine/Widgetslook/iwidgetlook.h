#pragma once
/********************************************************************
    Descibe:      widget基类
    created:      2012/9/1 11:57
    FullName:     d:\测试项目\MYFeel\MYFeel\IWidgetLook.h
    Path:         d:\测试项目\MYFeel\MYFeel
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
	//图像集合----------------------------------------//
	//获取图像描述
	virtual ImagerySection* getImagerySection(const CLeeString& section)=0;
	virtual void addImagerySection(ImagerySection& section)=0;
	virtual void clearImagerySections(void)=0;
	virtual RectF getImagerySectionBoundingRect(const CLeeString& section,const RectF& ControlRect)=0;
	virtual bool isImagerySectionPresent(const CLeeString& section) const=0;
	//状态集合---------------------------------------.//
	//该控件是否定义了该状态
	virtual bool isStateImageryPresent(const CLeeString& state) const=0;
	virtual StateImagery* getStateImagery(const CLeeString& state)=0;
	virtual void addStateImagery( StateImagery& state)=0;
	//清除所有状态定义
	virtual void clearStateImagerys(void)=0;
	//本地坐标系下区域
	virtual RectF getBoundingRect(const RectF& ControlRect) const =0;
	//添加命名空间
	virtual void addNamedArea(const NamedArea& area)=0;
	//清除所有命名控空间
	virtual void clearNamedAreas()=0;
	//获取命名空间
	virtual const NamedArea* getNamedArea(const CLeeString& name) const=0;
	//是否已经定义了命名空间
	virtual bool isNamedAreaDefined(const CLeeString& name) const=0;

};
