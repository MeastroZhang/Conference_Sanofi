#pragma once

class CWndReaderToolBar : public WindowImplBase
{
public:
	CWndReaderToolBar(HWND hParent = NULL);
	~CWndReaderToolBar(void);

	virtual CDuiString GetSkinFile(){return _T ("XML_WND_READER_TOOLBAR");};
	virtual LPCTSTR GetWindowClassName() const{return _T("ReaderToolBar");};
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void UpdateUI();
	void HideUI(bool bHide = true);
	void CalculateClock();
private:
	HWND m_hParent;
	bool m_bTimer;

	CButtonUI*	m_pBtnMic;
	CButtonUI*	m_pBtnMicClosed;
	CButtonUI*	m_pBtnViedo;
	CButtonUI*	m_pBtnVideoClosed;
	CButtonUI*	m_pBtnParticipants;
	CButtonUI*	m_pBtnShare;
	CButtonUI*	m_pBtnReShare;
	CButtonUI*	m_pBtnStopShare;
	//CButtonUI*	m_pBtnMore;
};

