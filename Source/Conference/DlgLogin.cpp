#include "StdAfx.h"
#include "DlgLogin.h"
#include "HttpClient.h"
#include "../../include/Util/Util.h"
#include "../../include/VidyoClient/VidyoClient.h"

extern VidyoClientRequestConfiguration reqConfig;

static VidyoClientInEventLogIn logInParams = {0};
static VidyoClientRequestVolume requestVolume = {0};
static VidyoClientInEventRoomLink roomLink = {0};

extern string g_sRoomCode;
extern string g_sRoomID;
extern string g_sRoomName;
extern string g_sRoomUrl;
extern string g_sUserEmail;
extern string g_sStartDate;
extern int	g_iTotalUser;

CDlgLogin::CDlgLogin(HWND hParent)
{
	m_hParent = hParent;
	m_bLogin = false;
	m_bTrayLogout = false;
}

CDlgLogin::~CDlgLogin()
{
	m_hParent = NULL;
}

LRESULT CDlgLogin::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_TIMER)
	{
		OnTimer(uMsg, wParam, lParam, bHandled);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgLogin::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntLoginUI, NULL);

	bHandled = TRUE;
	return 0;
}

void CDlgLogin::InitWindow()
{
	m_pBtnJoin = static_cast<CButtonUI*>(m_pm.FindControl(_T("btnJoin")));

	CLabelUI* labInviteCode = static_cast<CLabelUI*>(m_pm.FindControl(L"labInviteCode"));
	CLabelUI* labUserEmail = static_cast<CLabelUI*>(m_pm.FindControl(L"labUserEmail"));
	CLabelUI* labCurUser = static_cast<CLabelUI*>(m_pm.FindControl(L"labCurUser"));
	CEditUI* pEditRoomID = static_cast<CEditUI*>(m_pm.FindControl(_T("editInviteCode")));
	CEditUI* pEditDisplayName = static_cast<CEditUI*>(m_pm.FindControl(_T("editUserEmail")));
	CEditUI* pEditCurUser = static_cast<CEditUI*>(m_pm.FindControl(_T("editCurUser")));
	COptionUI* pOptMic = static_cast<COptionUI*>(m_pm.FindControl(_T("optMic")));
	COptionUI* pOptVideo = static_cast<COptionUI*>(m_pm.FindControl(_T("optVideo")));

	TCHAR szText[MAX_PATH], szOldText[MAX_PATH];
	project_language_loadstring(_T("LOGIN"), _T("labInviteCode"), szText, MAX_PATH);
	labInviteCode->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("labUserEmail"), szText, MAX_PATH);
	labUserEmail->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("labCurUser"), szText, MAX_PATH);
	labCurUser->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("editInviteCode"), szText, MAX_PATH);
	ReadIniString(_T("USERINFO"), _T("IniviteCode"), szText, szOldText, 255);
	pEditRoomID->SetText(szOldText);
	project_language_loadstring(_T("LOGIN"), _T("editUserEmail"), szText, MAX_PATH);
	ReadIniString(_T("USERINFO"), _T("UserEmail"), szText, szOldText, 255);
	pEditDisplayName->SetText(szOldText);
	project_language_loadstring(_T("LOGIN"), _T("editCurUser"), szText, MAX_PATH);
	pEditCurUser->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("optMic"), szText, MAX_PATH);	
	pOptMic->SetText(szText);
	project_language_loadstring(_T("LOGIN"), _T("optVideo"), szText, MAX_PATH);	
	pOptVideo->SetText(szText);
}

void CDlgLogin::InitUI()
{
	m_bLogin = false;
	CHorizontalLayoutUI* horJoinEdit = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinEdit"));
	CHorizontalLayoutUI* horJoinResult = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinResult"));
	CHorizontalLayoutUI* horJoinSetting = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinSetting"));
	horJoinEdit->SetVisible(true);
	horJoinResult->SetVisible(false);
	horJoinSetting->SetVisible(true);

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);	
	m_pBtnJoin->SetText(szText);
	m_pBtnJoin->SetEnabled();

	COptionUI* pOptMic = static_cast<COptionUI*>(m_pm.FindControl(_T("optMic")));
	COptionUI* pOptVideo = static_cast<COptionUI*>(m_pm.FindControl(_T("optVideo")));
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	pOptMic->Selected(reqConfig.enableMuteMicrophoneOnJoin);
	pOptVideo->Selected(reqConfig.enableHideCameraOnJoin);
}

