#include "StdAfx.h"
#include "DlgDocReader.h"
#include "WkeWebkit.h"

extern string g_sRoomID;

CDlgDocReader::CDlgDocReader(HWND hParent)
{
	m_hParent = hParent;
	m_pWke = NULL;
	m_pWndReaderBar = NULL;
}


CDlgDocReader::~CDlgDocReader(void)
{
}

LRESULT CDlgDocReader::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT CDlgDocReader::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MOUSEMOVE)
	{
		//RECT rc;
		//GetClientRect(m_pWndReaderBar->GetHWND(), &rc);
		//if (!::PtInRect(&rc, msg.ptMouse))
		//{
		//	m_pWndReaderBar->ShowWindow(false);
		//}
	}

	bHandled = FALSE;
	return 0;
}

void CDlgDocReader::InitWindow()
{
	if (m_pWndReaderBar == NULL)
	{
		m_pWndReaderBar = new CWndReaderToolBar(m_hParent);
		m_pWndReaderBar->Create(m_hWnd, NULL, UI_WNDSTYLE_CHILD,  NULL, 0, 0, 0, 0);
	}

	m_pWke = static_cast<CWkeWebkitUI*>(m_pm.FindControl(_T("wkeWebkit")));
}

CControlUI* CDlgDocReader::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcsicmp(pstrClassName, _T("wkeWebView")) == 0) return  new CWkeWebkitUI;
	return NULL;
}

LRESULT CDlgDocReader::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessageW(m_hParent, WM_SHARE, FALSE, NULL);

	bHandled = TRUE;
	return 0;
}

void CDlgDocReader::UpdateUI()
{
	RECT rc;
	SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rc, 0);
	SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right, rc.bottom, NULL);

	m_pWndReaderBar->UpdateUI();
}

void CDlgDocReader::SetDocUrl(CDuiString strUrl, bool bSpeaker)
{
	CDuiString sAudienceUrl, sSpeakerUrl;
	TCHAR szRoomID[MAX_PATH] = {0};
	ChangUtf8ToText(g_sRoomID.c_str(), szRoomID);
	sAudienceUrl.Format(L"%s?type=audience", strUrl);
	sAudienceUrl += L"&id=";
	sAudienceUrl += szRoomID;
	sSpeakerUrl.Format(L"%s?type=speaker", strUrl);
	sSpeakerUrl += L"&id=";
	sSpeakerUrl += szRoomID;

	m_pWke->Navigate(bSpeaker?sSpeakerUrl:sAudienceUrl);

	UpdateUI();
}

void CDlgDocReader::Notify(TNotifyUI& msg)
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
}