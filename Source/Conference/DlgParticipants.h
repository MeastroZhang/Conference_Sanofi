#pragma once
class CDlgParticipants : public WindowImplBase
{
public:
	CDlgParticipants(HWND hParent = NULL);
	~CDlgParticipants(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_PARTICIPANTS"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("Participants"); };
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

	HWND m_hParent;

public:
	void UpdataUI();
};