void CDlgLogin::ShowRoomInfo()
{
	CHorizontalLayoutUI* horJoinEdit = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinEdit"));
	CHorizontalLayoutUI* horJoinResult = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinResult"));
	CHorizontalLayoutUI* horJoinSetting = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horJoinSetting"));
	CLabelUI* labRoomID = static_cast<CLabelUI*>(m_pm.FindControl(L"labRoomID"));
	CLabelUI* labConferenceName = static_cast<CLabelUI*>(m_pm.FindControl(L"labConferenceName"));
	CLabelUI* labTime = static_cast<CLabelUI*>(m_pm.FindControl(L"labTime"));
	CLabelUI* labError = static_cast<CLabelUI*>(m_pm.FindControl(L"labError"));

	horJoinEdit->SetVisible(false);
	horJoinResult->SetVisible(true);
	horJoinSetting->SetVisible(false);

	TCHAR szTemp[MAX_PATH], szText[MAX_PATH];
	CDuiString sText;
	ChangUtf8ToText(g_sRoomCode.c_str(), szTemp);
	project_language_loadstring(_T("LOGIN"), _T("labRoomID"), szText, MAX_PATH);
	sText.Format(L"%s%s", szText, szTemp);
	labRoomID->SetText(sText);
	ChangUtf8ToText(g_sRoomName.c_str(), szTemp);
	project_language_loadstring(_T("LOGIN"), _T("labSubject"), szText, MAX_PATH);
	sText.Format(L"%s%s", szText, szTemp);
	labConferenceName->SetText(sText);
	ChangUtf8ToText(g_sStartDate.c_str(), szTemp);
	project_language_loadstring(_T("LOGIN"), _T("labDate"), szText, MAX_PATH);
	sText.Format(L"%s%s",szText,  szTemp);
	labTime->SetText(sText);
	project_language_loadstring(_T("LOGIN"), _T("btnJoinConfirm"), szText, MAX_PATH);
	m_pBtnJoin->SetText(szText);

	m_bLogin = true;
	SetTimer(m_hWnd, TIMER_HTTP_ALIVE, 5*60*1000, NULL);
}

void CDlgLogin::JoinRoom()
{
	if (g_sRoomUrl.empty()) return ;

	string url, key;
	int i = g_sRoomUrl.rfind('/');
	if ((i>0) && (i<g_sRoomUrl.length())) url = g_sRoomUrl.substr(0, i);
	i = g_sRoomUrl.find('=');
	if ((i>0) && (i<g_sRoomUrl.length())) key = g_sRoomUrl.substr(i+1, g_sRoomUrl.length());

	strcpy(roomLink.portalUri, url.c_str());
	strcpy(roomLink.roomKey, key.c_str());
	strcpy(roomLink.displayName, g_sUserEmail.c_str());
	strcpy(roomLink.pin, "");
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
	roomLink.muteMicrophone = reqConfig.enableMuteMicrophoneOnJoin;
	roomLink.muteCamera = reqConfig.enableHideCameraOnJoin;

	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_ROOM_LINK, &roomLink, sizeof(roomLink)) != VIDYO_FALSE)
	{
		m_pBtnJoin->SetEnabled(false);
		TCHAR szText[MAX_PATH];
		project_language_loadstring(_T("LOGIN"), _T("btnJoining"), szText, MAX_PATH);
		m_pBtnJoin->SetText(szText);
	}
}

LRESULT CDlgLogin::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		if (m_pBtnJoin->IsVisible()) 
		{
			m_pBtnJoin->SetFocus();
			m_pBtnJoin->Activate();
		}
			
		return TRUE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CDlgLogin::Login()
{
	m_bLogin = false;
	LoginErrorTips(NULL);
	CEditUI* editInviteCode = static_cast<CEditUI*>(m_pm.FindControl(_T("editInviteCode")));
	CEditUI* editUserEmail = static_cast<CEditUI*>(m_pm.FindControl(_T("editUserEmail")));
	CEditUI* editCurUser = static_cast<CEditUI*>(m_pm.FindControl(_T("editCurUser")));
	CDuiString sCode = editInviteCode->GetText();
	if (sCode.IsEmpty())
	{
		LoginErrorTips(ERROR_CLIENT_ROOMID_NULL);
		return ;
	}
	CDuiString sEmail = editUserEmail->GetText();
	if (sEmail.IsEmpty())
	{
		LoginErrorTips(ERROR_CLIENT_DISPLAYNAME_NULL);
		return ;
	}
	CDuiString sCurUser = editCurUser->GetText();
	TCHAR szTemp[MAX_PATH];
	project_language_loadstring(_T("LOGIN"), _T("editCurUser"), szTemp, MAX_PATH);
	if (sCurUser.IsEmpty() || !sCurUser.Compare(L"0"))
	{
		LoginErrorTips(ERROR_CLIENT_USERNO_NULL);
		return ;
	}

	char szCode[128] = {0};
	char szEmail[128] = {0};
	char szCurNo[128] = {0};
	ChangTextToUtf8(sCode, szCode);
	ChangTextToUtf8(sEmail, szEmail);
	ChangTextToUtf8(sCurUser, szCurNo);

	int iResult = curl_joinroom(szCode, szEmail, szCurNo);
	if (iResult == 0)
	{
		WriteIniString(_T("USERINFO"), _T("IniviteCode"), sCode);
		WriteIniString(_T("USERINFO"), _T("UserEmail"), sEmail);

		ShowRoomInfo();
	}
	else if (iResult == 1)
		LoginErrorTips(ERROR_SERVER_DISCONNECT);
	else if (iResult == 2) 
		LoginErrorTips(ERROR_CLIENT_INPUT_WRONG);
	else 
		LoginErrorTips(ERROR_SERVER_ABNORMAL);
}

