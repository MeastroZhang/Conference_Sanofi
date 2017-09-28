#pragma once
class CWndProcessing : public WindowImplBase
{
public:
	CWndProcessing(void);
	~CWndProcessing(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_PROCESSING"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndProcessing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	void InitUI();
};

