#pragma once
class CWndFastReply : public WindowImplBase
{
public:
	CWndFastReply(void);
	~CWndFastReply(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_FAST_REPLY"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndFastReply"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