void CDlgLogin::LoginErrorTips(unsigned int errorTpye)
{
	bool bBtnLogin = false;
	bool bLabLoginErrro = false;
	TCHAR szText[MAX_PATH] = {0};
	if (errorTpye)
	{
		switch (errorTpye)
		{
		case ERROR_CLIENT_USERNAME_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("usernameNull"), szText, MAX_PATH);
			break;
		case ERROR_CLIENT_PSW_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("PswNull"), szText, MAX_PATH);
			break;
		case ERROR_CLIENT_USERNO_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("userNoNull"), szText, MAX_PATH);
			break;
		case ERROR_CLIENT_ROOMID_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("RoomIDNull"), szText, MAX_PATH);
			break;
		case ERROR_CLIENT_DISPLAYNAME_NULL:
			project_language_loadstring(_T("LOGINERROR"), _T("displaynameNull"), szText, MAX_PATH);
			break;
		case ERROR_CLIENT_INPUT_WRONG:
			project_language_loadstring(_T("LOGINERROR"), _T("inputwrong"), szText, MAX_PATH);
			break;
		case ERROR_SERVER_DISCONNECT:
			project_language_loadstring(_T("LOGINERROR"), _T("disconnect"), szText, MAX_PATH);
			break;
		case ERROR_SERVER_ABNORMAL:
			project_language_loadstring(_T("LOGINERROR"), _T("abnormal"), szText, MAX_PATH);
			break;
		case VIDYO_CLIENT_ERROR_INTERNAL_NO_PORTAL_ADDRESS:
			project_language_loadstring(_T("LOGINERROR"), _T("portalAddressError"), szText, MAX_PATH);
			break;
		default:
			project_language_loadstring(_T("LOGINERROR"), _T("defaultError"), szText, MAX_PATH);
			break;
		}
		bLabLoginErrro = true;
	}
	else
	{
		bBtnLogin = true;
		bLabLoginErrro = false;
	}
	CLabelUI* labError = static_cast<CLabelUI*>(m_pm.FindControl(L"labError"));
	labError->SetText(szText);
	labError->SetVisible(bLabLoginErrro);
	project_language_loadstring(_T("LOGIN"), _T("btnJoin"), szText, MAX_PATH);
	m_pBtnJoin->SetText(szText);

}

void CDlgLogin::SetTrayLogout(bool bTrayLogout /* = true */)
{
	m_bTrayLogout = bTrayLogout;
}

LRESULT CDlgLogin::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_HTTP_ALIVE)
	{
		curl_alive();
	}

	return 0;
}

void CDlgLogin::Notify(TNotifyUI& msg)
{
	CDuiString sName = msg.pSender->GetName();
	if ( msg.sType == DUI_MSGTYPE_CLICK )
	{
		if( sName.CompareNoCase(L"btnClose") == 0 ) 
		{
			::PostMessageW(m_hParent, WM_WINDOW, (WPARAM)ntLoginUI, NULL);
		}
		else if ( msg.pSender == m_pBtnJoin )
		{
			if (m_bLogin)
				JoinRoom();
			else
				Login();
		}
		else if ( sName == L"btnMin" )
		{
			ShowWindow(false);
		}
		else if ( sName == L"btnSetting" )
		{
			CDlgSetting* pPopWnd = new CDlgSetting(m_hParent);
			pPopWnd->Create(m_hWnd, _T("Setting"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);
			pPopWnd->InitUI();
			pPopWnd->CenterWindow();
			pPopWnd->ShowModal();
		}
		else if ( sName.CompareNoCase(L"optMic") == 0 )
		{
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
			COptionUI* pOptMic = static_cast<COptionUI*>(m_pm.FindControl(_T("optMic")));
			reqConfig.enableMuteMicrophoneOnJoin = !pOptMic->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
		else if ( sName.CompareNoCase(L"optVideo") == 0 )
		{
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
			COptionUI* pOptVideo = static_cast<COptionUI*>(m_pm.FindControl(_T("optVideo")));
			reqConfig.enableHideCameraOnJoin = !pOptVideo->IsSelected();
			VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_CONFIGURATION , &reqConfig, sizeof(reqConfig));
		}
	}
	else if ( msg.sType == DUI_MSGTYPE_TEXTCHANGED )
	{
		LoginErrorTips(NULL);
	}
	else if ( msg.sType == DUI_MSGTYPE_SETFOCUS )
	{
		if ( sName.CompareNoCase(L"editInviteCode") == 0 )
		{
			TCHAR szTemp[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("editInviteCode"), szTemp, MAX_PATH);
			if (!((CEditUI*)msg.pSender)->GetText().Compare(szTemp))
			{
				((CEditUI*)msg.pSender)->SetText(L"");
			}
		}
		else if ( sName.CompareNoCase(L"editUserEmail") == 0 )
		{
			TCHAR szTemp[MAX_PATH];
			project_language_loadstring(_T("LOGIN"), _T("editUserEmail"), szTemp, MAX_PATH);
			if (!((CEditUI*)msg.pSender)->GetText().Compare(szTemp))
			{
				((CEditUI*)msg.pSender)->SetText(L"");
			}
		}
	}
}