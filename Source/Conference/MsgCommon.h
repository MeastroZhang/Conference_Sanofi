#pragma once

#include "../../include/Util/Util.h"

class MsgCommon : public WindowImplBase
{
public:
	MsgCommon(eMsgWndType msgType);
	~MsgCommon(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_MSG_COMMON"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("MsgCommon"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	eMsgWndType m_eMsgType;
};

