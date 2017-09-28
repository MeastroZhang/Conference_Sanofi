#include "StdAfx.h"
#include "DlgConference.h"

VidyoClientRequestParticipants g_reqParticipants = {0};
VidyoClientLectureStage g_lectureMode = {VIDYO_CLIENT_LECTURE_MODE_STOP};
extern tSelfControlStatus g_tBtnStatus;

tStartTime g_StartTime;

extern string g_sRoomCode;

CDuiString g_sShareUrl;
bool g_bExiting = false;
bool g_bSharing = false;

CDlgConference::CDlgConference(HWND pParent)
	: m_pWndToolbar(NULL)
	, m_pWndRightTopBar(NULL)
	, m_pWndLectureMode(NULL)
	, m_bShowToolBar(FALSE)
	, m_pDlgDocReader(NULL)
	, m_pDlgShareDock(NULL)
	, m_pDlgSharing(NULL)
	, m_pDlgParticipants(NULL)
	, m_pMsgRoomExit(NULL)
	, m_pWndUploadProcessing(NULL)
{
	m_pParent = pParent;

	m_iChatCount = 0;
	m_ptEntityData = NULL;

	m_pDlgPrivateChat = NULL;

	m_bHavePresenter = false;
}


CDlgConference::~CDlgConference()
{
	m_pParent = NULL;

	if (m_ptEntityData)
	{
		delete m_ptEntityData;
		m_ptEntityData = NULL;
	}
}

LRESULT CDlgConference::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
		return TRUE;
	}
	return FALSE;
}

void CDlgConference::InitWindow()
{
	if (m_pWndToolbar == NULL)
	{
		m_pWndToolbar = new CWndToolbar(m_hWnd);
		m_pWndToolbar->Create(m_hWnd, _T("Toolbar"), UI_WNDSTYLE_CHILD,  NULL, 0, 0, 0, 0);
		m_pWndToolbar->ShowUI(false);
	}

	if (m_pWndRightTopBar == NULL)
	{
		m_pWndRightTopBar = new CWndRightTopBar;
		m_pWndRightTopBar->Create(m_hWnd, L"RigthTopBar", UI_WNDSTYLE_CHILD, NULL, 0, 0, 0, 0);
		m_pWndRightTopBar->ShowUI(false);
	}

	if (m_pWndLectureMode == NULL)
	{
		m_pWndLectureMode = new CWndLectureMode;
		m_pWndLectureMode->Create(m_hWnd, L"LectureMode", UI_WNDSTYLE_CHILD, NULL, 0, 0, 0, 0);
		m_pWndLectureMode->ShowWindow(false);
	}

	if (m_pDlgPrivateChat == NULL)
	{
		m_pDlgPrivateChat = new CDlgPrivateChat;
		m_pDlgPrivateChat->Create(NULL, L"PrivateChat", WS_POPUP | WS_VISIBLE, NULL, 0, 0, 0, 0);
		m_pDlgPrivateChat->CenterWindow();
		m_pDlgPrivateChat->ShowWindow(false);
	}

	if (m_pDlgDocReader == NULL)
	{
		m_pDlgDocReader = new CDlgDocReader(m_hWnd);
		m_pDlgDocReader->Create(NULL, L"Reader", UI_WNDSTYLE_DIALOG, NULL, 0, 0, 0, 0);
		m_pDlgDocReader->CenterWindow();
		m_pDlgDocReader->ShowWindow(false);
	}

	if (m_pDlgShareDock == NULL)
	{
		m_pDlgShareDock = new CDlgShareDock(m_hWnd);
		m_pDlgShareDock->Create(NULL, L"ShareDock", NULL, UI_WNDSTYLE_EX_DIALOG);
		m_pDlgShareDock->ShowWindow(false);
	}

	if (m_pDlgSharing == NULL)
	{
		m_pDlgSharing = new CDlgSharing(m_hWnd);
		m_pDlgSharing->Create(NULL, _T("Sharing"), UI_WNDSTYLE_DIALOG | WS_EX_TOPMOST, NULL, 0, 0, 0, 0);
		m_pDlgSharing->CenterWindow();
		m_pDlgSharing->ShowUI(SHARE_NULL, false);
	}

	if (m_pDlgParticipants == NULL)
	{
		m_pDlgParticipants = new CDlgParticipants(m_hWnd);
		m_pDlgParticipants->Create(NULL, _T("Participants"), UI_WNDSTYLE_DIALOG | WS_EX_TOPMOST, NULL, 0, 0, 0, 0);
		m_pDlgParticipants->CenterWindow();
		m_pDlgParticipants->ShowWindow(false);
	}

	if (m_pMsgRoomExit == NULL)
	{
		m_pMsgRoomExit = new CMsgRoomExit(m_hWnd);
		m_pMsgRoomExit->Create(NULL, _T("MsgRoomExit"), WS_POPUP | WS_VISIBLE, NULL, 0, 0, 0, 0);
		m_pMsgRoomExit->CenterWindow();
		m_pMsgRoomExit->ShowWindow(false);
	}
	
	if (m_pWndUploadProcessing == NULL)
	{
		m_pWndUploadProcessing = new CWndUploadProcessing;
		m_pWndUploadProcessing->Create(NULL, L"Processing", WS_POPUP | WS_VISIBLE, NULL, 0, 0, 0, 0);
		m_pWndUploadProcessing->CenterWindow();
		m_pWndUploadProcessing->ShowWindow(false);
	}
}

