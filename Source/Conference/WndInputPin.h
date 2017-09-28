#pragma once

#include "../../include/Util/Util.h"

class CWndInputPin : public WindowImplBase
{
public:
	CWndInputPin(eInputWndType wndType, char* ConferenceID);
	~CWndInputPin(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_INPUT_PIN"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndInputPin"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	char* m_pRoomID;
	eInputWndType	m_eWndType;
};

