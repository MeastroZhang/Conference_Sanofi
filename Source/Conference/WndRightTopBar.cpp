#include "StdAfx.h"
#include "WndRightTopBar.h"

extern VidyoClientLectureStage g_lectureMode;
extern tSelfControlStatus g_tBtnStatus;

CWndRightTopBar::CWndRightTopBar(void)
{
}


CWndRightTopBar::~CWndRightTopBar(void)
{
}

LRESULT CWndRightTopBar::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CWndRightTopBar::ShowUI(bool bShow /* = true */)
{
	if (bShow)
	{
		int iCount = 0;
		CButtonUI* btnFlgModerator = static_cast<CButtonUI*>(m_pm.FindControl(L"btnFlgModerator"));
		CButtonUI* btnFlgRaiseHand = static_cast<CButtonUI*>(m_pm.FindControl(L"btnFlgRaiseHand"));
		CButtonUI* btnNoVideo = static_cast<CButtonUI*>(m_pm.FindControl(L"btnNoVideo"));
		CButtonUI* btnNoAudio = static_cast<CButtonUI*>(m_pm.FindControl(L"btnNoAudio"));
		CButtonUI* btnNoSpeaker = static_cast<CButtonUI*>(m_pm.FindControl(L"btnNoSpeaker"));
		CButtonUI* btnRestore = static_cast<CButtonUI*>(m_pm.FindControl(L"btnRestore"));

		if (g_lectureMode == VIDYO_CLIENT_LECTURE_MODE_PRESENT)
		{
			iCount ++;
			btnFlgModerator->SetVisible();
		}
		else btnFlgModerator->SetVisible(false);

		if (g_tBtnStatus.bMuteVideo || g_tBtnStatus.bMuteServerVideo)
		{
			iCount ++;
			btnNoVideo->SetVisible();
		}
		else btnNoVideo->SetVisible(false);

		if (g_tBtnStatus.bMuteAudioIn || g_tBtnStatus.bMuteServerAuidoIn)
		{
			iCount ++;
			btnNoAudio->SetVisible();
		}
		else btnNoAudio->SetVisible(false);

		if (g_tBtnStatus.bMuteAudioOut)
		{
			iCount ++;
			btnNoSpeaker->SetVisible();
		}
		else btnNoSpeaker->SetVisible(false);

		if (g_tBtnStatus.bFullScreen)
		{
			iCount ++;
			btnRestore->SetVisible();
		}
		else btnRestore->SetVisible(false);

		RECT rc;
		GetClientRect(GetParent(m_hWnd), &rc);
		::SetWindowPos(m_hWnd, NULL, rc.right-iCount*32-10, 40, 
			iCount*32, 32, NULL);
	}

	ShowWindow(bShow);
}

void CWndRightTopBar::Notify(TNotifyUI& msg)
{
	if (msg.sType == L"click")
	{
		if (msg.pSender->GetName() == L"btnRestore")
		{
			::PostMessageW(GetParent(m_hWnd), WM_SYSCOMMAND, SC_RESTORE, NULL);
		}
	}
}