//----------------------------------------------------------------------
// File: AniButton.h
// 
// 
// Create: 
//     2012-04-13 
//
// History:
// 
//----------------------------------------------------------------------

#pragma once

#ifndef CANIBUTTON_HEAD_FILE
#define CANIBUTTON_HEAD_FILE

#define MSG_TO_PARENT_CLICKED_DOWN    (WM_USER+300)
#define MSG_TO_PARENT_CLICKED_UP	  (WM_USER+301)	


class CAniButton : public CButton
{
public:
	CAniButton();
	virtual ~CAniButton();
	bool LoadAniBitmap(TCHAR* szFileName, 
        HWND hWndParent,
        int nIDEvent,
        int nIDBtn,
        bool bExpand = false, 
        COLORREF crTransColor = RGB(255,255,255));
	bool LoadAniBitmap(HBITMAP hBitmap, 
		HWND hWndParent,
		int nIDEvent,
		int nIDBtn,
		bool bExpand = false, 
		COLORREF crTransColor = RGB(255,255,255));

private:
    bool m_bIsMouseMove;
    HDC m_hDCWhole;
    HDC m_hDCSingle;
    HBITMAP m_hBmpWhole;
    BLENDFUNCTION m_bf;
    int m_nWidth;
    int m_nHeight;
    HWND m_hWndParent;
    int m_nIDEvent;
    int m_nIDBtn;

    void MakeWindowRgn(COLORREF crTransColor);
    enum { TIMER_ELAPSE = 10 };
    enum { ALPHA_INCREMENT = 2 };

	//œ˚œ¢”≥…‰
    DECLARE_MESSAGE_MAP()
public:
    LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnTimer(UINT nIDEvent);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

#endif