LRESULT CDlgConference::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	case WM_TOOLBAR:	   lRes = OnToolBarMsg(uMsg, wParam, lParam, bHandled); break;
	case WM_END_CONFERENCE:		lRes = OnEndConference(uMsg, wParam, lParam, bHandled); break;
	case WM_EXIT_CONFERENCE:	lRes = OnExitConference(uMsg, wParam, lParam, bHandled); break;
	case WM_CHAT_REPLY:	   lRes = OnFastReply(uMsg, wParam, lParam, bHandled); break;
	case WM_MODERATOR_CHANGED:	lRes = OnModeratorChanged(uMsg, wParam, lParam, bHandled); break;
	case WM_SHARE:		   lRes = OnShowShareWindow(uMsg, wParam, lParam, bHandled); break;
	case WM_SHARE_PROCESS:	lRes = OnShareProcessing(uMsg, wParam, lParam, bHandled); break;
	case WM_SHARE_SCREEN:	lRes = OnShareScreen(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}
	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;
	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CDlgConference::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	/*if (wParam == SC_MINIMIZE)
	{
	ShowWindow(false);
	bHandled = TRUE;
	return 0;
	}
	else */if (wParam == SC_MAXIMIZE)
	{
		POINT pt = {0,0};
		ClientToScreen(m_hWnd, &pt);
		GetClientRect(m_hWnd, &m_oldRect);
		m_oldRect.left = m_oldRect.left + pt.x;
		m_oldRect.right = m_oldRect.right + pt.x;
		m_oldRect.top = m_oldRect.top + pt.y;
		m_oldRect.bottom = m_oldRect.bottom + pt.y;
		SetWindowPos(m_hWnd, NULL, 0, -30, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)+30, SWP_SHOWWINDOW);
		g_tBtnStatus.bFullScreen = true;

		if (g_lectureMode != VIDYO_CLIENT_LECTURE_MODE_STOP)
		{
			UpdateLectureUI();
		}

		bHandled = TRUE;
		return 0;
	}
	else if (wParam == SC_RESTORE)
	{
		if (g_tBtnStatus.bFullScreen) 
		{
			SetWindowPos(m_hWnd, NULL, m_oldRect.left, m_oldRect.top, m_oldRect.right-m_oldRect.left, m_oldRect.bottom-m_oldRect.top, SWP_SHOWWINDOW);
			g_tBtnStatus.bFullScreen = false;

			if (g_lectureMode != VIDYO_CLIENT_LECTURE_MODE_STOP)
			{
				UpdateLectureUI();
			}
		}
		ShowWindow();
		bHandled = TRUE;
		return 0;
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LeaveConference();

	bHandled = TRUE;
	return 0;
}

