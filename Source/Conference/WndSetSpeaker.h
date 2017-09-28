#pragma once

class CWndSetSpeaker : public WindowImplBase
{
public:
	CWndSetSpeaker(void);
	~CWndSetSpeaker(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_SET_SPEAKER"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("SpeakerSetting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void UpdataUI();
};

