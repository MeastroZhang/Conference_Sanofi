#pragma once

#define PORTALURL	L"http://t.vf365.cn"
#define PATH_DOCS	"C:\\Users\\Public"
#define PATH_LOG	"C:\\Users\\Public\\Sanofi\\Log"
#define PATH_PHOTO	L"C:\\Users\\Public\\Sanofi\\PhotoGraph"
#define PATH_THUMB	L"C:\\Users\\Public\\Sanofi\\Thumb"
#define PATH_CONFIG	L"C:\\Users\\Public\\Sanofi\\config.ini"
#define PATH_LOCAL_LOG	L"C:\\Users\\Public\\Sanofi\\log.txt"

#define WM_VIDYO						WM_USER+0x1001
#define WM_WINDOW						WM_USER+0x1002
#define WM_WINDOW_LOGIN					WM_USER+0x1003
#define WM_WINDOW_CONTACTS				WM_USER+0x1004
#define WM_WINDOW_CONFERENCE			WM_USER+0x1005
#define WM_WINDOW_SETTING				WM_USER+0x1006
#define WM_TRAY							WM_USER+0x1007
#define WM_TOOLBAR						WM_USER+0x1008
#define WM_SPEAKER_TEST					WM_USER+0x1009
#define WM_EXIT_CONFERENCE				WM_USER+0x100a
#define WM_END_CONFERENCE				WM_USER+0x100b
#define WM_CHAT_REPLY					WM_USER+0x100c
#define WM_JOIN_PROCESS					WM_USER+0x100d
#define WM_MODERATOR_CHANGED			WM_USER+0x100e
#define WM_SHARE						WM_USER+0x100f
#define WM_SHARE_SCREEN					WM_USER+0x1010
#define WM_SHARE_PROCESS				WM_USER+0x1011
#define WM_READERBAR_STOPSHARE			WM_USER+0x1012

#define TIMER_TOOLBAR			1001
#define TIMER_SEARCH			1002
#define TIMER_CLOCK				1003
#define TIMER_AUDIO_TEST		1004
#define TIMER_SPEAKER_TEST		1005
#define TIMER_LECTUREMODE		1006	// lecture mode refresh status
#define TIMER_LECTURE_NOTE		1007	// lecture mode reminder window close timer
#define TIMER_SHARE				1008	// rev share message
#define TIMER_SHARING			1009	// confirm is not sharing when conference start
#define TIMER_HIDE_TOOLBAR		1010	// share wnd toolbar
#define TIMER_HTTP_ALIVE		1011
#define TIMER_EXIT				1012

#define SIZE_FILES				128		// 文档共享列表文件数量

typedef enum eMsgWndType_
{
	MSG_COMMON,
	MSG_WRONGPIN,
	MSG_ROOMLOCKED,
	MSG_RECORD_DISABLE,
} eMsgWndType;

typedef enum ClientStatusType_
{
	CLIENTSTATUSNOLOG,		//未登录
	CLIENTSTATUSLOGINED,	//已登录
	CLIENTSTATUSMEETING,	//会议中
} ClientStatusType;

typedef enum eLoginError_
{
	ERROR_CLIENT_USERNAME_NULL = 1,
	ERROR_CLIENT_PSW_NULL,
	ERROR_CLIENT_ROOMID_NULL,
	ERROR_CLIENT_DISPLAYNAME_NULL,
	ERROR_CLIENT_USERNO_NULL,
	ERROR_CLIENT_INPUT_WRONG,
	ERROR_SERVER_DISCONNECT,
	ERROR_SERVER_ABNORMAL,
} eLoginError;

typedef enum ToolBarMsg_
{	
	TB_MSG_SHARE = 1,
	TB_MSG_STOPSHARE,
	TB_MSG_CONTROL,
	TB_MSG_CHAT,
	TB_MSG_PHOTO,
	TB_MSG_EXIT,
	TB_MSG_MIN,
} ToolBarMsg;

typedef enum ShareType_
{
	SHARE_NULL = 0,
	SHARE_SANOFI,
	SHARE_SKEX,
	SHARE_RESHARE,
} ShareType;

typedef enum _eModeType 
{	
	ntConferenceUI = 0, 
	ntGuestUI,
	ntLoginUI,
	ntSetting,
	ntContactsUI,
	ntProcessing
} eModeType;

typedef enum _eBtnLoginWin 
{	
	ntCloseBtn = 0,
	ntMinBtn,
	ntLoginBtn
} eBtnLoginWin;

typedef enum _eInputWndType
{
	ntRoomPin = 0,
	ntModeratorPin,
} eInputWndType;

typedef enum _eClientType
{
	ntNormal = 0,
	ntSKEX,
	ntSZBX,
	ntDBUP,
} eClientType;

typedef struct _tLocalData
{
	bool bAutoStart;	//开机启动
	bool bMinTray;		//最小化到托盘
	bool bCopyUrl;		//进入会议后复制链接到剪贴板
	bool bExitMsg;		//退出会议显示提示框
	bool bDBScreen;		//启用双屏显示共享内容
	bool bFullScreen;	//进入会议全屏显示
	bool bShareFullScreen;	//全屏显示共享
	bool bShareAutoSize;	//共享自适应显示窗口尺寸
} tLocalData;

typedef struct _tShareFiles
{
	TCHAR	FileName[SIZE_FILES][MAX_PATH];
	TCHAR	FileUrl[SIZE_FILES][MAX_PATH];
	int		FileNum;
} tShareFiles;

typedef struct _tChatUser
{
	TCHAR	UserName[129];
	TCHAR	UserURL[257];
} tChatUser;

typedef struct _tChatUserList
{
	int			iNum;
	tChatUser*	Users;
} tChatUserList;

typedef struct _tChatMsg
{
	BOOL	bSend;
	TCHAR	szName[129];
	TCHAR	szMsg[1025];
	TCHAR	szTime[128];
} tChatMsg;

typedef struct _tChatMsgList
{
	int			iMsgNum;
	tChatMsg*	tMsg;
} tChatMsgList;

// get video replay library
typedef struct _tRecordLibrary
{
	char	token[128];
	char	URL[128];
} tRecordLibrary;

// self control status
typedef struct _tSelfControlStatus
{
	bool	bMuteVideo;
	bool	bMuteAudioIn;
	bool	bMuteAudioOut;
	bool	bMuteServerVideo;
	bool	bMuteServerAuidoIn;
	bool	bFullScreen;
	bool	bShare;
	bool	bShareSpeaker;
	bool	bShareScreen;
	bool	bRevScreen;
} tSelfControlStatus;

typedef struct _tStartTime
{
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
} tStartTime;

// server control status
typedef struct _tServerControlStatus
{
	bool	bLocked;
	bool	bMuteVide;
	bool	bMuteAudio;
	bool	bRecord;
	bool	bRecordPause;
	VidyoClientLectureStage	lectureMode;
} tServerContorlStatus;