LRESULT CDlgConference::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	VidyoClientInEventResize inEventResize = {0};
	inEventResize.x = 0;

	if (g_sRoomCode.compare(0, 4, "SKEX") == 0)
	{
		CVerticalLayoutUI* pVerMain = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verMain"));
		CDuiString sTemp;
		int iLeft = (2436-rc.right)/2;
		sTemp.Format(L"file='DlgConference/bgCaption.png' source='%d,0,%d,110' ytiled='true'", iLeft, iLeft+rc.right);
		pVerMain->SetAttribute(L"bkimage", sTemp);

		inEventResize.y = 110;
	}
	else if (g_sRoomCode.compare(0, 4, "DBUP") == 0)
	{
		CVerticalLayoutUI* pVerMain = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verMain"));
		CDuiString sTemp;
		sTemp.Format(L"file='DlgConference/bgCaptionDBUP.png' source='0,0,%d,110' ytiled='true'", rc.right);
		pVerMain->SetAttribute(L"bkimage", sTemp);

		inEventResize.y = 110;
	}
	else
		inEventResize.y = 30;

	inEventResize.width = (VidyoUint)rc.right;
	inEventResize.height = (VidyoUint)(rc.bottom - inEventResize.y);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_RESIZE, &inEventResize, sizeof(inEventResize));

	VidyoClientRequestSetBackground reqBackground = {0};
	reqBackground.willBackground = TRUE;
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_SET_BACKGROUND, &reqBackground, sizeof(reqBackground));

	SIZE szRoundCorner = m_pm.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
	if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
		CDuiRect rcWnd;
		::GetWindowRect(*this, &rcWnd);
		rcWnd.Offset(-rcWnd.left, -rcWnd.top);
		rcWnd.right++; rcWnd.bottom++;
		HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
		::SetWindowRgn(*this, hRgn, TRUE);
		::DeleteObject(hRgn);
	}
#endif

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (wParam == TIMER_TOOLBAR)
	{
		if(m_pWndToolbar)
		{
			if (!m_pWndToolbar->IsSetUIShow() && g_lectureMode != VIDYO_CLIENT_LECTURE_MODE_LISTEN)
			{
				m_pWndLectureMode->ShowWindow(false);
				m_pWndToolbar->ShowUI(FALSE);
				KillTimer(m_hWnd, TIMER_TOOLBAR);
			}
		}
		if (m_pWndRightTopBar)
		{
			if (g_lectureMode != VIDYO_CLIENT_LECTURE_MODE_LISTEN)
			{
				m_pWndRightTopBar->ShowUI(false);
			}
		}
	}
	else if (wParam == TIMER_SHARE)
	{
		if (g_tBtnStatus.bShareSpeaker)
		{
			CDuiString sUrl;
			sUrl.Format(L"@@@@&&&&&&&&@@@@%s", m_sRoomUrl);
			SendChatMsg(sUrl);
		}
		else
		{
			m_iShareTimerCount++;
			if (m_iShareTimerCount-m_iShareRevCount > 3)
			{
				StopShare();
			}
		}
	}
	else if (wParam == TIMER_SHARING)
	{
		g_bSharing = false;
		KillTimer(m_hWnd, TIMER_SHARING);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CDlgConference::OnModeratorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}

