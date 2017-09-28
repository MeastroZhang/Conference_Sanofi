#pragma once
class CMenuTray : public WindowImplBase
{
public:
	CMenuTray(void);
	~CMenuTray(void);

protected:
	virtual CDuiString GetSkinFile(){return _T ("XML_TRAY");};
	virtual LPCTSTR GetWindowClassName() const{return _T("MenuTray");};
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
};

