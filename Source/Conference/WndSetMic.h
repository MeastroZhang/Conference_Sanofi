#pragma once

class CWndSetMic : public WindowImplBase
{
public:
	CWndSetMic(void);
	~CWndSetMic(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_SET_MIC"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MicSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();
};

