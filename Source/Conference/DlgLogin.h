#pragma once

#include "DlgSetting.h"
#include "resource.h"
#include <ShellAPI.h>

class CDlgLogin : public WindowImplBase
{
public:
	CDlgLogin(HWND hParent = NULL);
	~CDlgLogin();

protected:
	virtual CDuiString GetSkinFile(){return _T ("XML_LOGIN");};
	virtual LPCTSTR GetWindowClassName() const{return _T("Login");};
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual	void Notify(TNotifyUI& msg);

public:
	HWND m_hParent;

	bool		m_bLogin;

	CButtonUI*	m_pBtnJoin;

	void Login();
	void LoginErrorTips(unsigned int errorType);
	void InitUI();

	void ShowRoomInfo();
	void JoinRoom();

	bool m_bTrayLogout;
	void SetTrayLogout(bool bTrayLogout = true);
};

