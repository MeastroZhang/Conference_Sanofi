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
#include "AniButton.h"
#include <atlimage.h>


BEGIN_MESSAGE_MAP(CAniButton, CButton)
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_MOUSEMOVE()
    ON_WM_TIMER()
    ON_WM_PAINT()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


CAniButton::CAniButton()
{
    m_bIsMouseMove = false;
    m_nWidth = 0;
    m_nHeight = 0;
}


CAniButton::~CAniButton()
{
}


// 鼠标移动消息
void CAniButton::OnMouseMove(UINT nFlags, CPoint point)
{
    if (m_bIsMouseMove)
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
    ::BitBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*4, 0, SRCCOPY);

    m_bf.SourceConstantAlpha = 0;

    SetTimer(m_nIDEvent, TIMER_ELAPSE, NULL); 

    CButton::OnMouseMove(nFlags, point);
}


// 鼠标离开消息
LRESULT CAniButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
    m_bIsMouseMove = false;

    // Normal状态
    ::BitBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, 0, 0, SRCCOPY);

    m_bf.SourceConstantAlpha = 0; 

    return 0;
}


// 裁剪透明区域(暂只对四个角裁剪)
void CAniButton::MakeWindowRgn(COLORREF crTransColor)
{
    // 裁剪透明区域
    CRect rc;
    GetClientRect(&rc);

    CRgn rgnClient;
    rgnClient.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

    // 要剪掉的区域
    CRgn rgnDec;  
    rgnDec.CreateRectRgn(0, 0, 1, 1);  // 创建一个像素的矩形区域

    int iRow, iCol;
    int nWidth, nHeight;
    nWidth = m_nWidth;
    nHeight = m_nHeight;

    // top-left
    for (iRow = 0; iRow < 2; iRow++)
    {
        for (iCol = 0; iCol < 2; iCol++)
        {
            if (::GetPixel(m_hDCWhole, iCol, iRow) == crTransColor)
            {
                int iX = iCol;
                int iY = iRow;
                rgnDec.SetRectRgn(iX, iY, iX+1, iY+1);
                rgnClient.CombineRgn(&rgnClient, &rgnDec, RGN_XOR);	// 裁减	
            }
        }
    }

    // top-right
    for (iRow = 0; iRow < 2; iRow++)
    {
        for (iCol = nWidth-2; iCol < nWidth; iCol++)
        {
            if (::GetPixel(m_hDCWhole, iCol, iRow) == crTransColor)
            {
                int iX = rc.right-rc.left-(nWidth-iCol);
                int iY = iRow;
                rgnDec.SetRectRgn(iX, iY, iX+1, iY+1);
                rgnClient.CombineRgn(&rgnClient, &rgnDec, RGN_XOR);	
            }
        }
    }

    // bottom-left
    for (iRow = nHeight-2; iRow < nHeight; iRow++)
    {
        for (iCol = 0; iCol < 2; iCol++)
        {
            if (::GetPixel(m_hDCWhole, iCol, iRow) == crTransColor)
            {
                int iX = iCol;
                int iY = rc.bottom-rc.top-(nHeight-iRow);
                rgnDec.SetRectRgn(iX, iY, iX+1, iY+1);
                rgnClient.CombineRgn(&rgnClient, &rgnDec, RGN_XOR);	
            }
        }
    }

    // bottom-right
    for (iRow = nHeight-2; iRow < nHeight; iRow++)
    {
        for (iCol = nWidth-2; iCol < nWidth; iCol++)
        {
            if (::GetPixel(m_hDCWhole, iCol, iRow) == crTransColor)
            {
                int iX = rc.right-rc.left-(nWidth-iCol);
                int iY = rc.bottom-rc.top-(nHeight-iRow);
                rgnDec.SetRectRgn(iX, iY, iX+1, iY+1);
                rgnClient.CombineRgn(&rgnClient, &rgnDec, RGN_XOR);	
            }
        }
    }

    SetWindowRgn(rgnClient, TRUE);
    rgnDec.DeleteObject();
    rgnClient.DeleteObject();
}


