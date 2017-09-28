#pragma once
class CWndSetPhoto : public WindowImplBase
{
public:
	CWndSetPhoto(void);
	~CWndSetPhoto(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_SET_PHOTO"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("PhotoSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

