#include "StdAfx.h"
#include "HttpClient.h"
#include "../../include/Util/Util.h"
#include <time.h>
#include "tinyxml2.h"

using namespace tinyxml2;

#define POSTURL L"http://sanofi.arkenvmc.com/VmcPlat/"
#define SKEX_POST_URL	L"http://skex.arkenvmc.com/"
#define SZLX_POST_URL	L"http://szbx.arkenvmc.com/"
#define IDOC_URL	"http://106.14.240.42/"

string g_sToken;	//token

string g_sUserEmail;	//Email
string g_sRoomCode;		// room code

string g_sRoomID;	//Room ID
string g_sRoomName;	//room name
string g_sRoomUrl;
string g_sDisplayName;
string g_sStartDate;
int	g_iTotalUser;

long g_lRandNum;

TCHAR	g_szPostUrl[MAX_PATH];

typedef struct _TUserInfo{
	string sInviteCode;
	string sUserEmail;
	string sRoomID;
	string sRoomName;
	string sRoomUrl;
	string sUserName;
	string sStartDate;
	string sToken;
} TUserInfo;

TUserInfo* g_TUserInfo;

size_t write_data(void *data, size_t size, size_t nmemb, string &content) {
	long sizes = size * nmemb;
	string temp((char*)data,sizes);
	content += temp;
	return sizes;
}

string curl_perform(string url, string fields, curl_httppost* formpost, bool bHeader)
{
	CURL *curl;
	CURLcode res;
	string content;

	curl = curl_easy_init();

	if(curl) 
	{
		if (bHeader)
		{
			string strToken = "vmc_token:";
			strToken += g_sToken;
			struct curl_slist *headers = curl_slist_append(NULL, strToken.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		}

		
		/* what URL that receives this POST */
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		if (!fields.empty()) 
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, fields.c_str());
		if (formpost!=NULL) 
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &content);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		/* always cleanup */
		curl_easy_cleanup(curl);

		/* then cleanup the formpost chain */
		curl_formfree(formpost);
	}

	return content;
}

int curl_joinroom(char* szCode, char* szEmail, char* szCount)
{
	int iResult = 0;
	g_sUserEmail = szEmail;
	g_sRoomCode = szCode;

	if (g_sRoomCode.compare(0, 4, "SKEX") == 0)
	{
		wcscpy(g_szPostUrl, SKEX_POST_URL);
	}
	else if (g_sRoomCode.compare(0, 4, "SZBX") == 0)
	{
		wcscpy(g_szPostUrl, SZLX_POST_URL);
	}
	else
		wcscpy(g_szPostUrl, POSTURL);

	TCHAR szText[MAX_PATH];
	ReadIniString(_T("SERVER"), _T("PostUrl"), g_szPostUrl, szText, 255);
	char szTemp[128] = {0};
	ChangTextToUtf8(szText, szTemp);
	string strUrl = szTemp;
	strUrl += "sanofi/join_meeting";

	string strFields = "code=";
	strFields += szCode;
	strFields += "&name=";
	strFields += szEmail;
	strFields += "&count=";
	strFields += szCount;

	string sContent = curl_perform(strUrl, strFields, NULL);

	if (!sContent.empty())
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(sContent, root))
		{
			string sRes = root["res"].asString();
			if (sRes.compare("success") == 0)
			{
				Json::Value val_data = root["data"];
				g_sToken = val_data["vmc_token"].asString();

				Json::Value val_meeting = root["meeting"];
				g_sRoomID = val_meeting["sMeetingId"].asString();
				g_sRoomName = val_meeting["mSubject"].asString();
				g_sRoomUrl = val_meeting["mMeetingUrl"].asString();
				g_sDisplayName = val_meeting["sDsmName"].asString();
				g_sStartDate = val_meeting["mStartDate"].asString();
				g_iTotalUser = val_meeting["count"].asInt();
			}
			else
			{
				Json::Value val_data = root["data"];
				string sError = val_data["msg"].asString();
				iResult = stoi(sError);
			}
		}
		else iResult = 3;
	}
	else iResult = 4;

	return iResult;
}

void curl_conferencecall(char* szAction, bool bCallIn)
{
	TCHAR szText[MAX_PATH];
	TCHAR szLogFlag[MAX_PATH];
	ReadIniString(L"SERVER", L"addlog", L"1", szLogFlag, 128);

	ReadIniString(_T("SERVER"), _T("PostUrl"), g_szPostUrl, szText, 255);
	char szTemp[128] = {0};
	ChangTextToUtf8(szText, szTemp);
	string strUrl = szTemp;
	strUrl += "sanofi/conference_call";

	string strFields = "cfMeetingId=";
	strFields += g_sRoomID;
	strFields += "&sUser=";
	strFields += g_sUserEmail;
	strFields += "&sUserType=1&cfCallAction=";
	strFields += szAction;
	strFields += "&cfLog=";

	ChangTextToUtf8(szLogFlag, szTemp);
	strFields += szTemp;
	strFields += "&cfApplicationName=WinClient&cfApplicationVersion=v1.0&cfApplicationOs=win10";

	strFields += "&pairId=";
	if (bCallIn)
	{
		time_t sec;
		sec = time(0);
		srand((unsigned)sec);
		g_lRandNum = time(0) + rand();
	}
	char szRandNum[20];
	ltoa(g_lRandNum, szRandNum, 10);
	strFields += szRandNum;

	curl_perform(strUrl, strFields, NULL, true);
}

