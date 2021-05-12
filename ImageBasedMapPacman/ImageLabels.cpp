#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "WinHttpWrapper.h"
#include <winhttp.h>
#include "ImageLabels.h"
#include <string>

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

	cout << "Action: Retrieve the product with id = 1" << endl;
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