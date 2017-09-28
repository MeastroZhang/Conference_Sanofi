#pragma once

#pragma warning(disable: 4995) 

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>

#include "../../Include/DuiLib/UIlib.h"
#include "../../include/curl/curl.h"
#include "../../include/cryptopp/aes.h"
#include "../../include/cryptopp/filters.h"
#include "../../include/cryptopp/modes.h"
#include "../../include/json/json.h"
#include "../../include/VidyoClient/VidyoClient.h"

//#include "../../include/glog/logging.h"

using namespace DuiLib;
using namespace CryptoPP;

#ifdef _DEBUG
#       pragma comment(lib, "../../lib/DuiLib_d.lib")
#		pragma comment(lib, "../../lib/cryptlib_d.lib")
#		pragma comment(lib, "../../lib/json_vc71_libmtd.lib")
#else
#       pragma comment(lib, "../../lib/DuiLib.lib")
#		pragma comment(lib, "../../lib/cryptlib.lib")
#		pragma comment(lib, "../../lib/lib_json.lib")
#endif

//#		pragma comment(lib, "../../lib/libglog.lib")

#		pragma comment(lib,"gdiplus.lib")
#       pragma comment(lib, "../../lib/libcurl.lib")
#		pragma comment(lib, "../../lib/VidyoClientDll.lib")