#include "StdAfx.h"
#include "DlgParticipants.h"
#include "../../include/Util/Util.h"

static VidyoClientRequestParticipants reqParticipants = {0};

CDlgParticipants::CDlgParticipants(HWND hParent)
{
	m_hParent = hParent;
}


CDlgParticipants::~CDlgParticipants(void)
{
}

void CDlgParticipants::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_pm.FindControl(_T("labCaption")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("PARTICIPANTS"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
}

void CDlgParticipants::UpdataUI()
{
	int iError = VidyoClientSendRequest(VIDYO_CLIENT_REQUEST_GET_PARTICIPANTS, &reqParticipants, sizeof(reqParticipants));
	if (iError == VIDYO_FALSE)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		CListUI* pParList = static_cast<CListUI*>(m_pm.FindControl(L"listParticipants"));
		pParList->RemoveAll();

		for (unsigned int i = 0; i < reqParticipants.numberParticipants; i++)
		{
			CListContainerElementUI* pListItem  = new CListContainerElementUI();
			pListItem->SetFixedHeight(40);
			pListItem->SetManager(&m_pm, NULL, false);
			ChangUtf8ToText(reqParticipants.name[i], szBuf);
			CLabelUI* pLabItem = new CLabelUI;
			pLabItem->SetText(szBuf);
			pLabItem->SetAttribute(L"textpadding", L"30,0,0,0");
			pLabItem->SetAttribute(L"font", L"1");
			pListItem->Add(pLabItem);
			pParList->Add(pListItem);
		}
	}
}

void CDlgParticipants::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			ShowWindow(false);
		}
	}
}