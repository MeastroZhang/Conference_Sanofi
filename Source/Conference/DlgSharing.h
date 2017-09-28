#pragma once
#include "DataDefine.h"

class CDlgSharing : public WindowImplBase
{
public:
	CDlgSharing(HWND hParent = NULL);
	~CDlgSharing(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_SHARE"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Sharing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	HWND m_hParent;

public:
	void ShowUI(ShareType type, bool bShow = true);

private:
	void UpdateData();
	void UpdateUI(ShareType type);
	void GetShareFiles();
	void SearchShareFiles();
};

