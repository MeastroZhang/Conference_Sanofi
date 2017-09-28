#include "StdAfx.h"
#include "DlgShareDock.h"


CDlgShareDock::CDlgShareDock(HWND hParent)
{
	m_hParent = hParent;
	m_pWndReaderBar = NULL;
}


CDlgShareDock::~CDlgShareDock(void)
{
}

LRESULT CDlgShareDock::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE)
	{
		RECT rc, rcDesk;
		GetClientRect(m_pWndReaderBar->GetHWND(), &rc);
		GetClientRect(GetDesktopWindow(), &rcDesk);
		SetWindowPos(m_pWndReaderBar->GetHWND(), NULL, (rcDesk.right-rc.right)/2, 0, rc.right, rc.bottom, NULL);
		m_pWndReaderBar->ShowWindow();
		m_pWndReaderBar->UpdateUI();
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgShareDock::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CDlgShareDock::InitWindow()
{
	if (m_pWndReaderBar == NULL)
	{
		m_pWndReaderBar = new CWndReaderToolBar(m_hParent);
		m_pWndReaderBar->Create(m_hWnd, NULL, UI_WNDSTYLE_DIALOG,  NULL, 0, 0, 0, 0);
		m_pWndReaderBar->ShowWindow(false);
	}
}

void CDlgShareDock::ShowUI(bool bShow /* = true */)
{
	m_pWndReaderBar->ShowWindow(bShow);
	ShowWindow(bShow);
}

void CDlgShareDock::UpdateUI()
{
	m_pWndReaderBar->UpdateUI();
}

void CDlgShareDock::Notify(TNotifyUI& msg)
{
	if ( msg.sType == DUI_MSGTYPE_CLICK )
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			::PostMessageW(m_hParent, WM_SHARE, FALSE, NULL);
		}
		else if ( msg.pSender->GetName() == L"btnMin" )
		{
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_MIN, NULL);
		}
	}
	else if ( msg.sType == DUI_MSGTYPE_MOUSEENTER )
	{
		m_pWndReaderBar->ShowWindow();
	}
}