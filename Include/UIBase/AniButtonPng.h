#pragma once

#ifndef CANIBUTTONPNG_HEAD_FILE
#define CANIBUTTONPNG_HEAD_FILE


#include "afxwin.h"
#include <afxtempl.h>
#define MSG_TO_PARENT_CLICKED_DOWN    (WM_USER+300)
#define MSG_TO_PARENT_CLICKED_UP	  (WM_USER+301)	


typedef enum 
{ 
	NORMLE, 
	CLICK, 
	FOCUS, 
	UNABLE, 
	HOVER 
}E_STATE;//该顺序根据图片顺序需要变动


class CAniButtonPng :
	public CButton
{
public:
	CAniButtonPng(void);
	~CAniButtonPng(void);

	virtual bool LoadAniBitmap(LPCTSTR szFileName, 
								HWND hWndParent,
								int nIDBtn,
								bool bExpand = false);

	virtual bool LoadAniBitmap(HBITMAP hBitmap, 
								HWND hWndParent,
								int nIDBtn,
								bool bExpand = false);

	void SetAutoClickUp( BOOL bAuto ); // 设定是否点击后自动弹起
	void SetClicked( BOOL bClicked );
	void SetBmpLeft( BOOL bLeft ); // 设定是否文字左侧显示icon
	void SetDisabled( BOOL bDisabled );
	BOOL GetAutoClickUp(){return m_bAutoClickUp;};
	BOOL GetClicked(){return m_bClicked;};
	void RePaintBK();

protected:
	virtual void PaintBK(CDC* pDC);
	virtual void PaintPng( CDC *pDC );
	virtual void DrawBtnText( CDC *pDC );

private:
	E_STATE		m_eState;
	bool		m_bIsMouseMove;
	HDC			m_hDCWhole;
	HDC			m_hDCBK;
	HBITMAP		m_hBmpWhole;
	HBITMAP     m_bmpBk;
	int			m_nWidth;
	int			m_nHeight;
	HWND		m_hWndParent;
	int			m_nIDBtn;
	BOOL		m_bAutoClickUp;
	BOOL		m_bClicked;
	BOOL		m_bBmpLeft;
	BOOL		m_bDisabled;

	//消息映射
	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd( CDC* pDC );
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

typedef CList<CAniButtonPng*>  LIST_ANI_BTNPNG;

#endif