// 加载位图
bool CAniButton::LoadAniBitmap(HBITMAP hBitmap, 
							   HWND hWndParent, 
							   int nIDEvent,
							   int nIDBtn,
							   bool bExpand, 
							   COLORREF crTransColor)
{
	if (NULL == hBitmap)
		return false;

	if (NULL == hWndParent)
		return false;

	if (0 == nIDEvent)
		return false;

	if (0 == nIDBtn)
		return false;

	m_hWndParent = hWndParent;
	m_nIDEvent = nIDEvent;
	m_nIDBtn = nIDBtn;

	HDC hDC = ::GetDC(GetSafeHwnd());
	m_hDCSingle = CreateCompatibleDC( hDC );
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

	HBITMAP hBmpSingle = CreateCompatibleBitmap( m_hDCWhole, m_nWidth, m_nHeight );
	SelectObject( m_hDCSingle, hBmpSingle );
	BOOL bFlag = ::BitBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*0, 0, SRCCOPY);

	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.AlphaFormat = 0;
	m_bf.SourceConstantAlpha = 0;

	MakeWindowRgn(crTransColor);

	if (NULL != GetSafeHwnd())
	{		
		if (!(GetButtonStyle() & WS_CLIPSIBLINGS))
			SetWindowLong(GetSafeHwnd(), GWL_STYLE, GetWindowLong(GetSafeHwnd(), GWL_STYLE) | WS_CLIPSIBLINGS);
	}

	return true;
}

// 加载位图
bool CAniButton::LoadAniBitmap(TCHAR* szFileName, 
                               HWND hWndParent, 
                               int nIDEvent,
                               int nIDBtn,
                               bool bExpand, 
                               COLORREF crTransColor)
{
	if (NULL == szFileName)
		return false;

	CImage img;
	img.Load( szFileName );
	if ( img.IsNull() )
	{
		img.Destroy();
		return false;
	}
    
	return LoadAniBitmap( (HBITMAP)img.Detach(), hWndParent, nIDEvent, nIDBtn, bExpand, crTransColor );
}


void CAniButton::OnTimer(UINT nIDEvent)
{
    if (m_nIDEvent == nIDEvent)
    {
        HDC hDC = ::GetDC(GetSafeHwnd());

        // 核心部分
        m_bf.SourceConstantAlpha += ALPHA_INCREMENT;
        AlphaBlend(hDC, 0, 0, m_nWidth, m_nHeight, m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_bf);

        ::ReleaseDC(GetSafeHwnd(), hDC);

        CButton::OnTimer(nIDEvent);
    }
}


void CAniButton::OnPaint()
{
    // 若不将内存DC：m_hDCSingle拷贝给dc，仍能显示，但按下按钮时闪烁严重
    CPaintDC dc(this);
    CDC* pDC = CDC::FromHandle(m_hDCSingle);

	dc.BitBlt(0, 0, m_nWidth, m_nHeight, pDC, 0, 0, SRCCOPY);
}


void CAniButton::OnLButtonDown(UINT nFlags, CPoint point)
{
    // 按下状态
    ::StretchBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*1, 0, m_nWidth, m_nHeight, SRCCOPY);

    // 屏蔽基类OnLButtonDown
    // CButton::OnLButtonDown(nFlags, point);

    // 因为屏蔽了点击消息，所以父窗口无法处理点击事件，在此发消息给父窗口替代点击事件
    ::PostMessage(m_hWndParent, MSG_TO_PARENT_CLICKED_DOWN, m_nIDBtn, NULL);
}


void CAniButton::OnLButtonUp(UINT nFlags, CPoint point)
{
    // Hover状态
    ::StretchBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*4, 0, m_nWidth, m_nHeight, SRCCOPY);

    // 屏蔽基类OnLButtonUp
    // CButton::OnLButtonUp(nFlags, point);

	// 因为屏蔽了点击消息，所以父窗口无法处理点击事件，在此发消息给父窗口替代点击事件
	::PostMessage(m_hWndParent, MSG_TO_PARENT_CLICKED_UP, m_nIDBtn, NULL);
}


void CAniButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
    // 按下状态
    ::StretchBlt(m_hDCSingle, 0, 0, m_nWidth, m_nHeight, m_hDCWhole, m_nWidth*1, 0, m_nWidth, m_nHeight, SRCCOPY);

    // 屏蔽基类OnLButtonDblClk
    // CButton::OnLButtonDblClk(nFlags, point);
}

