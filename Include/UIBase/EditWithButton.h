#pragma once
#include <atlimage.h>
#include "AutoFont.h"


#define WM_USER_EDITWITHBUTTON_CLICKED	(WM_USER + 3000)
#define WM_USER_EDITWITHBUTTON_KEYDWON	(WM_USER + 3001)

#define ID_CONTEXT_MENU_ITEM_CUT         0x100
#define ID_CONTEXT_MENU_ITEM_COPY        0x101
#define ID_CONTEXT_MENU_ITEM_PASTE       0x102
#define ID_CONTEXT_MENU_ITEM_SELALL      0x103

#define TIMER_ID_KEY_EVENT_DELAY_NOTIFY  0x500

// CEditWithButton

struct TEditWithButtonNotifier
{
	virtual void   OnKeyDown( UINT nNotifyMsgId, UINT nChar ) = 0;
	virtual void   OnClicked( UINT nNotifyMsgId ) = 0;
};

class CEditWithButton : public CEdit
{
	DECLARE_DYNAMIC(CEditWithButton)

public:
	CEditWithButton();
	virtual ~CEditWithButton();

protected:
	CImage          m_imgState;
	CSize           m_sizeState;
	CRect	        m_rcEditArea;
	CRect	        m_rcButtonArea;
	BOOL	        m_bButtonExistsAlways;
	UINT	        m_nButtonClickedMessageId;
	UINT            m_nKeyDownMessageID;
	CAutoFont       m_fntDef;
	CString         m_sHintText;
	TCHAR           m_tchKeyPressed;

private:
	TEditWithButtonNotifier* m_pNotifier;

public:
	BOOL            SetImages( CString sImagePath );
	void            SetButtonArea( CRect rcButtonArea );
	BOOL            SetEditArea( CRect rcEditArea );
	void            SetButtonClickedMessageId( UINT nButtonClickedMessageId );
	void            SetKeyDownMessageId( UINT nKeyDwonMessageId );
	void            SetButtonExistsAlways( BOOL bButtonExistsAlways );
	void            SetHintText( CString sText, BOOL bRedraw = TRUE );
	void            SetNotifier( TEditWithButtonNotifier* pNotifier );

protected:
	void            ResizeWindow();
	BOOL            IsHintText();

protected:
	virtual void    PreSubclassWindow( );
	virtual BOOL    PreTranslateMessage( MSG* pMsg );

	DECLARE_MESSAGE_MAP()
	
public:
	afx_msg BOOL    OnEraseBkgnd(CDC* pDC);
	afx_msg void    OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void    OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL    OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg int     OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void    OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM lParam); // Maps to WM_SETFONT

	afx_msg void    OnTextCut();
	afx_msg void    OnTextCopy();
	afx_msg void    OnTextPaste();
	afx_msg void    OnTextSelAll();

	afx_msg HBRUSH  CtlColor( CDC* pDC, UINT id );
	afx_msg void    OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void    OnSetFocus(CWnd* pOldWnd);
	afx_msg void    OnContextMenu(CWnd* pWnd, CPoint point );
	afx_msg void    OnKillFocus(CWnd* pNewWnd);
	afx_msg void    OnTimer(UINT nIDEvent);
};