void CDlgConference::InitUI()
{
	g_bExiting = false;
	g_bSharing = true;
	SetTimer(m_hWnd, TIMER_SHARING, 3000, NULL);

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	CVerticalLayoutUI* pVerMain = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verMain"));
	VidyoClientInEventResize inEventResize = {0};
	if (g_sRoomCode.compare(0, 4, "SKEX") == 0)
	{
		CDuiString sTemp;
		sTemp.Format(L"file='DlgConference/bgCaption.png' source='%d,0,%d,110' ytiled='true'", (2436-rc.right)/2, (2436+rc.right)/2);
		
		pVerMain->SetAttribute(L"bkimage", sTemp);

		inEventResize.x = 0;
		inEventResize.y = 110;
		inEventResize.height = rc.bottom - 110;
		inEventResize.width = rc.right;
	}
	else if (g_sRoomCode.compare(0, 4, "DBUP") == 0)
	{
		CDuiString sTemp;
		sTemp.Format(L"file='DlgConference/bgCaptionDBUP.png' source='0,0,%d,110' ytiled='true'", rc.right);

		pVerMain->SetAttribute(L"bkimage", sTemp);

		inEventResize.x = 0;
		inEventResize.y = 110;
		inEventResize.height = rc.bottom - 110;
		inEventResize.width = rc.right;
	}
	else
	{
		pVerMain->SetAttribute(L"bkimage", L"");

		inEventResize.x = 0;
		inEventResize.y = 30;
		inEventResize.height = rc.bottom - 30;
		inEventResize.width = rc.right;
	}
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_RESIZE, &inEventResize, sizeof(inEventResize));

	CLabelUI* pLabCaption = static_cast<CLabelUI*>(m_pm.FindControl(_T("labCaption")));
	if (pLabCaption)
	{
		TCHAR szText[MAX_PATH], szTemp[MAX_PATH];
		ChangUtf8ToText(g_sRoomCode.c_str(), szText);
		project_language_loadstring(_T("CONFERENCE"), _T("labCaption"), szTemp, MAX_PATH);
		CDuiString sCaption;
		sCaption.Format(L"%s%s", szTemp, szText);
		pLabCaption->SetText(sCaption);
	}

	VidyoClientInEventPreview inEventPreview;
	if (VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_PREVIEW_MODE , &inEventPreview, sizeof(inEventPreview)) == VIDYO_FALSE)
	{
		if (inEventPreview.previewMode != VIDYO_CLIENT_PREVIEW_MODE_NONE)
		{
			inEventPreview.previewMode = VIDYO_CLIENT_PREVIEW_MODE_DOCK;
			VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_PREVIEW, &inEventPreview, sizeof(inEventPreview));
		}
	}

	UpdateUI();

	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	g_StartTime.wHour = sys.wHour;
	g_StartTime.wMinute = sys.wMinute;
	g_StartTime.wSecond = sys.wSecond;
}

void CDlgConference::ShowUI(bool bShow /* = true */)
{
	ShowWindow(bShow&&!g_tBtnStatus.bShareScreen);
}

void CDlgConference::UpdateUI()
{
	if (m_pWndToolbar != NULL && !g_tBtnStatus.bShare) m_pWndToolbar->ShowUI();

	if (m_pWndRightTopBar && !g_tBtnStatus.bShare) m_pWndRightTopBar->ShowUI();

	if (m_pDlgDocReader && g_tBtnStatus.bShare) m_pDlgDocReader->UpdateUI();

	if (m_pDlgShareDock && g_tBtnStatus.bShareScreen) m_pDlgShareDock->UpdateUI();

	SetTimer(m_hWnd, TIMER_TOOLBAR, 5000, NULL);
}

void CDlgConference::UpdateLectureUI()
{
	if (((g_lectureMode==VIDYO_CLIENT_LECTURE_MODE_LISTEN) && m_bHavePresenter) || g_lectureMode == VIDYO_CLIENT_LECTURE_MODE_STOP)
	{
		m_pWndLectureMode->ShowWindow(false);
		return ;
	}

	RECT rc;
	GetClientRect(m_hWnd, &rc);
	if (g_lectureMode == VIDYO_CLIENT_LECTURE_MODE_LISTEN)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, 0, 30, rc.right, rc.bottom-90, NULL);
		m_pWndLectureMode->UpdateUI(1);
	}
	else if (g_lectureMode == VIDYO_CLIENT_LECTURE_MODE_PRESENT)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, (rc.right-480)/2, 30, 480, 45, NULL);
		m_pWndLectureMode->UpdateUI(2);
	}
	else if (g_lectureMode == VIDYO_CLIENT_LECTURE_MODE_ALLOWED_TO_SPEAK)
	{
		SetWindowPos(m_pWndLectureMode->GetHWND(), NULL, (rc.right-480)/2, 30, 480, 45, NULL);
		m_pWndLectureMode->UpdateUI(3);
	}
}

void CDlgConference::ReleaseUI()
{
	if (m_ptEntityData)
	{
		delete m_ptEntityData;
		m_ptEntityData = NULL;
	}

	m_iChatCount = 0;
	g_lectureMode = VIDYO_CLIENT_LECTURE_MODE_STOP;
	m_bHavePresenter = false;

	if (m_pWndToolbar)
	{
		m_pWndToolbar->m_bRaisehand = false;
		m_pWndToolbar->ShowUI(false);
	}
	if (m_pWndRightTopBar) m_pWndRightTopBar->ShowUI(false);
	if (m_pDlgPrivateChat) m_pDlgPrivateChat->ShowUI(false);
	if (m_pDlgSharing) m_pDlgSharing->ShowUI(SHARE_NULL, false);
	if (m_pDlgParticipants) m_pDlgParticipants->ShowWindow(false);
}

