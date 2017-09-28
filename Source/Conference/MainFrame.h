#pragma once

#include <vector>
#include "../../include/Util/Util.h"
#include "DlgConference.h"
#include "DlgLogin.h"
#include "WndProcessing.h"
#include "MenuTray.h"
#include "resource.h"

#include "WkeWebkit.h"

class MainFrame : public WindowImplBase
{
public:
	MainFrame(void);
	~MainFrame(void);

protected:
	virtual CDuiString	GetSkinFile()				{ return _T ("XML_MAIN"); };
	virtual LPCTSTR		GetWindowClassName() const	{ return _T ("FrameWnd"); };
	virtual void		InitWindow();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	
private:
	CDlgConference*		m_pDlgConference;
	CDlgLogin*			m_pDlgLogin;
	CWndProcessing*		m_pWndProcessing;

	// UI界面显示调用栈
	vector<eModeType>	m_ModeTypeStack;		// 存放显示的模块堆栈
	int					m_iConferenceUICount;
	int					m_iLoginUICount;
	int					m_iContactsUICount;

	// 保存鼠标位置
	int					m_iMouseXPos;
	int					m_iMouseYPos;

	CTrayIcon			m_trayIcon;
	CMenuWnd*			m_pMenu;
private: 
	void Push(eModeType eType);					// 压栈
	void Pop(eModeType eType);					// 出栈
	void ShowUI();								// 显示模块界面
	void HideAllUI();							// 隐藏所有界面
	void ClearStack();							// 清空栈
	void DestroyAllUI();						// 释放所有的界面

	void CreateConferenceUI();
	void CreateLoginUI();
	void CreateGuestUI();
	void CreateContactsUI();
	void CreateProcessingWnd();

	BOOL ProCmdline();
	
	//当有新的状态产生时，通知相应窗口
	void NotifyWindow();
public:
	LRESULT OnVidyoMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnExitUI(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnShowProcess(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void InitVidyoSystem();
	void ReleaseVidyoSystem();
	void TrayLogout();
	void TrayExitConf();
};