#pragma once
class CWndUploadProcessing : public WindowImplBase
{
public:
	CWndUploadProcessing(void);
	~CWndUploadProcessing(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_UPLOAD_PRO"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("UploadProcessing"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);

public:
	void ShowUI(bool bProc);
};