void CDlgConference::ClearRaisehand()
{
	if (m_pWndToolbar)
	{
		m_pWndToolbar->m_bRaisehand = false;
		m_pWndToolbar->UpDateUI();
	}
}

void CDlgConference::SetLectureStage(VidyoClientLectureStage lectureMode)
{
	g_lectureMode = lectureMode;

	UpdateLectureUI();
}

void CDlgConference::SetPresenter(bool bHavePresenter)
{
	m_bHavePresenter = bHavePresenter;
	UpdateLectureUI();
}

void CDlgConference::ShareScreen()
{
	VidyoClientRequestGetWindowsAndDesktops getWindwosAndDesktops = {0};
	VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_WINDOWS_AND_DESKTOPS, &getWindwosAndDesktops, sizeof(getWindwosAndDesktops));

	VidyoClientInEventShare inEventShare;
	inEventShare.shareType = VIDYO_CLIENT_CONTENTS_SHARE_TYPE_DESKTOP_WINDOW;
	inEventShare.window = (HWND)1;
	strcpy(inEventShare.desktopId, getWindwosAndDesktops.sysDesktopId[0]);

	if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_SHARE, &inEventShare, sizeof(inEventShare)))
	{
		ShowWindow(false);
		RECT rcDesk, rc;
		GetClientRect(GetDesktopWindow(), &rcDesk);
		GetClientRect(m_pDlgShareDock->GetHWND(), &rc);
		SetWindowPos(m_pDlgShareDock->GetHWND(), HWND_TOPMOST, (rcDesk.right-rc.right)/2, 0, rc.right, rc.bottom, NULL);
		m_pDlgShareDock->ShowWindow();
		g_tBtnStatus.bShareScreen = true;
	}
}

void CDlgConference::ShowShareList(ShareType type)
{
	if (g_bSharing) return ;
	if (m_pDlgSharing)
	{
		if (type != SHARE_RESHARE)
		{
			if (g_sRoomCode.compare(0, 4, "SKEX") == 0 || g_sRoomCode.compare(0, 4, "SZBX") == 0)
			{
				type = SHARE_SKEX;
			}
			else
				type = SHARE_SANOFI;
		}
		//if (::IsWindowVisible(m_pDlgSharing->GetHWND())) m_pDlgSharing->ShowUI(false);
		m_pDlgSharing->ShowUI(type, !::IsWindowVisible(m_pDlgSharing->GetHWND()));
	}
}

void CDlgConference::MinWndWithShare()
{
	::PostMessageW(m_hWnd, WM_SYSCOMMAND, SC_MINIMIZE, NULL);
	::PostMessageW(m_pDlgDocReader->GetHWND(), WM_SYSCOMMAND, SC_MINIMIZE, NULL);
}

void CDlgConference::ShowControlWnd()
{
	if (m_pDlgParticipants)
	{
		if (::IsWindowVisible(m_pDlgParticipants->GetHWND())) m_pDlgParticipants->ShowWindow(false);
		m_pDlgParticipants->UpdataUI();
		m_pDlgParticipants->ShowWindow();
	}
}

void CDlgConference::ShowChatWnd()
{
	UpdateUI();
}

void CDlgConference::TakePicture()
{
	RECT rc;
	::GetClientRect(m_hWnd, &rc);
	rc.top = rc.top + m_pm.GetCaptionRect().bottom;
	rc.right = rc.right;

	TCHAR strText[255] = _T("");
	ReadIniString(_T("SETTING"), _T("PhotoPath"), _T(""), strText, 255);
	if (!wcscmp(strText, L""))
	{
		ReadIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO, strText, 255);
		if (_wmkdir(strText) == 0)
			WriteIniString(_T("SETTING"), _T("PhotoPath"), PATH_PHOTO);
	}
	char szText[255] = {0}, szName[128] = {0};
	ChangTextToUtf8(strText, szText);
	ChangUtf8ToGBK(m_ptEntityData->displayName, szName);

	char szPath[512] = {0};
	SYSTEMTIME sys; 
	GetLocalTime( &sys ); 
	sprintf(szPath, "%s\\%s_%d-%02d-%02d_%02d-%02d-%02d.bmp", szText, szName,
		sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond);
	if (m_pWndToolbar) m_pWndToolbar->ShowUI(false);
	Sleep(100);
	SaveWindowCopy(NULL, szPath, m_pm.GetCaptionRect().bottom, rc.right-rc.left, rc.bottom-rc.top, m_hWnd);
	Sleep(100);
	if (m_pWndToolbar) m_pWndToolbar->ShowUI(true);
}

