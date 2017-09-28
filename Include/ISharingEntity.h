/////////////////////////////////////////////////////////////
//  文件名:		ISharingEntity.h  
//
//  创建:		版本：1.0      创建日期：2016-8-30
//
//  描述:		FTP文档共享接口
//                       
//										Add:  Carl   
/////////////////////////////////////////////////////////////

#pragma once

#ifdef _ISHARING_
#define DllExport_ISharing __declspec(dllexport)
#else
#define DllExport_ISharing __declspec(dllimport) 
#endif

class ISharingEntity
{
public:
	// create ftp connection
	virtual void FtpConnection() = 0;

	// open file by IEActiveX
	virtual void OpenFile() = 0;
};

DllExport_ISharing ISharingEntity* CreateSharingEntity();