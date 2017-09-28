//----------------------------------------------------------------------
// File: AniButton.cpp
// 
// Create: 
//     2012-04-13 
//
// History:
// 
//----------------------------------------------------------------------

#pragma once

#include "Stdafx.h"
#include "AniButtonPng.h"
#include "Img.h"


BEGIN_MESSAGE_MAP(CAniButtonPng, CButton)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


CAniButtonPng::CAniButtonPng(void)
{
	m_bIsMouseMove = false;
	m_nWidth = 0;
	m_nHeight = 0;
	m_eState = NORMLE;
	m_hDCBK = NULL;
	m_hDCWhole = NULL;
	m_bmpBk = NULL;
	m_bAutoClickUp = TRUE;
	m_bClicked = FALSE;
	m_bBmpLeft = FALSE;
	m_bDisabled = FALSE;
}


CAniButtonPng::~CAniButtonPng(void)
{
	if ( m_hDCWhole )
	{
		DeleteDC( m_hDCWhole );
		m_hDCWhole = NULL;
	}

	if ( m_bmpBk )
	{
		::DeleteObject(m_bmpBk);
		m_bmpBk = NULL;
	}

	if ( m_hDCBK )
	{
		DeleteDC( m_hDCBK );
		m_hDCBK = NULL;
	}
}


// 鼠标移动消息
void CAniButtonPng::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsMouseMove || m_bDisabled)
	{
		CButton::OnMouseMove(nFlags, point);
		return;
	}

	//注册消息
	m_bIsMouseMove = true;

	TRACKMOUSEEVENT TrackMouseEvent;
	TrackMouseEvent.cbSize = sizeof(TrackMouseEvent);
	TrackMouseEvent.dwFlags = TME_LEAVE;
	TrackMouseEvent.hwndTrack = GetSafeHwnd();
	TrackMouseEvent.dwHoverTime = HOVER_DEFAULT;
	_TrackMouseEvent(&TrackMouseEvent);

	// Hover状态
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	if ( m_bAutoClickUp || m_eState!=CLICK )
	{
		m_eState = HOVER;
		Invalidate();
	}
	
	CButton::OnMouseMove(nFlags, point);
}


// 鼠标离开消息
LRESULT CAniButtonPng::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bIsMouseMove = false;

	// Normal状态
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	if ( m_bAutoClickUp || m_eState!=CLICK )
	{
		m_eState = NORMLE;
		Invalidate();
	}

	return 0;
}




// 加载位图
bool CAniButtonPng::LoadAniBitmap(HBITMAP hBitmap, 
							   HWND hWndParent, 
							   int nIDBtn,
							   bool bExpand)
{
	if (NULL == hBitmap)
		return false;

	if (NULL == hWndParent)
		return false;

	if (0 == nIDBtn)
		return false;
	
	m_hWndParent = hWndParent;
	m_nIDBtn = nIDBtn;

	HDC hDC = ::GetDC(GetSafeHwnd());
	m_hDCWhole = CreateCompatibleDC( hDC );
	::ReleaseDC( GetSafeHwnd(), hDC );

	m_hBmpWhole = hBitmap;
	SelectObject(m_hDCWhole, m_hBmpWhole);

	CBitmap* bmp = CBitmap::FromHandle(m_hBmpWhole);
	BITMAP stbmp;   
	bmp->GetBitmap(&stbmp);   
	m_nWidth = stbmp.bmWidth/5;
	m_nHeight = stbmp.bmHeight;

	if (m_nWidth > 0 && m_nHeight > 0)
	{
		SetWindowPos(NULL, 0, 0, m_nWidth, m_nHeight, SWP_NOMOVE);
	}

	Invalidate();

	if (NULL != GetSafeHwnd())
	{		
		if (!(GetButtonStyle() & WS_CLIPSIBLINGS))
			SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(GetSafeHwnd(), GWL_STYLE) | WS_CLIPSIBLINGS);
	}
	
	return true;
}

// 加载位图
bool CAniButtonPng::LoadAniBitmap(LPCTSTR szFileName, 
							   HWND hWndParent, 
							   int nIDBtn,
							   bool bExpand)
{
	if (NULL == szFileName)
		return false;

	CImg img;
	img.Load( szFileName );
	if ( img.IsNull() )
	{
		img.Destroy();
		return false;
	}

	return LoadAniBitmap( (HBITMAP)img.Detach(), hWndParent, nIDBtn, bExpand );
}


void CAniButtonPng::SetAutoClickUp( BOOL bAuto )
{
	m_bAutoClickUp = bAuto;
}

void CAniButtonPng::SetClicked( BOOL bClicked )
{
	m_bClicked = bClicked;

	if ( bClicked )
	{
		m_eState = CLICK;
	}
	else
	{
		
		m_eState = NORMLE;
	}

	Invalidate();
}

