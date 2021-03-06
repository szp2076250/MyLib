//
//#include "stdafx.h"
//#include "Http.h"
//
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <WinSock2.h>
//
//#pragma comment(lib,"../debug/lib.lib")
//#pragma comment(lib,"ws2_32.lib")
//
////#include "../curl/curl.h"
////#pragma comment(lib,"../debug/libcurl.lib")
//
//size_t http_data_writer(void* data, size_t size, size_t nmemb, void* content)
//{
//	long totalSize = size * nmemb;
//	std::string* symbolBuffer = (std::string*)content;
//	if (symbolBuffer)
//	{
//		symbolBuffer->append((char *)data, ((char*)data) + totalSize);
//	}
//	return totalSize;
//}
//
//bool HttpClass::Post(std::string strHostUrl, std::string strParams, std::string & strRetData)
//{
//
//	auto curl = curl_easy_init();
//	IS_RETURN_ERROR(!curl,false,"Curl null.");
//
//	curl_easy_setopt(curl, CURLOPT_URL, strHostUrl.c_str());
//	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strParams.c_str());
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_data_writer);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strRetData);
//
//	auto ret = curl_easy_perform(curl);
//	IS_RETURN_ERROR(ret != CURLcode::CURLE_OK,false,"curl_easy_perform 失败!");
//
//	curl_easy_cleanup(curl);
//	return true;
//}
//
//size_t write_data (char *ptr, size_t size, size_t nmemb, void *userdata)
//{
//	FILE* fp = NULL;
//	fopen_s(&fp, ((std::string*)userdata)->c_str(), "ab+");
//	size_t nWrite = fwrite(ptr, size, nmemb, fp);
//	fclose(fp);
//	return nWrite;
//};
//
//bool HttpClass::DownLoadImage(std::string strImageUrl,std::string strSavePath)
//{
//	DeleteFileA(strSavePath.c_str());
//
//	auto curl = curl_easy_init();
//	curl_easy_setopt(curl, CURLOPT_URL, strImageUrl.c_str());
//	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
//	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strSavePath);
//	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 5);
//	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
//	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
//	auto ret = curl_easy_perform(curl);
//	curl_easy_cleanup(curl);
//
//	return true;
//}
