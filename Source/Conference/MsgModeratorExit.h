#pragma once
class CMsgModeratorExit : public WindowImplBase
{
public:
	CMsgModeratorExit(void);
	~CMsgModeratorExit(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_MSG_MODERATOR_EXIT"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgModeratorExit"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