string curl_meetingfiles()
{
	TCHAR szText[MAX_PATH];
	ReadIniString(_T("SERVER"), _T("PostUrl"), g_szPostUrl, szText, 255);
	char szTemp[128] = {0};
	ChangTextToUtf8(szText, szTemp);
	string strUrl = szTemp;
	strUrl += "sanofi/meeting_files";

	string strFields = "meetingId=";
	strFields += g_sRoomID;

	return curl_perform(strUrl, strFields, NULL, true);
}

void curl_alive()
{
	TCHAR szText[MAX_PATH];
	ReadIniString(_T("SERVER"), _T("PostUrl"), g_szPostUrl, szText, 255);
	char szTemp[128] = {0};
	ChangTextToUtf8(szText, szTemp);
	string strUrl = szTemp;
	strUrl += "app/keep_alive";

	curl_perform(strUrl, "", NULL, true);
}

string curl_uploadfile(char* szFilePath)
{
	string sUrl(IDOC_URL);
	sUrl += "doc/upload";

	struct curl_httppost *formpost = NULL;
	struct curl_httppost *lastptr = NULL;

	/* Fill in the file upload field */  
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "file",
		CURLFORM_FILE, szFilePath,
		CURLFORM_END);

	/* Fill in the filename field */  
	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "token",
		CURLFORM_COPYCONTENTS, "VMC_TOKEN_22112188_DFC@#D2D",
		CURLFORM_END);

	string sName(szFilePath);
	char szName[MAX_PATH];
	int iPos = sName.rfind("\\");
	if (iPos > 0)
	{
		sName = sName.substr(iPos+1);
		TCHAR* tszName = new TCHAR[sName.length()*2+1];
		USES_CONVERSION;
		tszName = A2T(sName.c_str());
		ChangTextToUtf8(tszName, szName);
	}

	curl_formadd(&formpost,
		&lastptr,
		CURLFORM_COPYNAME, "name",
		CURLFORM_COPYCONTENTS, szName,
		CURLFORM_END);

	string sContent = curl_perform(sUrl, "", formpost);

	if (!sContent.empty())
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(sContent, root))
		{
			string sRes = root["code"].asString();
			if (sRes.compare("1") == 0)	// success
			{
				DocViewUploadBack uploadBack;
				uploadBack.sFileType = root["ext"].asString();

				string sTest = root["nname"].asString();
				uploadBack.sFileName = szName;
				uploadBack.sFileSize = root["size"].asString();
				string sUuid = root["uuid"].asString();

				string sViewUrl(IDOC_URL);
				sViewUrl += "view/";
				sViewUrl += sUuid;
				uploadBack.sViewUrl = sViewUrl;
				curl_addfile(uploadBack);

				return sViewUrl;
			}
		}
	}

	return "";
}

void curl_addfile(DocViewUploadBack uploadBack)
{
	TCHAR szText[MAX_PATH];
	ReadIniString(_T("SERVER"), _T("PostUrl"), g_szPostUrl, szText, 255);
	char szTemp[128] = {0};
	ChangTextToUtf8(szText, szTemp);
	string sUrl(szTemp);
	sUrl += "sanofi/add_file";

	string sFields = "meetingId=";
	sFields += g_sRoomID;
	sFields += "&fileName=";
	sFields += uploadBack.sFileName;
	sFields += "&fileType=";
	sFields += "1";
	sFields += "&fileSize=";
	sFields += uploadBack.sFileSize;
	sFields += "&fileViewUrl=";
	sFields += uploadBack.sViewUrl;

	curl_perform(sUrl, sFields, NULL, true);
}

void curl_deletefile()
{

}

string curl_getversion()
{
	char szTemp[128] = {0};
	ChangTextToUtf8(POSTURL, szTemp);
	string strUrl(szTemp);
	strUrl += "sanofi/get_client_version";

	string strFields = "pType=1";

	string sContent = curl_perform(strUrl, strFields, NULL, true);

	string sVersion, sUrl;
	if (!sContent.empty())
	{
		Json::Reader reader;
		Json::Value root;
		if (reader.parse(sContent, root))
		{
			string sRes = root["res"].asString();
			if (sRes.compare("success") == 0)	// success
			{
				Json::Value val_app = root["app"];
				sVersion = val_app["pValue"].asString();
				sUrl = val_app["pUrl"].asString();
				tiny_writeupdateserveraddr(sUrl);
			}
		}
	}

	return sVersion;
}

void tiny_writeupdateserveraddr(string sServerIP)
{
	tinyxml2::XMLDocument doc;

	TCHAR szConfigPath[MAX_PATH];
	GetFileName(L"\\AutoUpdater\\AutoUpdater.exe.config", szConfigPath);

	char szTemp[MAX_PATH] = {0};
	ChangTextToUtf8(szConfigPath, szTemp);
	if (doc.LoadFile(szTemp) != 0)
	{
		return ;
	}

	XMLElement* root = doc.RootElement();
	XMLElement* nodeApp = root->LastChildElement();

	if (nodeApp != NULL)
	{
		XMLElement* serverip = nodeApp->FirstChildElement();
		serverip->SetAttribute("value", sServerIP.c_str());
	}

	doc.SaveFile(szTemp);
}