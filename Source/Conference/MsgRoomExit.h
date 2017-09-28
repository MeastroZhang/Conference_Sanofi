#pragma once
class CMsgRoomExit : public WindowImplBase
{
public:
	CMsgRoomExit(HWND hParent);
	~CMsgRoomExit(void);

	HWND m_hParent;

	void ShowUI(bool bDBUP, bool bShow = true);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_MSG_ROOM_EXIT"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgRoomExit"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);

	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	int m_iCount;
};

