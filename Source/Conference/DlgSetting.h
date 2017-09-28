#pragma once

class CDlgSetting : public WindowImplBase
{
public:
	CDlgSetting(HWND hParent = NULL);
	~CDlgSetting(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_SETTING"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Setting"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual	void Notify(TNotifyUI& msg);
	void OnSelectChanged( TNotifyUI &msg );

	HWND m_hParent;
public:
	void InitUI();

private:
	COptionUI*	m_pOptCommon;
	COptionUI*	m_pOptDevice;
	COptionUI*	m_pOptUpdate;
	COptionUI*	m_pOptCommit;

	CTabLayoutUI*	m_pTabSetting;
};

