// EditWithButton.cpp : implementation file
//

#include "stdafx.h"
#include "EditWithButton.h"
#include ".\editwithbutton.h"
#include "ISkinLoader.h"
#include "UILang.h"

// CEditWithButton

IMPLEMENT_DYNAMIC(CEditWithButton, CEdit)

CEditWithButton::CEditWithButton()
{
	m_sHintText               = _T("");
	m_tchKeyPressed           = 0;

	m_nButtonClickedMessageId = WM_USER_EDITWITHBUTTON_CLICKED;
	m_nKeyDownMessageID       = WM_USER_EDITWITHBUTTON_KEYDWON;
	m_bButtonExistsAlways     = FALSE;

	m_rcEditArea.SetRect( 0, 0, 0, 0 );
	m_rcButtonArea.SetRect( 0, 0, 0, 0 );
	m_sizeState.SetSize( 0, 0 );

	m_pNotifier               = NULL;

	LOGFONT lf;
	ZeroMemory( &lf, sizeof(LOGFONT) );
	_tcscpy( lf.lfFaceName, _T("宋体") );
	lf.lfHeight = 12;
	m_fntDef.SetLogFont( lf );
	m_fntDef.SetFontColor( RGB( 10, 10, 10 ) );
}

CEditWithButton::~CEditWithButton()
{
	m_imgState.Destroy();
}

BEGIN_MESSAGE_MAP(CEditWithButton, CEdit) 
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KEYUP()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_KILLFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CEditWithButton message handlers

void CEditWithButton::PreSubclassWindow( )
{	
	ASSERT( GetStyle() & ES_MULTILINE );

	ResizeWindow();
}

BOOL CEditWithButton::PreTranslateMessage( MSG* pMsg )
{
	return CEdit::PreTranslateMessage(pMsg);
}

BOOL CEditWithButton::SetImages( CString sImagePath )
{
	m_imgState.Destroy();
	if ( ::PathFileExists( sImagePath ) && m_imgState.Load(sImagePath) )
	{
		m_sizeState.SetSize( m_imgState.GetWidth() / 2, m_imgState.GetHeight() );
		return TRUE;
	}

	return FALSE;
}

void CEditWithButton::SetButtonArea( CRect rcButtonArea )
{
	m_rcButtonArea = rcButtonArea;
}

BOOL CEditWithButton::SetEditArea(CRect rcEditArea)
{
	m_rcEditArea = rcEditArea;

	ResizeWindow();

	return TRUE;
}

void CEditWithButton::ResizeWindow()
{
	if ( !::IsWindow(m_hWnd) ) 
		return;

	if ( m_rcEditArea.IsRectEmpty() ) 
		return;

	CRect rtClient;
	GetClientRect( &rtClient );

	int nTextLen = GetWindowTextLength();
	if ( nTextLen <= 0 )
	{
		//SetWindowPos( &wndTop, 0, 0, rtClient.Width(), rtClient.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );
	}
	else
	{
		//SetWindowPos( &wndTop, 0, 0, rtClient.Width(), rtClient.Height(), SWP_SHOWWINDOW | SWP_NOZORDER );
	}

	SetRect(&m_rcEditArea);
}

BOOL CEditWithButton::IsHintText()
{
	CString sText = _T("");
	GetWindowText( sText );

	sText.Trim();

	return ( sText.CompareNoCase( m_sHintText ) == 0 );
}

void CEditWithButton::SetButtonClickedMessageId(UINT nButtonClickedMessageId)
{
	m_nButtonClickedMessageId = nButtonClickedMessageId;
}

void CEditWithButton::SetKeyDownMessageId( UINT nKeyDwonMessageId )
{
	m_nKeyDownMessageID = nKeyDwonMessageId;
}

void CEditWithButton::SetButtonExistsAlways( BOOL bButtonExistsAlways )
{
	m_bButtonExistsAlways = bButtonExistsAlways;
}

void CEditWithButton::SetHintText( CString sText, BOOL bRedraw/* = TRUE*/ )
{
	m_sHintText = sText;
	m_sHintText.Trim();

	if ( bRedraw && IsWindow( m_hWnd ) )
	{
		SetWindowText( m_sHintText );
	}
}

void CEditWithButton::SetNotifier( TEditWithButtonNotifier* pNotifier )
{
	m_pNotifier = pNotifier;
}

