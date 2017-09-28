#pragma once
class CWndSetPhotoPath : public WindowImplBase
{
public:
	CWndSetPhotoPath(void);
	~CWndSetPhotoPath(void);

protected:
	virtual CDuiString GetSkinFile()  { return _T ("XML_WND_SET_PHOTO_PATH"); };
	virtual LPCTSTR GetWindowClassName(void) const { return _T ("WndSetPhotoPath"); };
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual void InitWindow();
	virtual	void Notify(TNotifyUI& msg);
};

