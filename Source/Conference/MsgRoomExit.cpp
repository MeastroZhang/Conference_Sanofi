#include "StdAfx.h"
#include "MsgRoomExit.h"
#include "../../include/Util/Util.h"

CMsgRoomExit::CMsgRoomExit(HWND hParent)
{
	m_hParent = hParent;
	m_iCount = 10;
}


CMsgRoomExit::~CMsgRoomExit(void)
{
}

LRESULT CMsgRoomExit::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CMsgRoomExit::InitWindow()
{
	CLabelUI* labExit = static_cast<CLabelUI*>(m_pm.FindControl(_T("labExit")));
	CLabelUI* labExitDBUP = static_cast<CLabelUI*>(m_pm.FindControl(_T("labExitDBUP")));
	CButtonUI* pbtnCancel	= static_cast<CButtonUI*>(m_pm.FindControl(_T("btnCancel")));
	CButtonUI* pbtnOK	= static_cast<CButtonUI*>(m_pm.FindControl(_T("btnOK")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("MSGWND"), _T("labExit"), szText, MAX_PATH);
	labExit->SetText(szText);
	project_language_loadstring(_T("MSGWND"), _T("labExitDBUP"), szText, MAX_PATH);
	labExitDBUP->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnCancel"), szText, MAX_PATH);
	pbtnCancel->SetText(szText);
	project_language_loadstring(_T("COMMON"), _T("btnOK"), szText, MAX_PATH);
	pbtnOK->SetText(szText);
}

LRESULT CMsgRoomExit::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;
	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_TIMER:		   lRes = OnTimer(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CMsgRoomExit::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_EXIT)
	{
		CLabelUI* labExitDBUP = static_cast<CLabelUI*>(m_pm.FindControl(_T("labExitDBUP")));
		TCHAR szText[MAX_PATH];
		project_language_loadstring(_T("MSGWND"), _T("labExitDBUP"), szText, MAX_PATH);

		m_iCount--;
		if (m_iCount == 0)
		{
			KillTimer(m_hWnd, TIMER_EXIT);
			labExitDBUP->SetText(szText);
			CHorizontalLayoutUI* horExit = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horExit"));
			horExit->SetVisible(true);
		}
		else
		{
			CDuiString sText;
			sText.Format(L"%s    Ê£Óà%dÃë", szText, m_iCount);
			labExitDBUP->SetText(sText);
		}
	}

	bHandled = FALSE;
	return 0;
}

void CMsgRoomExit::ShowUI(bool bDBUP, bool bShow /* = true */)
{
	if (bDBUP)
	{
		m_iCount = 10;
		SetTimer(m_hWnd, TIMER_EXIT, 1000, NULL);
		CHorizontalLayoutUI* horExit = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horExit"));
		horExit->SetVisible(false);
	}
	else
	{
		CHorizontalLayoutUI* horExitDBUP = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horExitDBUP"));
		horExitDBUP->SetVisible(false);
		SetWindowPos(m_hWnd, NULL, 0, 0, 588, 132, NULL);
		CenterWindow();
	}
	
	ShowWindow(bShow);
}

void CMsgRoomExit::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnOK"))
		{
			::PostMessageW(m_hParent, WM_EXIT_CONFERENCE, NULL, NULL);
		}
		ShowWindow(false);
	}
}