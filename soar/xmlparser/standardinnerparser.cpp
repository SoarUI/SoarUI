#include "StandardinnerParser.h"
#include<conio.h>
#include "../../SoarHeader/leelog.h"
#include "../../XML\XMLAttributes.h"
CInnerParser::CInnerParser(void)
{
}

CInnerParser::~CInnerParser(void)
{
}
void CInnerParser::fireEndElement(IXMLHandler * handler,  TCHAR* element,  NameValuePair& attr)
{
	handler->elementEnd(element);
}
void CInnerParser::firestartElement(IXMLHandler * handler,  TCHAR* element, NameValuePair& attr)
{
	XMLAttributes attrs;
	NameValuePair::const_iterator it =attr.begin();
	for( ; it !=attr.end() ;  ++it)
	{
		attrs.add(it->first, it->second);
	}
	handler->elementStart(element, attrs);
}
void CInnerParser::fireTextElement(IXMLHandler * handler,  TCHAR* element , TCHAR* text)
{
  handler->text(text);
}
bool CInnerParser::Parse(IXMLHandler * handler,BYTE* DataPtr,size_t size)// name="value"
{
	NameValuePair Val_nameMap;
	TCHAR ElementName[128]={0,};
	TCHAR *MemData=new TCHAR[size+2];
	TCHAR *DataHead =MemData;
	//
	memcpy(MemData, DataPtr, size*sizeof(BYTE));
	MemData[size] = '\n';
	MemData[size+1] = 0;
	TCHAR elementTXT [128] ={0,};
	int iPos = 0;
	TCHAR *elementST =NULL;
	TCHAR *elementST1 =NULL;
	TCHAR *elementEN =NULL;
	TCHAR *elementLEN =NULL;
	TCHAR *elementST_EN =NULL;
	int iKey = 0;
	while(MemData!=NULL&&MemData[0]!=_T('\0'))
	{
		//������һ������
		ZeroMemory(ElementName,sizeof(ElementName));
		Val_nameMap.clear();
		//����<??>
		elementST =_tcsstr(MemData,_T("?>"));
		if (elementST)
		{
			MemData=elementST+2;
		}
		//����<%s xx=yy name="%s" >xxx</yyy>
		//����<����start�¼�
		elementST =_tcsstr(MemData,_T("<"));
		elementST_EN =_tcsstr(MemData,_T(">"));
		TCHAR * eleSpace=_tcsstr(MemData,_T(" "));
		elementEN =_tcsstr(MemData,_T("/>"));
		elementLEN=_tcsstr(MemData,_T("</"));
		
		if (elementST)
		{
			if (!elementST_EN)
			{
				return false;
			}
			//��<!-- >ע�ͽ��й���
			TCHAR* commST =_tcsstr(MemData,_T("<!--"));
			TCHAR* commEND = _tcsstr(MemData,_T("-->"));
			if (commST &&
				commEND &&
				MemData>=commST)
			{
				MemData=commEND+lstrlen(_T("-->"));
			}
			//����</�� />����end�¼�
			//��</ XXX>����
			if (elementST==elementLEN)
			{
				ParseTagEnd(ElementName,&MemData);
				//Dbgc_Function_DumpX(_T("dump"),_T("element:%s line:%d"),Dbgc_Param2(ElementName,Dbgc_LINE));
				fireEndElement(handler,ElementName,Val_nameMap);
				continue;
			}
			
			//<elementstart  name=value   ...../><elementstart  name=value   .....><elementstart>
			if(ParseTagStart(ElementName,&MemData,_T("<"),_T(" "),elementST_EN))
			{
				ElementName[127]=_T('\0');
				//��һ���ڽ�������-ֵ���з���
				AnalysisLineText(&MemData,elementST_EN,Val_nameMap);
				firestartElement(handler,ElementName,Val_nameMap);
				//����<����text�¼�
				elementST1 =_tcsstr(MemData,_T("<"));
				if (elementST1-elementST_EN>1&& elementEN==NULL)
				{
					if(ParsetxtElement(elementTXT,&MemData,_T(">"),_T("</")))
						fireTextElement(handler,ElementName,elementTXT);
				}
			}
			//��< XXX xxx xxxx />����
			if (elementEN<elementST_EN)
			{
				fireEndElement(handler,ElementName,Val_nameMap);
				iPos +=elementEN-elementST_EN;
				MemData =elementEN+2;
				continue;
			}
			else//<elementstart  name=value   .....>
			{
				iPos +=elementST_EN-elementST;
				MemData =elementST_EN+1;
			}
			
		}
		else
		{
			MemData++;
		}
	}
	delete[] DataHead;
	return true;
}
//-------------------------------------
bool CInnerParser::ParseTagStart(TCHAR* element,TCHAR** ppDataBuffer,TCHAR* lspliter,TCHAR* rspliter,TCHAR* pEND)
{
	TCHAR* DataBuffer=* ppDataBuffer;
	TCHAR *elementST =NULL;
	TCHAR *elementST_EN =NULL;
	size_t ilenST =0,ilenEn=0;
	if (lspliter)
	{
		elementST =_tcsstr(DataBuffer,lspliter);
		ilenST=lstrlen(lspliter);
	}
	if(!elementST)
	{
		elementST =DataBuffer;
	}
	elementST_EN =_tcsstr(elementST+ilenST,rspliter);
	ilenEn=lstrlen(rspliter);
	/*����Ҳ���������־����ô">"����"/>"��Ϊ����
	��������־����" "ʱ����Ϊ" "
	*/
	if (!elementST_EN||elementST_EN>pEND)
	{
		TCHAR* End=_tcsstr(elementST+ilenST,_T("/>"));
		if (End && End<=pEND)
		{
			elementST_EN =End;;
			ilenEn=lstrlen(_T("/>"));
		} 
		else
		{
			elementST_EN=_tcsstr(elementST+ilenST,_T(">"));
			ilenEn=lstrlen(_T(">"));
		}
	}
	if (elementST_EN &&elementST_EN <= pEND)
	{
		if ( elementST_EN >elementST && elementST)
		{
			_tcsncpy(element,elementST+ilenST,(elementST_EN-ilenEn)-(elementST+ilenST)+1);
			element[(elementST_EN-ilenEn)-(elementST+ilenST)+1]=_T('\0');
			//����ǰ�ߵĿո�
			TrimLeftSpace(element,lstrlen(element));
			//advance the Datasource
			*ppDataBuffer=elementST_EN+ilenEn;
			return true;
		}
	}
	return false ;
}
void CInnerParser::ParseTagEnd(TCHAR* element,TCHAR** ppDataBuffer)
{
	if (!element||!ppDataBuffer)
	{
		return;
	}
	TCHAR* DataBuffer=* ppDataBuffer;
	TCHAR *elementST =NULL;
	TCHAR *elementST_EN =NULL;
	size_t ilenST =0,ilenEn=0;
	elementST =_tcsstr(DataBuffer,_T("</"));
	ilenST=lstrlen(_T("</"));
	elementST_EN =_tcsstr(elementST+ilenST,_T(">"));
	ilenEn=lstrlen(_T(">"));
	/*����Ҳ���������־����ô">"����"/>"��Ϊ����
	��������־����" "ʱ����Ϊ" "
	*/
	if (elementST_EN &&elementST_EN>elementST)
	{
		if ( elementST_EN >elementST && elementST)
		{
			_tcsncpy(element,elementST+ilenST,(elementST_EN-ilenEn)-(elementST+ilenST)+1);
			element[(elementST_EN-ilenEn)-(elementST+ilenST)+1]=_T('\0');
			//����ǰ�ߵĿո�
			TrimLeftSpace(element,lstrlen(element));
			TrimRightSpace(element,lstrlen(element));
			//advance the Datasource
			*ppDataBuffer=elementST_EN+ilenEn;
			
			return ;
		}
	}
	return ;
}
//��������-ֵ�ԣ���ʽname="xxx" name2="xxx"  �� name=xxx name2=xxx
bool CInnerParser::ParseNameValuePair(TCHAR* elementName,TCHAR* elementValue,TCHAR** ppDataBuffer,
						TCHAR* equalSpliter,TCHAR* lSpliter,TCHAR* rlSpliter,TCHAR* rrSpliter,TCHAR* pEND)
{
	if (!ppDataBuffer ||!(*ppDataBuffer))
	{
		return false;
	}
	TCHAR* DataBuffer=* ppDataBuffer;
	TCHAR *elementL =NULL;
	TCHAR *elementEqual =NULL;
	TCHAR *elementrlSpliter =NULL;
	TCHAR *elementrRSpliter =NULL;
	size_t ilenrST =0,ilenrEn=0,ilenlST=0,ilenEqual=0;
	if (equalSpliter)
	{
		elementEqual=_tcsstr(DataBuffer,equalSpliter);
		ilenEqual =lstrlen(equalSpliter);
	}
	if (!elementEqual||elementEqual>=pEND)
	{
		//����Ŀ�꣬�����������,��־Ϊ">"
		elementL= _tcsstr(DataBuffer,_T(">"));
		if (elementL && elementL<=pEND)
		{
			*ppDataBuffer=elementL+1;
		}
		else
		{
			*ppDataBuffer=pEND+1;
		}
		return false;
	}
	if (lSpliter)
	{
		elementL =_tcsstr(DataBuffer,lSpliter);
		ilenlST =lstrlen(lSpliter);
	}
	else
	{
		elementL =DataBuffer;
	}
	if (rlSpliter)
	{
		elementrlSpliter =_tcsstr(elementL+ilenlST,rlSpliter);
		ilenrST=lstrlen(rlSpliter);
	}
	else
	{
      elementrlSpliter =elementL+ilenlST;
	}
	if (rrSpliter)
	{
		elementrRSpliter =_tcsstr(elementrlSpliter+ilenrST,rrSpliter);
		ilenrEn=lstrlen(rrSpliter);
	}
	else//��" "��Ϊ�������ߴﵽ������־
	{
       elementrRSpliter =_tcsstr(elementrlSpliter+ilenrST,_T(" "));
	   ilenrEn=lstrlen(_T(" "));
	   if (!elementrRSpliter)
	   {
		   TCHAR* End=_tcsstr(DataBuffer,_T("/>"));
		   if (End)
		   {
			   elementrRSpliter =End;;
			   ilenrEn=lstrlen(_T("/>"));
		   } 
		   else
		   {
			   elementrRSpliter=_tcsstr(DataBuffer,_T(">"));
			   ilenrEn=lstrlen(_T(">"));
		   }
	   }
	}
	if (elementEqual && 
		elementrRSpliter>elementEqual)
	{
		//name
		_tcsncpy(elementName,elementL+ilenlST,(elementEqual-ilenEqual)-(elementL+ilenlST)+1);
		//value
		if (rlSpliter)//���Ҷ��б�־
		{
			_tcsncpy(elementValue,elementrlSpliter+ilenrST,(elementrRSpliter-ilenrEn)-(elementrlSpliter+ilenrST)+1);
		} 
		else
		{
			_tcsncpy(elementValue,elementEqual+ilenEqual,(elementrRSpliter-ilenrEn)-(elementEqual+ilenEqual)+1);
		}
		//advance the Datasource
		if (rlSpliter)
		{
			*ppDataBuffer=elementrRSpliter+ilenrEn;
		}
		else
		{

			*ppDataBuffer=elementrRSpliter;
		}
		return true;
	}
	return false;
}
bool CInnerParser::ParsetxtElement(TCHAR* element,TCHAR** ppDataBuffer,TCHAR* lspliter,TCHAR* rSpliter)
{
	TCHAR* DataBuffer=* ppDataBuffer;
	TCHAR *elementST =NULL;
	TCHAR *elementST_EN =NULL;
	size_t ilenST =0,ilenEn=0;
	if (lspliter)
	{
		elementST =_tcsstr(DataBuffer,lspliter);
		ilenST=lstrlen(lspliter);
	}
	else
	{
		elementST =DataBuffer;
	}
	elementST_EN =_tcsstr(DataBuffer,rSpliter);
	ilenEn=lstrlen(rSpliter);
	if ( elementST_EN >elementST)
	{
		_tcsncpy(element,elementST+ilenST,(elementST_EN-ilenEn)-(elementST+ilenST)+1);
		//advance the Datasource
		*ppDataBuffer=elementST_EN+ilenEn;
		return true;
	}
	return false;
}
//���Ȳ���<,������>���������ڸ����ķ�Χ��
void CInnerParser::AnalysisLineText(TCHAR** ppDataBuffer,TCHAR* pEND,NameValuePair &Val_nameMap)
{
	if (!ppDataBuffer ||
		!pEND)
	{
		return ;
	}
	TCHAR* DataBuffer=* ppDataBuffer;
	TCHAR Key[128]={0,};
	TCHAR Value[128] ={0,};
	//TCHAR* elementST =_tcsstr(DataBuffer,_T(" "));
	do
	{
		ZeroMemory(Key,sizeof(Key));
		ZeroMemory(Value,sizeof(Value));
		if(ParseNameValuePair(Key,Value,&DataBuffer,_T("="),NULL,_T("\""),_T("\""),pEND))
		{//��β�����/0
			Key[127]=_T('\0');
			Value[127]=_T('\0');
			//����ǰ�ߵĿո�
			TrimLeftSpace(Key,lstrlen(Key));
			//����ǰ�ߵĿո�
			TrimLeftSpace(Value,lstrlen(Value));
		//	Dbgc_Function_DumpX(_T("dump"),
		//		_T("---> name :%s,value:%s"),
		//		Dbgc_Param2(Key,Value) );
			Val_nameMap.insert(std::make_pair(Key,Value));
		}
		//elementST=_tcsstr(DataBuffer,_T(" "));
	}while ( DataBuffer<pEND);
}
void CInnerParser::TrimLeftSpace(TCHAR* element,size_t count)
{
	if (!element||count<=1)
	{
		return ;
	}
	TCHAR* DataBuffer= element;
	TCHAR* DataTmp =new TCHAR[count+1];
	CopyMemory(DataTmp,element,sizeof(TCHAR)*count);
	DataTmp[count]=_T('\0');
	TCHAR* DataHead=DataTmp;
	while(*DataTmp++==_T(' '))
	{
		;
	}
	DataTmp--;
	ZeroMemory(element,lstrlen(element)*sizeof(TCHAR));
	memmove(element,DataTmp,lstrlen(DataTmp)*sizeof(TCHAR));
	delete[] DataHead;
}
void CInnerParser::TrimRightSpace(TCHAR* element,size_t count)
{
	if (!element||count<=1)
	{
		return ;
	}
	TCHAR* DataBuffer= element;
	TCHAR* DataTmp =new TCHAR[count+1];
	CopyMemory(DataTmp,element,sizeof(TCHAR)*count);
	DataTmp[count]=_T('\0');
	TCHAR* DataHead=DataTmp;
	while(*DataTmp++!=_T('\0'))
	{
		;
	}
	DataTmp--;
	while(*DataTmp--==_T(' '))
	{
		*DataTmp=_T('\0');
	}
	DataTmp++;
	ZeroMemory(element,lstrlen(element)*sizeof(TCHAR));
	memmove(element,DataHead,lstrlen(DataHead)*sizeof(TCHAR));
	delete[] DataHead;
}