LRESULT CDlgConference::OnToolBarMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ToolBarMsg msg = (ToolBarMsg)wParam;
	switch (msg)
	{
	case TB_MSG_MIN:
		MinWndWithShare();
		break;
	case TB_MSG_SHARE:
		{
			ShareType type = (ShareType)lParam;
			ShowShareList(type);
		}
		break;
	case TB_MSG_CONTROL:
		ShowControlWnd();
		break;
	case TB_MSG_CHAT:
		ShowChatWnd();
		break;
	case TB_MSG_PHOTO:
		TakePicture();
		break;
	case TB_MSG_EXIT:
		LeaveConference();
		break;
	}
	return 0;
}

LRESULT CDlgConference::OnEndConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//SoapDisconnectConferenceAll(m_szConferenceID, "");
	return 0;
}

LRESULT CDlgConference::OnExitConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	g_bExiting = true;
	if (g_tBtnStatus.bShare)
	{
		if (g_tBtnStatus.bShareSpeaker)
		{
			SendChatMsg(L"&&&&@@@@@@@@&&&&");
		}

		StopShare();
	}

	if (g_tBtnStatus.bShareScreen)
	{
		if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_UNSHARE, NULL, NULL))
		{
			g_tBtnStatus.bShareScreen = false;
			m_pDlgShareDock->ShowUI(false);
		}
	}

	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_LEAVE, NULL, 0);

	return 0;
}

LRESULT CDlgConference::OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CDuiString strTemp;
	TCHAR szBuf[MAX_PATH] = {0};
	strTemp.Format(L"element%d", wParam+1);
	project_language_loadstring(_T("CHATREPLY"), strTemp, szBuf, MAX_PATH);

	ShowMsg(szBuf, L"ÎÒ");

	VidyoClientInEventGroupChat groupChat = {0};
	ChangTextToUtf8(szBuf, (char*)&groupChat);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_GROUP_CHAT, &groupChat, sizeof(groupChat));

	return 0;
}

LRESULT CDlgConference::OnShowShareWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (g_tBtnStatus.bShareScreen)
	{
		if (VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_UNSHARE, NULL, NULL))
		{
			m_pDlgShareDock->ShowUI(false);
			g_tBtnStatus.bShareScreen = false;
			ShowWindow();
			return 0;
		}
	}

	if (g_tBtnStatus.bShare)
	{
		StopShare();
		SendChatMsg(L"&&&&@@@@@@@@&&&&");
	}

	g_tBtnStatus.bShare = (bool)wParam;

	if (g_tBtnStatus.bShare)
	{
		if (m_pDlgDocReader == NULL) return 0;

		StartShare(g_sShareUrl);

		CDuiString strUrl;
		strUrl.Format(L"@@@@&&&&&&&&@@@@%s", g_sShareUrl);
		SendChatMsg(strUrl);
	}

	return 0;
}

LRESULT CDlgConference::OnShareProcessing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bool bProc = (bool)wParam;
	m_pWndUploadProcessing->ShowUI(bProc);
	return 0;
}

LRESULT CDlgConference::OnShareScreen(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShareScreen();
	return 0;
}