BOOL CEditWithButton::OnEraseBkgnd(CDC* pDC)
{
	CDC   dcMemory;
	CRect rtClient, rtCtrl;
	CBitmap bmpDC, *pOldBitmap = NULL;
	int iTextLength = GetWindowTextLength();

	GetClientRect( &rtClient );
	if ( !m_imgState.IsNull() )
	{
		dcMemory.CreateCompatibleDC( pDC );
		bmpDC.CreateCompatibleBitmap( pDC, rtClient.Width(), rtClient.Height() );
		pOldBitmap = dcMemory.SelectObject( &bmpDC );

		dcMemory.FillSolidRect( 0, 0, rtClient.Width(), rtClient.Height(), RGB( 223, 223, 223 ) );

		CPen pen( PS_SOLID, 1, CSkin::CurSkin()->GetColor( SY_IM_ID_CHAT_TOOLBTN_HOVER ) );
		CPen* pOldPen = NULL;
		pOldPen = dcMemory.SelectObject( &pen );
		rtCtrl = rtClient;
		rtCtrl.DeflateRect( 1, 0 );
		dcMemory.RoundRect( &rtCtrl, CPoint( 5, 5 ) );
		dcMemory.SelectObject( pOldPen );

		int nWidth = m_imgState.GetWidth() / 2;
		int nHeight = m_imgState.GetHeight();
		CString sText;
		GetWindowText( sText );
		if ( iTextLength <= 0 || sText.CompareNoCase( m_sHintText ) == 0 )
		{
			m_imgState.Draw( dcMemory.GetSafeHdc(), m_rcButtonArea.left, m_rcButtonArea.top, 
				m_rcButtonArea.Width(), m_rcButtonArea.Height(), 0, 0, nWidth, nHeight );
		}
		else
		{
			m_imgState.Draw( dcMemory.GetSafeHdc(), m_rcButtonArea.left, m_rcButtonArea.top, 
				m_rcButtonArea.Width(), m_rcButtonArea.Height(), nWidth, 0, nWidth, nHeight );
		}

		pDC->BitBlt( 0, 0, rtClient.Width(), rtClient.Height(), &dcMemory, 0, 0, SRCCOPY );
		dcMemory.SelectObject( pOldBitmap );
	}

	return TRUE;
}

void CEditWithButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	InvalidateRect(NULL);

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CEditWithButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_tchKeyPressed = nChar;

	SetTimer( TIMER_ID_KEY_EVENT_DELAY_NOTIFY, 200, NULL );

	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CEditWithButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if ( m_rcButtonArea.PtInRect(point) )
	{
		int nTextLen = GetWindowTextLength();
		if ( ( nTextLen > 0) || m_bButtonExistsAlways )
		{
			if ( m_pNotifier )
			{
				m_pNotifier->OnClicked( m_nButtonClickedMessageId );
			}

			CWnd *pOwner = GetOwner();
			if (pOwner)
			{
				SetWindowText( m_sHintText );
				pOwner->SendMessage( m_nButtonClickedMessageId, 0, 0 );
				pOwner->SetFocus();
			}
		}
	}

	CEdit::OnLButtonUp(nFlags, point);
}

BOOL CEditWithButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint pntCursor;
	GetCursorPos(&pntCursor);
	ScreenToClient(&pntCursor);

	if ( !m_rcEditArea.PtInRect(pntCursor) )
	{
		SetCursor( AfxGetApp()->LoadStandardCursor( MAKEINTRESOURCE(IDC_ARROW) ) );
		return TRUE;
	}

	return CEdit::OnSetCursor(pWnd, nHitTest, message);
}

int CEditWithButton::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetFont( &m_fntDef );
	SetWindowText( m_sHintText );

	ResizeWindow();

	return 0;
}


LRESULT CEditWithButton::OnSetFont( WPARAM wParam, LPARAM lParam )
{
	DefWindowProc( WM_SETFONT, wParam, lParam );

	ResizeWindow();

	return 0;
}

void CEditWithButton::OnSize( UINT nType, int cx, int cy ) 
{
	CEdit::OnSize( nType, cx, cy );

	if ( !m_imgState.IsNull() )
	{
		int nWidth = m_imgState.GetWidth() / 2;
		int nHeight = m_imgState.GetHeight();

		int nBorder = ( cy - nHeight ) / 2;
		nBorder = ( nBorder <= 0 ) ? 0 : nBorder;
		m_rcButtonArea.left   = cx - nWidth - nBorder;
		m_rcButtonArea.top    = nBorder;
		m_rcButtonArea.right  = m_rcButtonArea.left + nWidth;
		m_rcButtonArea.bottom = m_rcButtonArea.top + nHeight;

		m_rcEditArea.left   = 5;
		m_rcEditArea.top    = ( cy - m_fntDef.GetHeight() ) / 2;
		m_rcEditArea.right  = cx - m_rcButtonArea.Width() - nBorder;
		m_rcEditArea.bottom = m_rcEditArea.top + m_fntDef.GetHeight();
	}

	ResizeWindow();
}

