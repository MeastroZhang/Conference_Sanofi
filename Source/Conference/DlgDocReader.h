#pragma once

#include "../../include/Util/Util.h"
#include "WkeWebkit.h"
#include "WndReaderToolBar.h"

class CDlgDocReader : public WindowImplBase
{
public:
	CDlgDocReader(HWND hParent = NULL);
	~CDlgDocReader(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_DOCREADER"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("DlgDocReader"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	HWND m_hParent;
	CWkeWebkitUI* m_pWke;
	CWndReaderToolBar* m_pWndReaderBar;

public:
	void UpdateUI();
	void SetDocUrl(CDuiString szUrl, bool bSpeaker);
};