void CAniButtonPng::SetDisabled( BOOL bDisabled )
{
	if ( bDisabled )
	{
		ModifyStyle( 0, WS_DISABLED );
		m_eState = UNABLE;
	}
	else
	{
		ModifyStyle( WS_DISABLED, 0 );
		m_eState = NORMLE;
	}

	Invalidate();
}

void CAniButtonPng::SetBmpLeft( BOOL bLeft )
{
	m_bBmpLeft = bLeft;
}

void CAniButtonPng::RePaintBK()
{
	if ( m_hDCBK )
	{
		DeleteDC( m_hDCBK );
		m_hDCBK = NULL;
	}

	Invalidate();
}

void CAniButtonPng::PaintPng( CDC *pDC )
{
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	PaintBK(pDC);
	::AlphaBlend( pDC->GetSafeHdc(), 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*m_eState, 0, m_nWidth, m_nHeight, bf );
}

void CAniButtonPng::PaintBK(CDC* pDC)
{
	CClientDC parentDC(GetParent());

	if (m_hDCBK == NULL&&IsTopParentActive())//modified by felix
	{
		CRect rcBk;
		GetWindowRect(rcBk);
		GetParent()->ScreenToClient(rcBk);
		m_hDCBK = CreateCompatibleDC( parentDC.GetSafeHdc() );
		m_bmpBk = CreateCompatibleBitmap(parentDC.GetSafeHdc(),  m_nWidth, m_nHeight);
		SelectObject( m_hDCBK, m_bmpBk );
		::BitBlt(m_hDCBK, 0, 0, m_nWidth, m_nHeight, parentDC.GetSafeHdc(), rcBk.left, rcBk.top, SRCCOPY);
	} 

	pDC->BitBlt(0, 0,  m_nWidth, m_nHeight, CDC::FromHandle(m_hDCBK), 0, 0, SRCCOPY);
}

void CAniButtonPng::DrawBtnText( CDC *pDC )
{
	CRect rectClient;
	CFont* pOldFont = pDC->SelectObject(GetFont());
	CString strText;
	GetWindowText( strText );
	CSize size = pDC->GetTextExtent(strText);

	if ( m_bBmpLeft )
	{
		size.cx = size.cx > (m_nWidth-24) ? (m_nWidth-24) : size.cx;
		size.cy = size.cy > m_nHeight ? m_nHeight : size.cy;
		rectClient.left   = 24;
		rectClient.top    = (m_nHeight - size.cy)/2;
		rectClient.right  = rectClient.left + size.cx;
		rectClient.bottom = rectClient.top + size.cy;
	}
	else
	{
		size.cx = size.cx > m_nWidth ? m_nWidth : size.cx;
		size.cy = size.cy > m_nHeight ? m_nHeight : size.cy;
		rectClient.left   = (m_nWidth - size.cx)/2;
		rectClient.top    = (m_nHeight - size.cy)/2;
		rectClient.right  = rectClient.left + size.cx;
		rectClient.bottom = rectClient.top + size.cy;
	}
	
	switch(m_eState)
	{
	case CLICK:
		rectClient.top    = rectClient.top + 1;
		rectClient.left   = rectClient.left + 1;
		rectClient.right  = rectClient.left + size.cx;
		rectClient.bottom = rectClient.top + size.cy;
		break ;
	default:
		break ;
	}

	// 文本透明
	pDC->SetBkMode( TRANSPARENT );
	pDC->DrawText(strText, rectClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);

	pDC->SelectObject(pOldFont);
}

BOOL CAniButtonPng::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CAniButtonPng::OnPaint()
{
	CPaintDC dc(this);
	
	PaintPng( &dc );

	DrawBtnText( &dc );
}


void CAniButtonPng::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	// 按下状态
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;
	
	m_eState = CLICK;

	Invalidate();

	// 因为屏蔽了点击消息，所以父窗口无法处理点击事件，在此发消息给父窗口替代点击事件
	::PostMessage(m_hWndParent, MSG_TO_PARENT_CLICKED_DOWN, m_nIDBtn, NULL);
}


void CAniButtonPng::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	// Hover状态
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	CRect rcClient;
	GetClientRect( &rcClient );
	if ( !rcClient.PtInRect(point) )
	{
		m_eState = m_bClicked ? CLICK : NORMLE;
		return;
	}
	
	if ( m_bAutoClickUp || m_bClicked )
	{
		m_eState = FOCUS;

		Invalidate(); 
	}
	m_bClicked = !m_bClicked;

	// 因为屏蔽了点击消息，所以父窗口无法处理点击事件，在此发消息给父窗口替代点击事件
	::PostMessage(m_hWndParent, MSG_TO_PARENT_CLICKED_UP, m_nIDBtn, NULL);
}


void CAniButtonPng::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// 按下状态
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = AC_SRC_ALPHA;
	bf.SourceConstantAlpha = 255;

	m_eState = CLICK;

	Invalidate(); 
	// 屏蔽基类OnLButtonDblClk
	// CButton::OnLButtonDblClk(nFlags, point);
}
