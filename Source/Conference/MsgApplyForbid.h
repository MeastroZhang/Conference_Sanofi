#pragma once
class CMsgApplyForbid : public WindowImplBase
{
public:
	CMsgApplyForbid(void);
	~CMsgApplyForbid(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_MSG_APPLY_FORBID"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgApplyForbid"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