void CEditWithButton::OnTextCut()
{
	Cut();
	InvalidateRect( NULL );

	CWnd *pOwner = GetOwner();
	if ( pOwner )
	{
		pOwner->SendMessage( m_nKeyDownMessageID, 0, 0 );
	}
}

void CEditWithButton::OnTextCopy()
{
	Copy();
}

void CEditWithButton::OnTextPaste()
{
	Paste();
}

void CEditWithButton::OnTextSelAll()
{
	SetSel( 0, -1 );
}

HBRUSH CEditWithButton::CtlColor( CDC* pDC, UINT id )
{
	CBrush brush;
	brush.CreateStockObject( WHITE_BRUSH );

	CString sText = _T("");
	GetWindowText( sText );
	if ( sText.CompareNoCase( m_sHintText ) == 0 )
	{
		pDC->SetTextColor( RGB( 128, 128, 128 ) );
	}
	else
	{
		pDC->SetTextColor( m_fntDef.GetFontColor() );
	}

	return (HBRUSH)brush.Detach();
}


void CEditWithButton::OnSetFocus(CWnd* pOldWnd)
{
	CEdit::OnSetFocus(pOldWnd);

	// TODO: 在此处添加消息处理程序代码

	if ( IsHintText() )
	{
		SetWindowText( _T("") );
	}
	else
	{
		SetSel( 0, -1 );		
	}
}

void CEditWithButton::OnContextMenu(CWnd* pWnd, CPoint point )
{
	// TODO: 在此处添加消息处理程序代码

	int nStartChar = 0, nEndChar = 0;
	GetSel( nStartChar, nEndChar );

	BOOL bCopy = ( nStartChar != nEndChar );
	BOOL bPaste = FALSE;

	CMenu menu;
	menu.CreatePopupMenu();

	menu.AppendMenu( MF_STRING | ( bCopy ? 0 : ( MF_DISABLED | MF_GRAYED ) ), ID_CONTEXT_MENU_ITEM_CUT, CUILangWindowControl::GetSearchEditCut() );
	menu.AppendMenu( MF_STRING | ( bCopy ? 0 : ( MF_DISABLED | MF_GRAYED ) ), ID_CONTEXT_MENU_ITEM_COPY, CUILangWindowControl::GetSearchEditCopy() );
	menu.AppendMenu( MF_STRING | ( bPaste ? 0 : ( MF_DISABLED | MF_GRAYED ) ), ID_CONTEXT_MENU_ITEM_PASTE, CUILangWindowControl::GetSearchEditPaste() );
	menu.AppendMenu( MF_STRING, ID_CONTEXT_MENU_ITEM_SELALL, CUILangWindowControl::GetSearchEditSelAll() );

	BOOL bRet = ::TrackPopupMenuEx( menu.GetSafeHmenu(), TPM_LEFTALIGN | TPM_TOPALIGN | TPM_VERTICAL | TPM_RETURNCMD, point.x, point.y, m_hWnd, NULL  );
	if ( bRet > 0 )
	{
		switch ( bRet )
		{
		case ID_CONTEXT_MENU_ITEM_CUT:
			OnTextCut();
			break;
		case ID_CONTEXT_MENU_ITEM_COPY:
			OnTextCopy();
			break;
		case ID_CONTEXT_MENU_ITEM_PASTE:
			OnTextPaste();
			break;
		case ID_CONTEXT_MENU_ITEM_SELALL:
			OnTextSelAll();
			break;
		}
	}

	menu.DestroyMenu();
}

void CEditWithButton::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	// TODO: 在此处添加消息处理程序代码

	CString sText = _T("");
	GetWindowText( sText );
	sText.Trim();

	if ( sText.IsEmpty() )
	{
		SetWindowText( m_sHintText );
	}
}

void CEditWithButton::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	switch ( nIDEvent )
	{
	case TIMER_ID_KEY_EVENT_DELAY_NOTIFY:
		{
			if ( m_pNotifier )
			{
				m_pNotifier->OnKeyDown( m_nKeyDownMessageID, m_tchKeyPressed );
			}

			CWnd *pOwner = GetOwner();
			if ( pOwner )
			{
				pOwner->SendMessage( m_nKeyDownMessageID, 0, m_tchKeyPressed );
			}
		}
		KillTimer( nIDEvent );
		break;
	}

	CEdit::OnTimer(nIDEvent);
}
