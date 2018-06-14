
#include "stdafx.h"
#include "Http.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

#pragma comment(lib,"../debug/lib.lib")
#pragma comment(lib,"ws2_32.lib")

#include "../curl/curl.h"
#pragma comment(lib,"../debug/libcurl.lib")

void HttpClass::test()
{
	int num;

	SOCKET s;
	WSADATA wsa;
	struct sockaddr_in serv;

	char sndBuf[1024], rcvBuf[2048];

	WSAStartup(MAKEWORD(2, 1), &wsa);


	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket error!");
		exit(1);
	}

	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(80);
	serv.sin_addr.S_un.S_addr = inet_addr("115.239.210.26");

	if ((connect(s, (struct sockaddr *)&serv, sizeof(serv))) < 0)
	{
		perror("connet error!");
		exit(1);
	}

	memset(sndBuf, 0, 1024);
	memset(rcvBuf, 0, 2048);

	//头信息  
	strcat_s(sndBuf, "POST ");
	strcat_s(sndBuf, "https://store.steampowered.com/login/rendercaptcha?gid=3007723630214454648");
	strcat_s(sndBuf, " HTTP/1.0\r\n");
	strcat_s(sndBuf, "Host: ");
	strcat_s(sndBuf, "store.steampowered.com");
	strcat_s(sndBuf, "\r\n");
	strcat_s(sndBuf, "Connection: keep-alive\r\n");

	strcat_s(sndBuf, "\r\n");

	puts(sndBuf);

	if ((num = send(s, sndBuf, 1024, 0)) < 0)
	{
		perror("send error!");
		exit(1);
	}

	puts("send success!\n");

	do {

		if ((num = recv(s, rcvBuf, 2048, 0)) < 0)
		{
			perror("recv error!");
			system("pause");
			exit(1);
		}
		else if (num > 0)
		{
			printf("%s", rcvBuf);
			memset(rcvBuf, 0, 2048);
		}
	} while (num > 0);

	puts("\nread success!\n");

	closesocket(s);

	system("pause");
	WSACleanup();


}

size_t http_data_writer(void* data, size_t size, size_t nmemb, void* content)
{
	long totalSize = size * nmemb;
	std::string* symbolBuffer = (std::string*)content;
	if (symbolBuffer)
	{
		symbolBuffer->append((char *)data, ((char*)data) + totalSize);
	}
	return totalSize;
}

void HttpClass::test2()
{
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		/* First set the URL that is about to receive our POST. This URL can
		just as well be a https:// URL if that is what should receive the
		data. */
		curl_easy_setopt(curl, CURLOPT_URL, "https://store.steampowered.com/join/refreshcaptcha/");
		/* Now specify the POST data */
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "count=1");

		//设置回调函数  
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_data_writer);
		//设置写数据  
		std::string strData;
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&strData);

		/* Perform the request, res will get the return code */
		res = curl_easy_perform(curl);

		IS_RETURN_ERROR(res != CURLcode::CURLE_OK, , "调用失败!");

		/* always cleanup */
		curl_easy_cleanup(curl);
		system("pause");
	}

}