void CDlgConference::StartShare(CDuiString sSharUrl, bool bSpeaker/* = true */)
{
	if (m_pDlgDocReader == NULL) return;
	if (m_pWndToolbar)
	{
		m_pWndToolbar->HideAllSetUI();
		m_pWndToolbar->ShowUI(false);
	}

	if (m_pDlgSharing)
	{
		m_pDlgSharing->ShowUI(SHARE_NULL, false);
	}

	if (m_pWndUploadProcessing)
	{
		m_pWndUploadProcessing->ShowWindow(false);
	}

	g_tBtnStatus.bShareSpeaker = bSpeaker;
	g_tBtnStatus.bShare = true;

	m_sRoomUrl = sSharUrl;
	
	m_pDlgDocReader->SetDocUrl(sSharUrl, bSpeaker);
	m_pDlgDocReader->ShowWindow();
	CVerticalLayoutUI* verShareBtn = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verShareBtn"));
	CVerticalLayoutUI* verUnshareBtn = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verUnshareBtn"));
	verShareBtn->SetVisible();
	verUnshareBtn->SetVisible(false);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 320, 240, NULL);

	SetTimer(m_hWnd, TIMER_SHARE, 3000, NULL);
}

void CDlgConference::StopShare()
{
	g_tBtnStatus.bShareSpeaker = false;
	g_tBtnStatus.bShare = false;
	g_tBtnStatus.bFullScreen = false;

	if (m_pDlgDocReader) m_pDlgDocReader->ShowWindow(false);
	CVerticalLayoutUI* verShareBtn = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verShareBtn"));
	CVerticalLayoutUI* verUnshareBtn = static_cast<CVerticalLayoutUI*>(m_pm.FindControl(L"verUnshareBtn"));
	verShareBtn->SetVisible(false);
	verUnshareBtn->SetVisible();

	ShowWindow();
	SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, 1022, 670, NULL);
	CenterWindow();

	KillTimer(m_hWnd, TIMER_SHARE);
}

void CDlgConference::LeaveConference()
{
	if (m_pMsgRoomExit)
	{
		m_pMsgRoomExit->ShowUI((g_sRoomCode.compare(0, 4, "DBUP") == 0));
		SetForegroundWindow(m_pMsgRoomExit->GetHWND());
	}
}

void CDlgConference::RevGroupChat(VidyoClientOutEventGroupChat* groupChat)
{
	m_iChatCount++;
	m_pWndToolbar->SetNewChatMsgCount(m_iChatCount);

	TCHAR szName[128] = {0};
	ChangUtf8ToText(groupChat->displayName, szName);

	TCHAR szMsg[1025] = {0};
	ChangUtf8ToText(groupChat->message, szMsg);

	CDuiString sMsg(szMsg);
	CDuiString sTemp;
	CDuiString sUrl;
	sTemp = sMsg.Left(16);
	if (sTemp.CompareNoCase(L"@@@@&&&&&&&&@@@@") == 0 && !g_bExiting)
	{
		sUrl = sMsg.Mid(16);

		if (!g_tBtnStatus.bShare)
		{
			m_iShareRevCount = 0;
			m_iShareTimerCount = 0;
			StartShare(sUrl, false);
		}
		else
		{
			m_iShareRevCount++;
		}
	}
	else if (sTemp.CompareNoCase(L"&&&&@@@@@@@@&&&&") == 0)
	{
		StopShare();
	}

	//ShowMsg(szMsg, szName);
}

void CDlgConference::RevPrivateChat(VidyoClientOutEventPrivateChat* priviteChat)
{
	m_iChatCount++;
	m_pWndToolbar->SetNewChatMsgCount(m_iChatCount);

	if (m_pDlgPrivateChat)
	{
		TCHAR szName[128], szURI[257], szMsg[1025];
		ChangUtf8ToText(priviteChat->displayName, szName);
		ChangUtf8ToText(priviteChat->message, szMsg);
		ChangUtf8ToText(priviteChat->uri, szURI);
		m_pDlgPrivateChat->AddUser(szName, szURI);
		m_pDlgPrivateChat->RevPrivateChat(szMsg, szName);
		
		//m_pDlgPrivateChat->ShowUI();
	}
}

void CDlgConference::ShowMsg(LPCTSTR strMsg, LPCTSTR strSender)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_pm.FindControl(L"richGroupChatView"));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;
	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	if (!pRichEdit->GetText().IsEmpty())
	{
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
		pRichEdit->SetSel(-1, -1);
		pRichEdit->ReplaceSel(_T("\n"), false);
	}

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	pRichEdit->ReplaceSel(strSender, false);

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	SYSTEMTIME time;
	::GetLocalTime(&time);
	CDuiString strTime;
	strTime.Format(L":\t(%u/%u/%u %u:%u:%u)\n", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	pRichEdit->ReplaceSel(strTime, false);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0xcc, 0xCC, 0xCC);
	cf.dwEffects = 0;
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strMsg, false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();
}

