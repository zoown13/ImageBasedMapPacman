#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "WinHttpWrapper.h"
#include <winhttp.h>
#include "ImageLabels.h"
#include <string>
#include <fstream>
#include <algorithm>  // std::for_each

static ImageLabels iUsage;

ImageLabels::ImageLabels()
{
}

ImageLabels& ImageLabels::getInstance()
{
	// TODO: 여기에 return 문을 삽입합니다.
	return iUsage;
}

void getImageLabels(wchar_t* filename, wchar_t* labels)
{
	using namespace std;
	const wstring domain = L"th5a2rg7k4.execute-api.ap-northeast-2.amazonaws.com";
	const wstring requestHeader = L"Content-Type: application/json";
	int port = INTERNET_DEFAULT_HTTPS_PORT;
	bool https = true;
	wchar_t* pos, *startpos, *outpos;


	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;
	
	req.Get(filename, L"", response);
	cout << "Returned Text:" << response.text << endl << endl;
	std::wstring widestr = std::wstring(response.text.begin(), response.text.end());

	pos = wcsstr(const_cast<wchar_t*>(widestr.c_str()), L"labels");// labels 문자 시작위치 반환
	startpos = wcsstr(pos, L"[");// label 데이터 목록 시작위치 반환
	outpos = wcsstr(startpos, L"]");// label 데이터 목록 종료위치 반환
	wcsncpy(labels, startpos + 1, wcslen(startpos) - wcslen(outpos)-1);// label 데이터들만 슬라이싱 후 반환

	response.Reset();
	return;
}

void getUploadUrl(wchar_t* filename, wchar_t* url) {

	using namespace std;
	const wstring domain = L"z3r78ucou9.execute-api.ap-northeast-2.amazonaws.com";
	const wstring requestHeader = L"Content-Type: image/jpeg";
	int port = INTERNET_DEFAULT_HTTPS_PORT;
	bool https = true;

	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;

	req.Get(filename, L"", response);
	cout << "Returned Text:" << response.text << endl << endl;
	std::wstring widestr = std::wstring(response.text.begin(), response.text.end());
	wcscpy(url, widestr.c_str());
	response.Reset();

	return;
}

int uploadImage(wchar_t* filepath, wchar_t* filename) {

	using namespace std;
	const wstring domain = L"media-query-mediabucket-1i4slys4cekco.s3.amazonaws.com";
	const wstring requestHeader = L"Content-Type: image/jpeg";
	int port = INTERNET_DEFAULT_HTTPS_PORT;
	bool https = true;

	wchar_t url[5000]; // 이미지 업로드 할 서명된 url이 담길 배열
	getUploadUrl(filename, url); // 이미지 업로드 할 서명된 url 가져옴

	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;

	ifstream ifs(filepath, ios::binary);

	//파일 정상적으로 읽히지 않으면 return
	if (!ifs) {
		cout << "Error happend during file read" << endl;
		return -1;
	}

	string image = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()); // 이미지 binary 데이터를 string data로 변환

	req.Put(url, requestHeader, image, response);
	cout << "Returned Text:" << response.statusCode << endl << endl; //성공여부 확인을 위한 http 상태코드 출력
	response.Reset();

	return 0;
}