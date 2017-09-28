#pragma once

class CDlgShareLocal : public WindowImplBase
{
public:
	CDlgShareLocal(HWND hParent = NULL);
	~CDlgShareLocal(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_SHARE_LOCAL"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Sharing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	HWND m_hParent;

public:
	void ShowUI(bool bShow = true){return ;};

private:
	void UpdataData(){return ;};
	void UpdataUI();
	void GetShareFiles(){return ;};
	void SearchShareFiles(){return ;};
};

