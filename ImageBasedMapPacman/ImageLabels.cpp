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
	// TODO: ���⿡ return ���� �����մϴ�.
	return iUsage;
}

void getImageLabels(wchar_t* filename, wchar_t* labels)
{
	using namespace std;
	const wstring domain = L"th5a2rg7k4.execute-api.ap-northeast-2.amazonaws.com";
	const wstring requestHeader = L"Content-Type: application/json";
	int port = INTERNET_DEFAULT_HTTPS_PORT;
	bool https = true;

	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;

	/*cout << "Action: Create Product with Id = 1" << endl;
	req.Post(L"/api/products/create",
		requestHeader,
		R"({"Id":1, "Name":"ElectricFan","Qty":14,"Price":20.90})",
		response);
	cout << "Returned Status:" << response.statusCode << endl << endl;
	response.Reset();*/

	
	req.Get(filename, L"", response);
	cout << "Returned Text:" << response.text << endl << endl;
	std::wstring widestr = std::wstring(response.text.begin(), response.text.end());
	wcscpy(labels, widestr.c_str());
	response.Reset();

	/*cout << "Action: Update Product with Id = 1" << endl;
	req.Post(L"/api/products/1",
		requestHeader,
		R"({"Id":1, "Name":"ElectricFan","Qty":15,"Price":29.80})",
		response);
	cout << "Returned Status:" << response.statusCode << endl << endl;
	response.Reset();

	cout << "Action: Retrieve all products" << endl;
	req.Get(L"/api/products", L"", response);
	cout << "Returned Text:" << response.text << endl << endl;
	response.Reset();

	cout << "Action: Delete the product with id = 1" << endl;
	req.Delete(L"/api/products/1", L"", "", response);
	cout << "Returned Status:" << response.statusCode << endl << endl;
	response.Reset();

	cout << "Action: Retrieve all products" << endl;
	req.Get(L"/api/products", L"", response);
	cout << "Returned Text:" << response.text << endl << endl;
	response.Reset();*/

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

void uploadImage(wchar_t* filepath, wchar_t* filename) {

	using namespace std;
	const wstring domain = L"media-query-mediabucket-1i4slys4cekco.s3.amazonaws.com";
	const wstring requestHeader = L"Content-Type: image/jpeg";
	int port = INTERNET_DEFAULT_HTTPS_PORT;
	bool https = true;

	wchar_t url[5000]; // �̹��� ���ε� �� ����� url�� ��� �迭
	getUploadUrl(filename, url); // �̹��� ���ε� �� ����� url ������

	using namespace WinHttpWrapper;

	HttpRequest req(domain, port, https);
	HttpResponse response;

	ifstream ifs(filepath, ios::binary);

	//���� ���������� ������ ������ return
	if (!ifs) {
		cout << "Error happend during file read" << endl;
		return ;
	}

	string image = string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>()); // �̹��� binary �����͸� string data�� ��ȯ

	req.Put(url, requestHeader, image, response);
	cout << "Returned Text:" << response.statusCode << endl << endl; //�������� Ȯ���� ���� http �����ڵ� ���
	response.Reset();


	return;



	return;
}