#pragma once

#ifdef CAMERA_SHARE
#define DECLSPEC __declspec(dllexport)
#else
#define DECLSPEC __declspec(dllimport) 
#endif

/* 外部接口调用 */
DECLSPEC void InitCameraShare();

DECLSPEC void ReleaseCamera();