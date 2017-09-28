/********************************************************************
	created:	2016/07/19
	author:		Carl
	e-mail:		carl.zhang@arkenvmc.com
*********************************************************************/
#pragma once
#include "stdafx.h"
#include "MainFrame.h"

void InitResource()
{
	// 资源类型
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIP);
#endif
	// 资源路径
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	// 加载资源
	switch(CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			strResourcePath += _T("SKIN\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			strResourcePath += _T("SKIN\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"), _T("xml"));
			break;
		}
	case UILIB_ZIP:
		{
			strResourcePath += _T("SKIN\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CPaintManagerUI::SetResourceZip(_T("Conference.zip"), true);
			// 加载资源管理器
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			strResourcePath += _T("SKIN\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());

			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(), _T("IDR_ZIPRES"), _T("ZIPRES"));
			if( hResource != NULL ) {
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(), hResource);
				if( hGlobal != NULL ) {
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(), hResource);
					if( dwSize > 0 ) {
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal), dwSize);
						// 加载资源管理器
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"), NULL);
					}
				}
				::FreeResource(hResource);
			}
		}
		break;
	}
}

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	// 初始化UI管理器
	CPaintManagerUI::SetInstance(hInstance);
	// 初始化资源
	InitResource();
	// Init curl global
	curl_global_init(CURL_GLOBAL_NOTHING);
	// 加载控件
	CWkeWebkitUI::InitializeWebkit();

	MainFrame* pWnd = new MainFrame;
	pWnd->Create( NULL, _T("FrameWnd"), UI_WNDSTYLE_DIALOG, 0 );
	pWnd->ShowWindow( false );

	CPaintManagerUI::MessageLoop();

	delete pWnd;
	pWnd = NULL;

	//CWkeWebkitUI::UninitializeWebkit();
	curl_global_cleanup();
	CResourceManager::GetInstance()->Release();
	OleUninitialize();
	::CoUninitialize();

	return 0;
}