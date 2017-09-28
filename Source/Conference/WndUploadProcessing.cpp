#include "StdAfx.h"
#include "WndUploadProcessing.h"


CWndUploadProcessing::CWndUploadProcessing(void)
{
}


CWndUploadProcessing::~CWndUploadProcessing(void)
{
}

LRESULT CWndUploadProcessing::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CWndUploadProcessing::InitWindow()
{

}

void CWndUploadProcessing::ShowUI(bool bProc)
{
	CHorizontalLayoutUI* labUpFailed = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"UploadFailed"));
	CHorizontalLayoutUI* gifLoading = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"Upload"));
	labUpFailed->SetVisible(!bProc);
	gifLoading->SetVisible(bProc);
	ShowWindow();
}

void CWndUploadProcessing::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			ShowWindow(false);
		}
	}
}
