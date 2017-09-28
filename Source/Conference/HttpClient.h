#pragma once

typedef struct DocViewUploadBack_
{
	string sFileType;
	string sFileSize;
	string sFileName;
	string sViewUrl;
} DocViewUploadBack;

/*!
	@brief Perform curl

	curl perform post request

	@param[in] url		request url
	@param[in] fields	request fields
	@param[in] formpost	request formpost
	@param[in] bHeader	if or not with header
*/
string curl_perform(string url, string fields, curl_httppost* formpost, bool bHeader = false);

/*!
	@brief join room by code and email

	@param[in] szCode		Meeting code
	@param[in] szEmail		User's email
	@param[in] szCount		No of participants
*/
int curl_joinroom(char* szCode, char* szEmail, char* szCount);

/*!
	@brief post info to server when join or leave conference

	@param[in] szAction		1-join conference, 
							2-leave conference, 
							3-start conference, 
							4-end conference
*/
void curl_conferencecall(char* szAction, bool bCallIn = true);

/*!
	@brief Get all files in the meeting
*/
string curl_meetingfiles();

/*!
	@brief Keep alive
*/
void curl_alive();

/*!
	@brief Upload file to IDocview server
*/
string curl_uploadfile(char* szFilePath);

/*!
	@brief Add file to server
*/
void curl_addfile(DocViewUploadBack uploadBack);

/*!
	@brief delete file from server
*/
void curl_deletefile();


/*!
	@brief get client version
*/
string curl_getversion();


/*!
	@brief write update server address to config
*/
void tiny_writeupdateserveraddr(string sServerIP);