void CDlgConference::SendChatMsg(CDuiString sText)
{
	VidyoClientInEventGroupChat groupChat = {0};
	ChangTextToUtf8(sText.GetData(), (char*)&groupChat);
	VidyoClientSendEvent(VIDYO_CLIENT_IN_EVENT_GROUP_CHAT, &groupChat, sizeof(groupChat));
}

void CDlgConference::OnSelectChanged( TNotifyUI &msg )
{
	if (msg.pSender->GetName() == L"optSearch")
	{
		if (((COptionUI*)(msg.pSender))->IsSelected())
		{
			CEditUI* pEdit = static_cast<CEditUI*>(m_pm.FindControl(L"editSearch"));
			CListUI* pParList = static_cast<CListUI*>(m_pm.FindControl(L"listParticipants"));

			CDuiString strSearch = pEdit->GetText();
			TCHAR szName[256] = {0};
			for (unsigned int i = 0; i < g_reqParticipants.numberParticipants; i++)
			{
				ChangUtf8ToText(g_reqParticipants.name[i], szName);
				if (!strSearch.Compare(szName))
				{
					pParList->SelectItem(i);
				}
			}
		}
		return;
	}
}

void CDlgConference::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		if( msg.pSender->GetName() == _T("btnClose") ) 
		{
			LeaveConference();
		}
		else if ( msg.pSender->GetName() == _T("btnMin") )
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		}
		else if ( msg.pSender->GetName() == _T("btnMax") )
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		}
		else if ( msg.pSender->GetName() == L"btnShareMin" )
		{
			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, 320, 30, NULL);
		}
		else if ( msg.pSender->GetName() == L"btnShareNormal" )
		{
			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, 320, 240, NULL);
		}
		else if ( msg.pSender->GetName() == L"btnShareMax" )
		{
			RECT rc;
			GetWindowRect(m_hWnd, &rc);
			SetWindowPos(m_hWnd, HWND_TOPMOST, rc.left, rc.top, 640, 480, NULL);
		}
		else if ( msg.pSender->GetName() == L"btnSend" )
		{
			CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_pm.FindControl(L"richGroupChatInput"));
			CDuiString strInput = pRichEdit->GetText();
			if (strInput.IsEmpty())
			{
				CWndFastReply* pPopWnd = new CWndFastReply();
				pPopWnd->Create(m_hWnd, _T("FastReply"), WS_POPUP | WS_VISIBLE, WS_EX_TOOLWINDOW, 0, 0, 0, 0);

				RECT rc, rcClient;
				::GetClientRect(m_hWnd, &rc);
				::GetClientRect(pPopWnd->GetHWND(), &rcClient);
				POINT pt = msg.ptMouse;
				ClientToScreen(m_hWnd, &pt);
				SetWindowPos(pPopWnd->GetHWND(), NULL, 
					rc.right-rcClient.right+pt.x-msg.ptMouse.x, 
					rc.bottom-rcClient.bottom+pt.y-msg.ptMouse.y-34,
					rcClient.right, 
					rcClient.bottom, NULL);
				pPopWnd->ShowModal();
			}
			else
				SendChatMsg(L"");
		}
	}
	else if(msg.sType == _T("selectchanged"))
	{
		OnSelectChanged(msg);
	}
	else if ( msg.sType == L"itemactivate" )
	{
		CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("listParticipants")));
		if (m_pDlgPrivateChat)
		{
			TCHAR szName[64] = {0};
			TCHAR szURI[257] = {0};
			ChangUtf8ToText(g_reqParticipants.name[pList->GetCurSel()], szName);
			ChangUtf8ToText(g_reqParticipants.URI[pList->GetCurSel()], szURI);
			m_pDlgPrivateChat->AddUser(szName, szURI);
			m_pDlgPrivateChat->UpdataUI();
			m_pDlgPrivateChat->ShowWindow();
			SetForegroundWindow(m_pDlgPrivateChat->GetHWND());
		}
	}
}

