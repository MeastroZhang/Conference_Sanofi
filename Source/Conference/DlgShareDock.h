#pragma once

#include "../../include/Util/Util.h"
#include "WndReaderToolBar.h"

class CDlgShareDock : public WindowImplBase
{
public:
	CDlgShareDock(HWND hParent = NULL);
	~CDlgShareDock(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_SHARE_DOCK"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("DlgShareDock"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

private:
	HWND m_hParent;
	CWndReaderToolBar* m_pWndReaderBar;

public:
	void ShowUI(bool bShow = true);
	void UpdateUI();
};

