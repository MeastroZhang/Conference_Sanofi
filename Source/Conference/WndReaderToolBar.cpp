#include "StdAfx.h"
#include "WndReaderToolBar.h"
#include "../../include/Util/Util.h"

extern tSelfControlStatus g_tBtnStatus;
extern tStartTime g_StartTime;
static bool g_bHideToolBar = false;

CWndReaderToolBar::CWndReaderToolBar(HWND hParent)
{
	m_hParent = hParent;
	m_bTimer = false;
}


CWndReaderToolBar::~CWndReaderToolBar(void)
{
}

void CWndReaderToolBar::InitWindow()
{
	m_pBtnMic = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnMic")));
	m_pBtnMicClosed = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnMicClosed")));
	m_pBtnViedo = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnVideo")));
	m_pBtnVideoClosed = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnVideoClosed")));
	m_pBtnParticipants = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnParticipants")));
	m_pBtnShare = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnShare")));
	m_pBtnReShare = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnReShare")));
	m_pBtnStopShare = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnStopShare")));
	//m_pBtnMore = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnMore")));

	SetTimer(m_hWnd, TIMER_CLOCK, 1000, NULL);
}

LRESULT CWndReaderToolBar::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_TIMER)
	{
		OnTimer(uMsg, wParam, lParam, bHandled);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		g_bHideToolBar = true;
		SetTimer(m_hWnd, TIMER_HIDE_TOOLBAR, 3000, NULL);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		g_bHideToolBar = false;
		HideUI(g_bHideToolBar);
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CWndReaderToolBar::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_CLOCK)
	{
		CalculateClock();
	}
	else if (wParam == TIMER_HIDE_TOOLBAR)
	{
		if (g_bHideToolBar) HideUI();
	}
	return 0;
}

void CWndReaderToolBar::UpdateUI()
{
	m_pBtnViedo->SetVisible(!g_tBtnStatus.bMuteVideo);
	m_pBtnVideoClosed->SetVisible(g_tBtnStatus.bMuteVideo);

	m_pBtnMic->SetVisible(!g_tBtnStatus.bMuteAudioIn);
	m_pBtnMicClosed->SetVisible(g_tBtnStatus.bMuteAudioIn);

	m_pBtnViedo->SetEnabled(!g_tBtnStatus.bMuteServerVideo);
	m_pBtnVideoClosed->SetEnabled(!g_tBtnStatus.bMuteServerVideo);
	m_pBtnMic->SetEnabled(!g_tBtnStatus.bMuteServerAuidoIn);
	m_pBtnMicClosed->SetEnabled(!g_tBtnStatus.bMuteServerAuidoIn);

	if (g_tBtnStatus.bShareScreen)
	{
		m_pBtnShare->SetVisible(false);
		m_pBtnReShare->SetVisible(false);
		m_pBtnStopShare->SetVisible();
	}
	else
	{
		m_pBtnShare->SetVisible(!g_tBtnStatus.bShareSpeaker);
		m_pBtnReShare->SetVisible(g_tBtnStatus.bShareSpeaker);
		m_pBtnStopShare->SetVisible(g_tBtnStatus.bShareSpeaker);

		m_pBtnShare->SetEnabled(g_tBtnStatus.bShareSpeaker);
		m_pBtnReShare->SetEnabled(g_tBtnStatus.bShareSpeaker);
		m_pBtnStopShare->SetEnabled(g_tBtnStatus.bShareSpeaker);
	}

	ShowWindow();
	
	g_bHideToolBar = true;
	SetTimer(m_hWnd, TIMER_HIDE_TOOLBAR, 3000, NULL);
	HideUI(false);
}

void CWndReaderToolBar::HideUI(bool bHide /* = true */)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);
	int iWidth = GetSystemMetrics(SM_CXSCREEN);
	::SetWindowPos(m_hWnd, NULL, (iWidth-(rc.right-rc.left))/2, -bHide*(rc.bottom-4), rc.right, rc.bottom, NULL);
}

void CWndReaderToolBar::CalculateClock()
{
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	CDuiString strTime;
	if (m_bTimer)
	{
		if ((sys.wSecond-g_StartTime.wSecond) < 0)
		{
			sys.wMinute -= 1;
			sys.wSecond += 60;
		}
		if ((sys.wMinute-g_StartTime.wMinute) < 0)
		{
			sys.wHour -= 1;
			sys.wMinute += 60;
		}
		if ((sys.wHour-g_StartTime.wHour) < 0)
		{
			sys.wHour += 24;
		}
		strTime.Format(_T("%02d:%02d:%02d"), sys.wHour-g_StartTime.wHour, sys.wMinute-g_StartTime.wMinute, sys.wSecond-g_StartTime.wSecond);
		CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnTimer")));
		pBtnTimer->SetText(strTime);
	}
	else
	{
		CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnClock")));
		strTime.Format(_T("%02d:%02d:%02d"), sys.wHour, sys.wMinute, sys.wSecond);
		pBtnClock->SetText(strTime);
	}
}

void CWndReaderToolBar::Notify(TNotifyUI& msg)
{
	CDuiString sName = msg.pSender->GetName();
	if ( msg.sType == DUI_MSGTYPE_CLICK )
	{
		if ( sName.CompareNoCase(L"btnStopShare") == 0 )
		{
			::PostMessageW(m_hParent, WM_SHARE, FALSE, NULL);
		}
		else if ( sName.CompareNoCase(L"btnMic") == 0 )
		{
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = TRUE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_IN, &privacy, sizeof(privacy));
		}
		else if ( sName.CompareNoCase(L"btnMicClosed") == 0 )
		{
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = FALSE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_AUDIO_IN, &privacy, sizeof(privacy));
		}
		else if ( sName.CompareNoCase(L"btnVideo") == 0 )
		{
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = TRUE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_VIDEO, &privacy, sizeof(privacy));
		}
		else if ( sName.CompareNoCase(L"btnVideoClosed") == 0 )
		{
			VidyoClientInEventMute privacy = {FALSE};
			privacy.willMute = FALSE;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_MUTE_VIDEO, &privacy, sizeof(privacy));
		}
		else if ( sName.CompareNoCase(L"btnParticipants") == 0 )
		{
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_CONTROL, NULL);
		}
		else if ( sName.CompareNoCase(L"btnReShare") == 0 )
		{
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_SHARE, (LPARAM)SHARE_RESHARE);
		}
		else if ( sName.CompareNoCase(L"btnMore") == 0 )
		{
		}
		else if ( sName.CompareNoCase(L"btnClock") == 0 )
		{
			m_bTimer = true;
			CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnClock")));
			CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnTimer")));
			pBtnClock->SetVisible(!m_bTimer);
			pBtnTimer->SetVisible(m_bTimer);
			CalculateClock();
		}
		else if ( sName.CompareNoCase(L"btnTimer") == 0 )
		{
			m_bTimer = false;
			CButtonUI* pBtnClock = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnClock")));
			CButtonUI* pBtnTimer = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnTimer")));
			pBtnClock->SetVisible(!m_bTimer);
			pBtnTimer->SetVisible(m_bTimer);
			CalculateClock();
		}
		else if ( sName.CompareNoCase(L"btnExit") == 0 )
		{
			::PostMessageW(m_hParent, WM_TOOLBAR, (WPARAM)TB_MSG_EXIT, NULL);
		}
	}
}