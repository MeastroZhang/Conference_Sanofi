#include "StdAfx.h"
#include "DlgSharing.h"
#include "DlgDocReader.h"
#include "HttpClient.h"
#include "../../include/Util/Util.h"

static VidyoClientRequestGetWindowsAndDesktops getWindwosAndDesktops = {0};
static VidyoClientRequestWindowShares WindowShares;

static tShareFiles ShareFiles = {0};

extern CDuiString g_sShareUrl;

HWND g_hParent;

DWORD WINAPI UploadFile(LPVOID lpParamter)
{
	WCHAR szBuffer[MAX_PATH];
	wcscpy(szBuffer, (WCHAR *)lpParamter);
	USES_CONVERSION;
	char* szPath = T2A(szBuffer);

	string sPath(szPath);
	string sViewUrl = curl_uploadfile(szPath);

	if (!sViewUrl.empty())
	{
		TCHAR tszTemp[MAX_PATH];
		g_sShareUrl = ChangUtf8ToText(sViewUrl.c_str(), tszTemp);
		::PostMessageW(g_hParent, WM_SHARE, TRUE, NULL);
	}
	else
		::PostMessageW(g_hParent, WM_SHARE_PROCESS, FALSE, NULL);

	return 0L;
}

CDlgSharing::CDlgSharing(HWND hParent)
{
	g_hParent = hParent;
}


CDlgSharing::~CDlgSharing(void)
{
}

LRESULT CDlgSharing::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CDlgSharing::InitWindow()
{
	CLabelUI* labCaption = static_cast<CLabelUI*>(m_pm.FindControl(_T("labCaption")));

	TCHAR szText[MAX_PATH];
	project_language_loadstring(_T("SHARE"), _T("labCaption"), szText, MAX_PATH);
	labCaption->SetText(szText);
}

void CDlgSharing::ShowUI(ShareType type, bool bShow /* = true */)
{
	if (bShow)
	{
		UpdateData();
		UpdateUI(type);
	}

	ShowWindow(bShow);
}

void CDlgSharing::UpdateData()
{
	GetShareFiles();
}

void CDlgSharing::UpdateUI(ShareType type)
{
	CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(L"listShare"));
	pList->RemoveAll();
	for (int i = 0; i < ShareFiles.FileNum; i++)
	{
		CListContainerElementUI* pListItem  = new CListContainerElementUI();
		pListItem->SetFixedHeight(40);
		pListItem->SetManager(&m_pm, NULL, false);
		CLabelUI* pLabIndex = new CLabelUI;
		CDuiString strTemp;
		strTemp.Format(L"%d", i);
		pLabIndex->SetText(strTemp);
		pLabIndex->SetAttribute(L"width", L"30");
		pLabIndex->SetAttribute(L"font", L"1");
		pLabIndex->SetAttribute(L"align", L"right");
		pListItem->Add(pLabIndex);

		CLabelUI* pLabItem = new CLabelUI;
		pLabItem->SetText(ShareFiles.FileName[i]);
		pLabItem->SetAttribute(L"textpadding", L"10,0,0,0");
		pLabItem->SetAttribute(L"endellipsis", L"true");
		pLabItem->SetAttribute(L"tooltip", ShareFiles.FileName[i]);
		pListItem->Add(pLabItem);

		pListItem->SetUserData(ShareFiles.FileUrl[i]);
		pList->Add(pListItem);
	}

	CHorizontalLayoutUI* pHorShareLocal = static_cast<CHorizontalLayoutUI*>(m_pm.FindControl(L"horShareLocal"));
	if (type == SHARE_SKEX)
	{
		pHorShareLocal->SetAttribute(L"visible", L"true");
	}
	else
		pHorShareLocal->SetAttribute(L"visible", L"false");
}

void CDlgSharing::GetShareFiles()
{
	memset(&ShareFiles, 0, sizeof(ShareFiles));
	string strHttpResult = curl_meetingfiles();
	if (!strHttpResult.empty())
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(strHttpResult, root))
		{
			ShareFiles.FileNum = root["files"].size();
			for (int i = 0; i < ShareFiles.FileNum; i++)
			{
				string fileName = root["files"][i]["fileName"].asString();
				ChangUtf8ToText(fileName.c_str(), ShareFiles.FileName[i]);
				string fileUrl = root["files"][i]["fileViewUrl"].asString();
				ChangUtf8ToText(fileUrl.c_str(), ShareFiles.FileUrl[i]);
			}
		}
	}
}

void CDlgSharing::SearchShareFiles()
{
	CEditUI* editSearch = static_cast<CEditUI*>(m_pm.FindControl(L"editSearch"));
	CDuiString sSearch = editSearch->GetText();
	if (!sSearch.IsEmpty())
	{
		int iCount = 0;
		for (int i = 0; i < ShareFiles.FileNum; i++)
		{
			CDuiString sFind(ShareFiles.FileName[i]);
			if (sFind.Find(sSearch) >= 0)
			{
				if (iCount != i)
				{
					TCHAR szName[MAX_PATH], szUrl[MAX_PATH];
					_tcscpy_s(szName, ShareFiles.FileName[iCount]);
					_tcscpy_s(szUrl, ShareFiles.FileUrl[iCount]);
					_tcscpy_s(ShareFiles.FileName[iCount], ShareFiles.FileName[i]);
					_tcscpy_s(ShareFiles.FileUrl[iCount], ShareFiles.FileUrl[i]);
					_tcscpy_s(ShareFiles.FileName[i], szName);
					_tcscpy_s(ShareFiles.FileUrl[i], szUrl);
				}
				
				iCount++;
			}
		}
	}
}

void CDlgSharing::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click"))
	{
		if (msg.pSender->GetName() == _T("btnClose"))
		{
			ShowWindow(false);
		}
		else if (msg.pSender->GetName() == L"btnShareDesktop")
		{
			::PostMessageW(g_hParent, WM_SHARE_SCREEN, TRUE, NULL);
			ShowWindow(false);
		}
		else if (msg.pSender->GetName() == L"btnShareLocalFile")
		{
			WCHAR szBuffer[MAX_PATH] = {0}; 
			//SetPath(NULL, szBuffer);
			GetUploadFilePath(NULL, szBuffer);

			if (wcscmp(szBuffer, L"") != 0)
			{
				HANDLE hThread = CreateThread(NULL, 0, UploadFile, (LPVOID)szBuffer, 0, NULL);
				CloseHandle(hThread);

				::PostMessageW(g_hParent, WM_SHARE_PROCESS, TRUE, NULL);
			}
		}
	}
	else if ( msg.sType == L"itemactivate")
	{
		g_sShareUrl = msg.pSender->GetUserData();
		::PostMessageW(g_hParent, WM_SHARE, TRUE, NULL);
		ShowWindow(false);
	}
}