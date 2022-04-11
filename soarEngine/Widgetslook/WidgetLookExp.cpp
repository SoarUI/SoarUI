#include "WidgetLook.h"

const CLeeString& CWidgetLook::getName(void) const
{
	return d_name;
}
const CLeeString& CWidgetLook::getWindowType(void) const
{
	return d_windowType;
}
//图像集合-----------------------------------------------
ImagerySection* CWidgetLook::getImagerySection( const CLeeString& section) 
{
	ImageryList::iterator imgSect = d_imagerySections.find(section);

	if (imgSect == d_imagerySections.end())
	{
		return NULL;
	}
	return &((*imgSect).second);
}
void CWidgetLook::addImagerySection( ImagerySection& section)
{
	if (d_imagerySections.find(section.getName()) != d_imagerySections.end())
	{
		;
	}
	d_imagerySections[section.getName()] = section;
}
void CWidgetLook::clearImagerySections()
{
	d_imagerySections.clear();
}
//难点在于如何检测该look是否使用自己的区域
RectF CWidgetLook::getImagerySectionBoundingRect(const CLeeString& section,const RectF& ControlRect)
{
	ImagerySection* pSection =getImagerySection(section);
	if (pSection)
	{
		return pSection->getBoundingRect(ControlRect);
	}
	return RectF(0.0f,0.0f,0.0f,0.0f);
}


bool CWidgetLook::isImagerySectionPresent(const CLeeString& section) const
{
	return d_imagerySections.find(section) != d_imagerySections.end();
}
//状态集合-----------------------------------------------
StateImagery* CWidgetLook::getStateImagery(const CLeeString& state) 
{
	StateList::iterator imagery = d_stateImagery.find(state);

	if (imagery == d_stateImagery.end())
	{
		return NULL;
	}

	return &((*imagery).second);
}
void CWidgetLook::addStateImagery( StateImagery& state)
{
	if (d_stateImagery.find(state.getName()) != d_stateImagery.end())
	{
		throw(_T("State has been definite"));
	}

	d_stateImagery[state.getName()] = state;
}
void CWidgetLook::clearStateImagerys()
{
	d_stateImagery.clear();
}
bool CWidgetLook::isStateImageryPresent(const CLeeString& state) const
{
	return d_stateImagery.find(state) != d_stateImagery.end();
}




//命名空间---------------------------------------------------//
void CWidgetLook::addNamedArea(const NamedArea& area)
{
	if (d_namedAreas.find(area.getName()) != d_namedAreas.end())
	{
		throw(_T("NameArea has been definite"));
	}
	d_namedAreas[area.getName()] = area;
}

void CWidgetLook::clearNamedAreas()
{
	d_namedAreas.clear();
}

const NamedArea* CWidgetLook::getNamedArea(const CLeeString& name) const
{
	NamedAreaList::const_iterator area = d_namedAreas.find(name);

	if (area == d_namedAreas.end())
	{
		throw(_T("NameArea hasn't been definite"));
	}

	return &((*area).second);
}

bool CWidgetLook::isNamedAreaDefined(const CLeeString& name) const
{
	return d_namedAreas.find(name) != d_namedAreas.end();
}