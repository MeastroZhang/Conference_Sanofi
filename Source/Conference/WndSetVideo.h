#pragma once
class CWndSetVideo : public WindowImplBase
{
public:
	CWndSetVideo(void);
	~CWndSetVideo(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_SET_VIDEO"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("VideoSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();
};

