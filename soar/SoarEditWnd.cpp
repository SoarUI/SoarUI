#include "SoarEditWnd.h"
#include"../SoarHeader/CodePageConverter.h"
#include "../SoarHeader/leeLog.h"
#include"SoarRoot.h"
CLeeEditWnd::CLeeEditWnd(HWND root,ISoarEngine* Eng):CSoarWnd(root,Eng),d_isPwd(false)
{
	d_wndlook ="Editbox";
	d_wndtype=LWNDT_EDIT;
	d_wndlookState="Normal";//皮肤状态
}

CLeeEditWnd::~CLeeEditWnd(void)
{
}
void CLeeEditWnd::DrawSelf(ILeeDrawInterface *DrawFuns) 
{
	if (d_parent !=NULL && d_parent->getState()<= LWNDST_HIDE)
	{
		return;
	}
	if (getState()<= LWNDST_HIDE)
	{
		return;
	}
	RECT rc =getWindowRect();
	RectF rcc=rc;
	RectF rcparent =getCliperRect();
	if (d_DrawEng->hasWidgetlook(d_wndlook))
	{
		if(d_isPwd)
		{
				d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_maskstring,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
		}
		else
		{
			d_DrawEng->DrawWidgetlook(d_wndlook,d_wndlookState,rcc,d_wndText,d_VertTextFormat,d_horzTextFormat,&rcparent,true);
		}
	}
}
LRESULT CLeeEditWnd::HandleEvent( UINT uMsg ,WPARAM wp ,LPARAM lp) 
{
	/**解决汉字核数字地办法时将它们都作为2个字节存放，因为一个汉字占2个字节，这样删以哥汉字就是删除一个unicode字符
	*/
	switch( uMsg )
	{
		// Make sure that while editing, the keyup and keydown messages associated with 
		// WM_CHAR messages don't go to any non-focused controls or cameras
	case WM_KEYUP:
	case WM_KEYDOWN:
		return true;

	case WM_CHAR:
		{
			switch( ( WCHAR )wp )
			{
				// Backspace
			case VK_BACK:
				{
					//// If there's a selection, treat this
					//// like a delete key.
					//if( m_nCaret != m_nSelStart )
					//{
					//	DeleteSelectionText();
					//	m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					//}
					//else if( m_nCaret > 0 )
					//{
					//	// Move the caret, then delete the char.
					//	PlaceCaret( m_nCaret - 1 );
					//	m_nSelStart = m_nCaret;
					//	m_Buffer.RemoveChar( m_nCaret );
					//	m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					//}
					//ResetCaretBlink();
					WCHAR ascii_code = wp;
					d_unicodestrings= d_unicodestrings.substr(0,d_unicodestrings.length()-1);
					d_maskstring =d_maskstring.substr(0,d_maskstring.length()-1);
					if(::IsWindowUnicode(d_rootWnd))
					{
						d_wndText = (LPCTSTR)d_unicodestrings.c_str() ;
					}
					else
					{
						d_wndText=CCodePageConverter::UnicodeToMultibyte(d_unicodestrings.c_str());
					}
					d_maskstring.clear();
					//d_maskstring = _T("*");
					for(int i =0;i<d_unicodestrings.length();i++)
					{
						d_maskstring.Format("%s*",d_maskstring.c_str());
					}
					return true;
					break;
				}

			case 24:        // Ctrl-X Cut
			case VK_CANCEL: // Ctrl-C Copy
				{
					//CopyToClipboard();

					// If the key is Ctrl-X, delete the selection too.
					if( ( WCHAR )wp == 24 )
					{
					//	DeleteSelectionText();
					//	m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					}

					break;
				}

				// Ctrl-V Paste
			case 22:
				{
					//PasteFromClipboard();
					//m_pDialog->SendEvent( EVENT_EDITBOX_CHANGE, true, this );
					break;
				}

				// Ctrl-A Select All
			case 1:
				/*if( m_nSelStart == m_nCaret )
				{
					m_nSelStart = 0;
					PlaceCaret( m_Buffer.GetTextSize() );
				}*/
				break;

			case VK_RETURN:
				// Invoke the callback when the user presses Enter.
			//	m_pDialog->SendEvent( EVENT_EDITBOX_STRING, true, this );
				break;
			
			default:
				{
					;
				}
			}
			d_wndText.Format("%s%c",d_wndText.c_str(),wp);
			//将收到的char转化为unicode
			if(::IsWindowUnicode(d_rootWnd))
		    {
				d_unicodestrings = (LPCWSTR)d_wndText.c_str();
			}
			else
			{
				d_unicodestrings =CCodePageConverter::MultibyteToUnicode( d_wndText.c_str() );
			}
			d_maskstring.clear();
			//d_maskstring = _T("*");
			for(int i =0;i<d_unicodestrings.length();i++)
			{
				d_maskstring.Format("%s*",d_maskstring.c_str());
			}
			return true;
		}
		//case WM_SETCURSOR:
			//case  WM_SETFOCUS:
			case SOAR_SETFOCUS:
				{
					RECT rc =getWindowRect();
					POINT pt={rc.left,rc.top};
					::ClientToScreen(d_rootWnd,&pt);
					HCURSOR h=LoadCursor(NULL,IDC_IBEAM);
					///SetCursorPos(pt.x+d_unicodestrings.length()*1,pt.y);
					//SetCursor(h);
					return TRUE;
				}
	}//switch
	
	return  CSoarRoot::getSingletonPtr()->SoarDefWndProc(uMsg,wp,lp);//留系统底层处理
}
void CLeeEditWnd::isPassword(bool bpwd)
{
	d_isPwd =bpwd;
}
//勿传UTF8字符：有3字节的
int CLeeEditWnd::IsGBK(LPTSTR str)
{
	unsigned char sqChar[20];  
	sqChar[0]=*str;  
	if (sqChar[0]>=0xa1) //汉字开头 汉字形式UNICODE:0xA1(0xF7)0xA1(0xFE)        
			return 2;//汉字  
	else  
		return 1;//英文、数字、英文标点  
}