#pragma once
class CWndLectureMode : public WindowImplBase
{
public:
	CWndLectureMode(void);
	~CWndLectureMode(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_LECTURE_MODE"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndLectureMode"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
public:
	void UpdateUI(BYTE bFlag);
};

