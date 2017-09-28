#pragma once

#include "WndSetPhoto.h"
#include "WndSetVideo.h"
#include "WndSetMic.h"
#include "WndSetSpeaker.h"
#include "../../include/Util/Util.h"

class CWndToolbar : public WindowImplBase
{
public:
	CWndToolbar(HWND hParent = NULL);
	~CWndToolbar(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_TOOLBAR"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Toolbar"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	HWND m_hParent;
	bool m_bTimer;

	CWndSetPhoto*	m_pWndSetPhoto;
	CWndSetVideo*	m_pWndSetVideo;
	CWndSetMic*		m_pWndSetMic;
	CWndSetSpeaker*	m_pWndSetSpeaker;
public:
	void UpDateUI();
	void CalculateClock();
	void SetNewChatMsgCount(int iCount);
	void ShowUI(bool bShow = true, bool bShowChat = true);

	typedef enum SettingBtnType_
	{
		BTN_NONE,
		BTN_PHOTO,
		BTN_VIDEO,
		BTN_MIC,
		BTN_SPEAKER,
	} SettingBtnType;
	void ShowSettingUI(POINT ptMouse, SettingBtnType btnType = BTN_NONE);
	void HideAllSetUI();

	BOOL m_bSetUIShow;
	BOOL IsSetUIShow();

	bool	m_bRaisehand;
private:
	CButtonUI*	m_pBtnMic;
	CButtonUI*	m_pBtnMicClosed;
	CButtonUI*	m_pBtnSpeaker;
	CButtonUI*	m_pBtnSpeakerClosed;
	CButtonUI*	m_pBtnViedo;
	CButtonUI*	m_pBtnVideoClosed;
	CButtonUI*	m_pBtnParticipants;
	CButtonUI*	m_pBtnShare;
	CButtonUI*	m_pBtnStopShare;
};

