#pragma once

#include "stdafx.h"
#include "WndToolbar.h"
#include "WndRightTopBar.h"
#include "WndControl.h"
#include "DlgSharing.h"
#include "MsgRoomExit.h"
#include "MsgModeratorExit.h"
#include "MsgApplyForbid.h"
#include "WndInputPin.h"
#include "WndFastReply.h"
#include "DlgPrivateChat.h"
#include "WndLectureMode.h"
#include "DlgDocReader.h"
#include "DlgShareDock.h"
#include "DlgParticipants.h"
#include "WndUploadProcessing.h"
#include "../../include/Util/Util.h"

class CDlgConference : public WindowImplBase
{
public:
	CDlgConference(HWND pParent);
	~CDlgConference();

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_CONFERENCE"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Conference"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);
	void OnSelectChanged( TNotifyUI &msg );

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnModeratorChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnToolBarMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEndConference(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFastReply(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowShareWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShareProcessing(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShareScreen(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void StartShare(CDuiString sSharUrl, bool bSpeaker = true);
	void StopShare();

	HWND		m_pParent;

	struct _tStartTime
	{
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
	} tStartTime;

	VidyoClientAccountData* m_ptEntityData;
public:
	void InitUI();
	void ShowUI(bool bShow = true);
	void ReleaseUI();
	void UpdateUI();
	void UpdateLectureUI();
	void ClearRaisehand();
	
	void BtnStatusChanged();

	void ShareScreen();
	void ShowShareList(ShareType type);
	void SetLectureStage(VidyoClientLectureStage lectureMode);
	void SetPresenter(bool bHavePresenter);

	void MinWndWithShare();
	void ShowControlWnd();
	void ShowChatWnd();
	void TakePicture();
	void LeaveConference();

	CWndToolbar*			m_pWndToolbar;
	CWndRightTopBar*		m_pWndRightTopBar;
	CWndLectureMode*		m_pWndLectureMode;
	CDlgPrivateChat*		m_pDlgPrivateChat;
	CDlgDocReader*			m_pDlgDocReader;
	CDlgShareDock*			m_pDlgShareDock;
	CDlgSharing*			m_pDlgSharing;
	CDlgParticipants*		m_pDlgParticipants;
	CMsgRoomExit*			m_pMsgRoomExit;
	CWndUploadProcessing*	m_pWndUploadProcessing;

	bool					m_bShowToolBar;		// Show ToolBar Timer
	bool					m_bHavePresenter;
	int m_iShareTimerCount;
	int m_iShareRevCount;
	CDuiString m_sRoomUrl;

	void SendChatMsg(CDuiString sText);
	void RevGroupChat(VidyoClientOutEventGroupChat* groupChat);
	void RevPrivateChat(VidyoClientOutEventPrivateChat* priviteChat);
	void ShowMsg(LPCTSTR strMsg, LPCTSTR strSender);
private:
	int		m_iChatCount;
	char	m_szConferenceID[64];
	RECT	m_oldRect;
};

