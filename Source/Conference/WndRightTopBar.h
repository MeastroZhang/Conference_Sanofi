#pragma once

#include "../../include/Util/Util.h"

class CWndRightTopBar : public WindowImplBase
{
public:
	CWndRightTopBar(void);
	~CWndRightTopBar(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_RIGHT_TOPBAR"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndRightTopbar"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual	void Notify(TNotifyUI& msg);

public:
	void ShowUI(bool bShow = true);
};

