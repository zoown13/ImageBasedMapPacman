#define _CRT_SECURE_NO_WARNINGS

// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include "framework.h"
#include "ImageBasedMapPacman.h"
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ImageLabels.h"
#include "mmsystem.h" // 음악 재생을 위해 인클루드 

#pragma comment(lib,"winmm.lib") // lib 파일을 읽어들이는 메크로 

#define MAX_LOADSTRING 100
#define ObjectNum 9

//함수들 선언
void Clear_Map(int packman[][32]);
void IsItemInLabels(int* result, wchar_t* labels);
void MakeMap(HDC hdc);
void Snack(HDC hdc);
int Counter(int packman[20][32]);
void PackmanAnimation(HDC hdc, int s);
void RedAnimation(HDC hdc);
void PinkAnimation(HDC hdc);
void MintAnimation(HDC hdc);
void RandomMap(void);

/**
* 이미지에 존재하는 객체를 확인한 후 표현될 수 있는 오브젝트 목록 반환
* result: 표현할 수 있는 오브젝트 목록
* labels: 이미지에 존재하는 객체 목록
*/
void IsItemInLabels(int* result, wchar_t* labels);


// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[] = _T("ImageBasedMapPackman");                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.
OPENFILENAME OFN; // 파일을 열기 위한 구조체
wchar_t items[ObjectNum][100] = { L"Tree", L"Car", L"Bicycle", L"Dog", L"Person", L"Cat",  L"Ballon", L"Ball", L"Fruit" }; // 맵에 표현될 수 있는 오브젝트 목록
   /**
       * imageName: /api/ + 사진 파일 이름
       * labels: 사진 라벨링 데이터가 저장될 배열
    */
static wchar_t imageName[100];
static wchar_t imageNameUrl[100];
static wchar_t labels[500];
static int result[ObjectNum] = { 0, }; // 오브젝트 목록 여부

// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_IMAGEBASEDMAPPACMAN, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IMAGEBASEDMAPPACMAN));

    MSG msg;

    // 기본 메시지 루프입니다:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

//
//  함수: MyRegisterClass()
//
//  용도: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMAGEBASEDMAPPACMAN));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_IMAGEBASEDMAPPACMAN);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   용도: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   주석:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        0, 0, 1600, 1000, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
}

//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)i
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//



int Packman[20][32] = {
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,9,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
   {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},//1: 장애물 ,9 : 랜덤맵 구성물이 위치할 자리
};
int mapE1 = 48;//장애물의 세로(960/10)
int mapE2 = 40;//장애물의 가로(1280/16)
int snackSize1 = 40;//먹는 오브젝트 크기
int snackSize2 = 48;
static int score = 0;//출력 스코어 저장변수
static int ScoreCount = 0;
static int m, n;//몬스터로 인해 지워지는 과자 다시 생성을 위한 변수
static BOOL MEMORY;//몬스터로 인해 지워지는 과자 다시 생성을 위한 변수
  
HINSTANCE g_hInst;

void Clear_Map(int packman[][32]) { // 다시 게임 시작시 초기화하는 함수 
    int i, j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 32; j++) {// 몬스터, 과자, 팩맨 등 초기화 
            if (packman[i][j] == 3 || packman[i][j] == 15 || packman[i][j] == 16 || packman[i][j] == 17 || packman[i][j] == 5 || packman[i][j] == 6 || packman[i][j] == 7 || packman[i][j] == 8)
                packman[i][j] = 4;
        }
    }
}

void IsItemInLabels(int* result, wchar_t* labels)
{
    wchar_t* pwc;
    wchar_t* pt;

    pwc = wcstok(labels, L" ,", &pt);
    while (pwc != NULL)
    {
        for (int i = 0; i < ObjectNum; i++) {

            if (wcsstr(pwc, items[i]) != NULL) {
                result[i] = 1;
            }
        }

        pwc = wcstok(NULL, L" ,", &pt);
    }

}

void MakeMap(HDC hdc) //맵 장애물 표시
{
    HDC memdc;
    HBITMAP hBit;
    int i, j;   

    memdc = CreateCompatibleDC(hdc);
    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Tree));
    SelectObject(memdc, hBit);

    for (i = 0; i < 20; i += 19)
        for (j = 0; j < 32; j ++)
            if (Packman[i][j] == 1)
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
    for (i = 0; i < 20; i ++)
        for (j = 0; j < 32; j += 31)
            if (Packman[i][j] == 1)
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);//테두리 나무로 출력

    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_TreeX2));
    SelectObject(memdc, hBit);

    for (i = 1; i < 18; i += 2)
        for (j = 1; j < 30; j += 2)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//장애물 출력에 규칙성은 있지만 맵생성이 랜덤
            }
    if (result[0]) {       // result[i]가 1이면 해당 사진을 맵으로 씀 
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Tree));//나무
        SelectObject(memdc, hBit);
        for (i = 0; i < 20; i += 19)
            for (j = 1; j < 32; j += 2)
                if (Packman[i][j] == 1)
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
        for (i = 0; i < 20; i += 2)
            for (j = 0; j < 32; j += 31)
                if (Packman[i][j] == 1)
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);//겉에 태두리 부분 나무와 차로 구성
    }
    if (result[1]) {
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Car));//차
        SelectObject(memdc, hBit);
        for (i = 0; i < 20; i += 19)
            for (j = 0; j < 32; j += 2)
                if (Packman[i][j] == 1)
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
        for (i = 1; i < 20; i += 2)
            for (j = 0; j < 32; j += 31)
                if (Packman[i][j] == 1)
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);//겉에 태두리 부분 나무와 차로 구성

    }
    if (result[2]) {
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_bicycle));//자전거
        SelectObject(memdc, hBit);
        i = 5;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//장애물 출력에 규칙성은 있지만 맵생성이 랜덤

            }
        i = 9;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 11;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 15;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//사이즈 키워서 4칸에 들어가게 장애물 출력

            }
    }

    if (result[3]) {
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Dog));//강아지
        SelectObject(memdc, hBit);
        i = 3;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);
            }
        i = 7;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 13;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 17;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//사이즈 키워서 4칸에 들어가게 장애물 출력

            }
    }
    if (result[4]) {
        hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Person));//사람
        SelectObject(memdc, hBit);
        i = 3;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 9;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 13;
        for (j = 5; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);

            }
        i = 15;
        for (j = 3; j < 30; j += 4)
            if (Packman[i][j] == 1) {
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//사이즈 키워서 4칸에 들어가게 장애물 출력
            }

        if (result[5]) {
            hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Cat));//고양이
            SelectObject(memdc, hBit);
            i = 5;
            for (j = 5; j < 30; j += 4)
                if (Packman[i][j] == 1) {
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                    BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);
                }
            i = 7;
            for (j = 3; j < 30; j += 4)
                if (Packman[i][j] == 1) {
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                    BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);
                }
            i = 11;
            for (j = 5; j < 30; j += 4)
                if (Packman[i][j] == 1) {
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                    BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);
                }
            i = 17;
            for (j = 3; j < 30; j += 4)
                if (Packman[i][j] == 1) {
                    BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, i * mapE1, mapE2 + mapE2, mapE1, memdc, mapE2, 0, SRCCOPY);
                    BitBlt(hdc, j * mapE2, (i + 1) * mapE1, mapE2, mapE1 + mapE1, memdc, 0, mapE1, SRCCOPY);
                    BitBlt(hdc, (j + 1) * mapE2, (i + 1) * mapE1, mapE2 + mapE2, mapE1 + mapE1, memdc, mapE2, mapE1, SRCCOPY);//사이즈 키워서 4칸에 들어가게 장애물 출력
                }
        }

        DeleteObject(hBit);
        DeleteDC(memdc);

    }
}

void Snack(HDC hdc) {    // 과자 그리기 함수 
    HDC memdc;
    HBITMAP Snack, Mask;
    int i, j;

    memdc = CreateCompatibleDC(hdc);
    Snack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown2));
    Mask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));

    if (MEMORY)
        Packman[m][n] = 4;
    

    for (i = 0; i < 20; i += 2)
        for (j = 0; j < 32; j += 2) {
            if (Packman[i][j] == 4) {
                if (Packman[i][j + 1] == 1 && Packman[i + 1][j] == 1 && Packman[i - 2][j] == 1 && Packman[i][j - 2] == 1) {//랜덤으로 생성되는 장애물들에 의해서 먹지 못하는 오브젝트 나오는 것 방지하기 위해 주변이 막히면 장애물 처리
                    Packman[i][j] = 1;
                    Packman[i - 1][j] = 1;
                    Packman[i][j - 1] = 1;
                    Packman[i - 1][j - 1] = 1;
                }               
                SelectObject(memdc, Snack);
                BitBlt(hdc, j * mapE2 - 10, i * mapE1 - 10, 10, 10, memdc, 33, 33, SRCCOPY);//배경위에 원본
            }
        }
    if (result[6]) {
       Snack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_ballon));//풍선
       Mask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_ballonMask));
        for (i = 0; i < 20; i += 2)
            for (j = 0; j < 32; j += 6) {
                if (Packman[i][j] == 4) {
                    if (Packman[i][j + 1] == 1 && Packman[i + 1][j] == 1 && Packman[i - 2][j] == 1 && Packman[i][j - 2] == 1) {
                        Packman[i][j] = 1;
                        Packman[i - 1][j] = 1;
                        Packman[i][j - 1] = 1;
                        Packman[i - 1][j - 1] = 1;
                    }
                    SelectObject(memdc, Mask);
                    BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCAND); //배경위에 마스크
                    SelectObject(memdc, Snack);
                    BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCPAINT);//배경위에 원본
                }
            }
    }
   if (result[7]) {
        Snack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Ball));//공
        Mask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_BallMask));
        for (i = 0; i < 20; i += 2)
            for (j = 2; j < 32; j += 6) {
                if (Packman[i][j] == 4) {
                    if (Packman[i][j + 1] == 1 && Packman[i + 1][j] == 1 && Packman[i - 2][j] == 1 && Packman[i][j - 2] == 1) {
                        Packman[i][j] = 1;
                        Packman[i - 1][j] = 1;
                        Packman[i][j - 1] = 1;
                        Packman[i - 1][j - 1] = 1;
                    }
                   SelectObject(memdc, Mask);
                         BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCAND);//배경위에 마스크
                    SelectObject(memdc, Snack);
                    BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCPAINT);//배경위에 원본
                }
            }
   }
  if (result[8]) {
        Snack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_Fruit));//과일
        Mask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_obj_FruitMask));
        for (i = 0; i < 20; i += 2)
            for (j = 4; j < 32; j += 6) {
                if (Packman[i][j] == 4) {
                    if (Packman[i][j + 1] == 1 && Packman[i + 1][j] == 1 && Packman[i - 2][j] == 1 && Packman[i][j - 2] == 1) {
                        Packman[i][j] = 1;
                        Packman[i - 1][j] = 1;
                        Packman[i][j - 1] = 1;
                        Packman[i - 1][j - 1] = 1;
                    }
                   SelectObject(memdc, Mask);                   
                   BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCAND);//배경위에 원본
                    SelectObject(memdc, Snack);
                    BitBlt(hdc, j * mapE2 - 20, i * mapE1 - 20, snackSize1, snackSize2, memdc, 0, 0, SRCPAINT);//배경위에 원본
                }
            }
    }
   
    DeleteObject(Snack);
    DeleteObject(Mask);

}

int Counter(int packman[20][32]) {//먹은 오브젝트 개수 세는 함수
    int i = 0, j = 0, Score = 0;
    for (i = 0; i < 20; i += 2)
        for (j = 0; j < 32; j += 2)
            if (packman[i][j] == 3) {
                Score++;
            }
    return Score;
}

void PackmanAnimation(HDC hdc, int s)//팩맨 그리기
{
    HDC memdc;
    HBITMAP RunBit[2], Mask[2];
    static int count;
    int i, j;

    count++;
    count = count % 2;

    switch (s) {
    case 'L'://Left 왼쪽으로 갈때 PackmanLeft비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanLeft1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanLeft2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskLeft));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 'R'://Right 오른쪽으로 갈때 PackmanRightt비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskRight));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 'U'://Up 위로 갈때 PackmanUp비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanUp1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanUp2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskUp));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 'D'://Down 아래로 갈때 PackmanDown비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskDown));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 'B': // Back
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskRight));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    }
    memdc = CreateCompatibleDC(hdc);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++) {
            if (Packman[i][j] == 5) {//팩맨 그림을 4등분하여 왼쪽위을 영역 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }
            if (Packman[i][j + 1] == 6) {//팩맨 그림을 4등분하여 오른쪽위 영역을 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, 0, mapE1, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, 0, mapE1, SRCPAINT);//배경위에 원본
            }
            if (Packman[i + 1][j] == 7) {//팩맨 그림을 4등분하여 왼쪽아래 영역을 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1, mapE2, mapE1, memdc, mapE2, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1, mapE2, mapE1, memdc, mapE2, 0, SRCPAINT);//배경위에 원본
            }
            if (Packman[i + 1][j + 1] == 8) {//팩맨 그림을 4등분하여 오른쪽아래 영역을 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, mapE2, mapE1, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, mapE2, mapE1, SRCPAINT);//배경위에 원본
            }
        }

    for (i = 0; i < 2; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }
    DeleteDC(memdc);

}

void RedAnimation(HDC hdc)//빨간 몬스터 그리기
{
    HDC memdc;
    HBITMAP RunBit[3], Mask[3];
    static int count;
    int i, j;

    count++;
    count = count % 3;


    RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Red1));
    RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Red1));
    RunBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Red2));
    Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask1));
    Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask1));
    Mask[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask2));
             
    memdc = CreateCompatibleDC(hdc);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++) {
            if (Packman[i][j] == 15) {
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }

        }
    if (MEMORY)
        Packman[m][n] = 4;
    for (i = 0; i < 3; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }
    DeleteDC(memdc);

}

void PinkAnimation(HDC hdc)//분홍 몬스터 그리기
{
    HDC memdc;
    HBITMAP RunBit[3], Mask[3];
    static int count;
    int i, j;

    count++;
    count = count % 3;


    RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Pink2));
    RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Pink2));
    RunBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Pink1));
    Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask2));
    Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask2));
    Mask[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask1));

    memdc = CreateCompatibleDC(hdc);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++) {
            if (Packman[i][j] == 16) {
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }

        }

    for (i = 0; i < 3; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }
    DeleteDC(memdc);

}

void MintAnimation(HDC hdc)//민트색 몬스터 그리기
{
    HDC memdc;
    HBITMAP RunBit[3], Mask[3];
    static int count;
    int i, j;

    count++;
    count = count % 3;


    RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Mint1));
    RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Mint1));
    RunBit[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Mint2));
    Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask1));
    Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask1));
    Mask[2] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MonsterMask2));


    memdc = CreateCompatibleDC(hdc);

    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++) {
            if (Packman[i][j] == 17) {
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }

        }

    for (i = 0; i < 3; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }
    DeleteDC(memdc);

}

void RandomMap(void) {//맵 구성물 랜덤생생
    int i, j;

    for (i = 0; i < 20; i += 19)
        for (j = 0; j < 32; j++)
            Packman[i][j] = 1;
    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j += 31)
            Packman[i][j] = 1;
    for (i = 1; i < 19; i++)
        for (j = 1; j < 31; j++)
            Packman[i][j] = 0;
    for (i = 3; i < 16; i += 4)
        for (j = 3; j < 28; j += 4)
            Packman[i][j] = 9;

    srand((unsigned int)time(NULL));
    int a = 3 + (rand() % 3) * 4, b = 19 + (rand() % 3) * 4;;//ㅏ 난수를 이용하여 랜덤 좌표를 설정
    int a7 = 3 + (rand() % 3) * 4, b7 = 19 + (rand() % 3) * 4;;//ㅏ 모양 장애물

    int a4 = 3 + (rand() % 4) * 4, b4 = 19 + (rand() % 3) * 4;//ㅣ
    int a5 = 15, b5 = 3 + (rand() % 5) * 4;//ㅗ

    int a6 = 3, b6 = 3 + (rand() % 5) * 4;//ㅜ
    int a2 = 15, b2 = 3 + (rand() % 5) * 4;//ㅜ

    int a3 = 7, b3 = 7;//ㅓ
    int a8 = 11 + (rand() % 2) * 4, b8 = 27;//ㅓ

    int a1 = 7, b1 = 11;//ㅡ 
    int a9 = 11, b9 = 11;//ㅡ
    int a10 = 3 + (rand() % 3) * 4, b10 = 3 + (rand() % 5) * 4;//ㅡ
    int a11 = 3 + (rand() % 3) * 4, b11 = 3 + (rand() % 5) * 4;//ㅡ

    int a12 = 3, b12 = 3 + (rand() % 5) * 4;//ㅁ
    int a13 = 7, b13 = 3 + (rand() % 5) * 4;//ㅁ       
    int a14 = 7 + (rand() % 2) * 4, b14 = 19 + (rand() % 3) * 4;//-ㅁ
    int a15 = 15, b15 = 3 + (rand() % 5) * 4;//-

    if (Packman[a][b] == 9) {//ㅏ 모양 장애물 설정
        Packman[a][b] = 1; Packman[a + 1][b] = 1; Packman[a + 2][b] = 1; Packman[a + 3][b] = 1; Packman[a + 4][b] = 1; Packman[a + 5][b] = 1;

        Packman[a][b + 1] = 1; Packman[a + 1][b + 1] = 1; Packman[a + 2][b + 1] = 1; Packman[a + 3][b + 1] = 1; Packman[a + 4][b + 1] = 1; Packman[a + 5][b + 1] = 1;

        Packman[a + 2][b + 2] = 1; Packman[a + 3][b + 2] = 1; Packman[a + 2][b + 3] = 1; Packman[a + 3][b + 3] = 1;

    }
    if (Packman[a7][b7] == 9) {//ㅏ
        Packman[a7][b7] = 1;   Packman[a7 + 1][b7] = 1;   Packman[a7 + 2][b7] = 1;   Packman[a7 + 3][b7] = 1;   Packman[a7 + 4][b7] = 1;   Packman[a7 + 5][b7] = 1;

        Packman[a7][b7 + 1] = 1;   Packman[a7 + 1][b7 + 1] = 1;   Packman[a7 + 2][b7 + 1] = 1;   Packman[a7 + 3][b7 + 1] = 1;   Packman[a7 + 4][b7 + 1] = 1;   Packman[a7 + 5][b7 + 1] = 1;

        Packman[a7 + 2][b7 + 2] = 1;   Packman[a7 + 3][b7 + 2] = 1;   Packman[a7 + 2][b7 + 3] = 1;   Packman[a7 + 3][b7 + 3] = 1;
    }

    if (Packman[a1][b1] == 9) {//ㅡ 모양 장애물 설정
        Packman[a1][b1] = 1;   Packman[a1][b1 + 1] = 1;   Packman[a1][b1 + 2] = 1;   Packman[a1][b1 + 3] = 1;   Packman[a1][b1 + 4] = 1;   Packman[a1][b1 + 5] = 1;

        Packman[a1 + 1][b1] = 1;   Packman[a1 + 1][b1 + 1] = 1;    Packman[a1 + 1][b1 + 2] = 1;  Packman[a1 + 1][b1 + 3] = 1;   Packman[a1 + 1][b1 + 4] = 1;   Packman[a1 + 1][b1 + 5] = 1;
    }
    if (Packman[a9][b9] == 9) {//ㅡ
        Packman[a9][b9] = 1;   Packman[a9][b9 + 1] = 1;   Packman[a9][b9 + 2] = 1;   Packman[a9][b9 + 3] = 1;   Packman[a9][b9 + 4] = 1;   Packman[a9][b9 + 5] = 1;

        Packman[a9 + 1][b9] = 1;   Packman[a9 + 1][b9 + 1] = 1;   Packman[a9 + 1][b9 + 2] = 1;   Packman[a9 + 1][b9 + 3] = 1;   Packman[a9 + 1][b9 + 4] = 1;   Packman[a9 + 1][b9 + 5] = 1;
    }
    if (Packman[a10][b10] == 9) {//ㅡ
        Packman[a10][b10] = 1;     Packman[a10][b10 + 1] = 1; Packman[a10][b10 + 2] = 1; Packman[a10][b10 + 3] = 1; Packman[a10][b10 + 4] = 1; Packman[a10][b10 + 5] = 1;

        Packman[a10 + 1][b10] = 1; Packman[a10 + 1][b10 + 1] = 1; Packman[a10 + 1][b10 + 2] = 1; Packman[a10 + 1][b10 + 3] = 1; Packman[a10 + 1][b10 + 4] = 1; Packman[a10 + 1][b10 + 5] = 1;
    }
    if (Packman[a11][b11] == 9) {//ㅡ
        Packman[a11][b11] = 1; Packman[a11][b11 + 1] = 1; Packman[a11][b11 + 2] = 1; Packman[a11][b11 + 3] = 1; Packman[a11][b11 + 4] = 1; Packman[a11][b11 + 5] = 1;

        Packman[a11 + 1][b11] = 1; Packman[a11 + 1][b11 + 1] = 1; Packman[a11 + 1][b11 + 2] = 1; Packman[a11 + 1][b11 + 3] = 1; Packman[a11 + 1][b11 + 4] = 1; Packman[a11 + 1][b11 + 5] = 1;

    }
    if (Packman[a2][b2] == 9) {//ㅜ 모양 장애물 설정
        Packman[a2][b2] = 1;   Packman[a2][b2 + 1] = 1;   Packman[a2][b2 + 2] = 1;   Packman[a2][b2 + 3] = 1;   Packman[a2][b2 + 4] = 1;   Packman[a2][b2 + 5] = 1;

        Packman[a2 + 1][b2] = 1; Packman[a2 + 1][b2 + 1] = 1;   Packman[a2 + 1][b2 + 2] = 1;   Packman[a2 + 1][b2 + 3] = 1;   Packman[a2 + 1][b2 + 4] = 1;   Packman[a2 + 1][b2 + 5] = 1;

        Packman[a2 + 2][b2 + 2] = 1;   Packman[a2 + 2][b2 + 3] = 1;   Packman[a2 + 3][b2 + 2] = 1;   Packman[a2 + 3][b2 + 3] = 1;
    }
    if (Packman[a6][b6] == 9) {//ㅜ
        Packman[a6][b6] = 1;   Packman[a6][b6 + 1] = 1;   Packman[a6][b6 + 2] = 1;   Packman[a6][b6 + 3] = 1;   Packman[a6][b6 + 4] = 1;   Packman[a6][b6 + 5] = 1;

        Packman[a6 + 1][b6] = 1;   Packman[a6 + 1][b6 + 1] = 1;   Packman[a6 + 1][b6 + 2] = 1;   Packman[a6 + 1][b6 + 3] = 1;   Packman[a6 + 1][b6 + 4] = 1;   Packman[a6 + 1][b6 + 5] = 1;

        Packman[a6 + 2][b6 + 2] = 1;   Packman[a6 + 2][b6 + 3] = 1;   Packman[a6 + 3][b6 + 2] = 1;   Packman[a6 + 3][b6 + 3] = 1;
    }
    if (Packman[a3][b3] == 9) {//ㅓ 모양 장애물 설정
        Packman[a3][b3] = 1;   Packman[a3 + 1][b3] = 1;   Packman[a3 + 2][b3] = 1;   Packman[a3 + 3][b3] = 1;   Packman[a3 + 4][b3] = 1;   Packman[a3 + 5][b3] = 1;

        Packman[a3][b3 + 1] = 1;   Packman[a3 + 1][b3 + 1] = 1;   Packman[a3 + 2][b3 + 1] = 1;   Packman[a3 + 3][b3 + 1] = 1;   Packman[a3 + 4][b3 + 1] = 1;   Packman[a3 + 5][b3 + 1] = 1;

        Packman[a3 + 2][b3 - 1] = 1;   Packman[a3 + 2][b3 - 2] = 1;   Packman[a3 + 2][b3 - 3] = 1;   Packman[a3 + 2][b3 - 4] = 1;   Packman[a3 + 3][b3] = 1;   Packman[a3 + 3][b3 - 1] = 1;
        Packman[a3 + 3][b3 - 2] = 1;   Packman[a3 + 3][b3 - 3] = 1;   Packman[a3 + 3][b3 - 4] = 1;

    }
    if (Packman[a8][b8] == 9) {//ㅓ
        Packman[a8][b8] = 1;   Packman[a8 + 1][b8] = 1;   Packman[a8 + 2][b8] = 1;   Packman[a8 + 3][b8] = 1;   Packman[a8 + 4][b8] = 1;   Packman[a8 + 5][b8] = 1;

        Packman[a8][b8 + 1] = 1;   Packman[a8 + 1][b8 + 1] = 1;   Packman[a8 + 2][b8 + 1] = 1;   Packman[a8 + 3][b8 + 1] = 1;   Packman[a8 + 4][b8 + 1] = 1;   Packman[a8 + 5][b8 + 1] = 1;

        Packman[a8 + 2][b8 - 1] = 1;   Packman[a8 + 2][b8 - 2] = 1;   Packman[a8 + 2][b8 - 3] = 1;   Packman[a8 + 2][b8 - 4] = 1;   Packman[a8 + 3][b8] = 1;   Packman[a8 + 3][b8 - 1] = 1;
        Packman[a8 + 3][b8 - 2] = 1;   Packman[a8 + 3][b8 - 3] = 1;   Packman[a8 + 3][b8 - 4] = 1;
    }
    if (Packman[a4][b4] == 9) {//ㅣ//모양 장애물 설정
        Packman[a4][b4] = 1;   Packman[a4 + 1][b4] = 1;   Packman[a4 + 2][b4] = 1;   Packman[a4 + 3][b4] = 1;   Packman[a4 + 4][b4] = 1;   Packman[a4 + 5][b4] = 1;

        Packman[a4][b4 + 1] = 1;   Packman[a4 + 1][b4 + 1] = 1;   Packman[a4 + 2][b4 + 1] = 1;   Packman[a4 + 3][b4 + 1] = 1;   Packman[a4 + 4][b4 + 1] = 1;   Packman[a4 + 5][b4 + 1] = 1;
    }
    if (Packman[a5][b5] == 9) {//- 모양 장애물 설정
        Packman[a4][b4] = 1;   Packman[a4][b4 + 1] = 1;   Packman[a4][b4 + 2] = 1;   Packman[a4][b4 + 3] = 1;   Packman[a4 + 1][b4] = 1;   Packman[a4 + 1][b4 + 1] = 1;   Packman[a4 + 1][b4 + 2] = 1;   Packman[a4 + 1][b4 + 3] = 1;
    }
    if (Packman[a5][b5] == 9) {//ㅗ 모양 장애물 설정

        Packman[a5][b5] = 1;   Packman[a5][b5 + 1] = 1;   Packman[a5][b5 + 2] = 1;   Packman[a5][b5 + 3] = 1;   Packman[a5][b5 + 4] = 1;   Packman[a5][b5 + 5] = 1;

        Packman[a5 + 1][b5] = 1;   Packman[a5 + 1][b5 + 1] = 1;   Packman[a5 + 1][b5 + 2] = 1;   Packman[a5 + 1][b5 + 3] = 1;   Packman[a5 + 1][b5 + 4] = 1;   Packman[a5 + 1][b5 + 5] = 1;

        Packman[a5 - 1][b5 + 2] = 1;   Packman[a5 - 1][b5 + 3] = 1;   Packman[a5 - 2][b5 + 2] = 1;   Packman[a5 - 2][b5 + 3] = 1;
    }
    if (Packman[a12][b12] == 9) {//ㅁ 모양 장애물 설정
        Packman[a12][b12] = 1; Packman[a12][b12 + 1] = 1; Packman[a12][b12 + 2] = 1; Packman[a12][b12 + 3] = 1; Packman[a12 + 1][b12] = 1; Packman[a12 + 1][b12 + 1] = 1; Packman[a12 + 1][b12 + 2] = 1;
        Packman[a12 + 1][b12 + 3] = 1; Packman[a12 + 2][b12] = 1; Packman[a12 + 2][b12 + 1] = 1; Packman[a12 + 2][b12 + 2] = 1; Packman[a12 + 2][b12 + 3] = 1; Packman[a12 + 3][b12] = 1;
        Packman[a12 + 3][b12 + 1] = 1; Packman[a12 + 3][b12 + 2] = 1; Packman[a12 + 3][b12 + 3] = 1;
    }

    if (Packman[a13][b13] == 9) {//ㅁ
        Packman[a13][b13] = 1; Packman[a13][b13 + 1] = 1; Packman[a13][b13 + 2] = 1; Packman[a13][b13 + 3] = 1; Packman[a13 + 1][b13] = 1; Packman[a13 + 1][b13 + 1] = 1; Packman[a13 + 1][b13 + 2] = 1;
        Packman[a13 + 1][b13 + 3] = 1; Packman[a13 + 2][b13] = 1; Packman[a13 + 2][b13 + 1] = 1; Packman[a13 + 2][b13 + 2] = 1; Packman[a13 + 2][b13 + 3] = 1; Packman[a13 + 3][b13] = 1;
        Packman[a13 + 3][b13 + 1] = 1; Packman[a13 + 3][b13 + 2] = 1; Packman[a13 + 3][b13 + 3] = 1;
    }
    if (Packman[a14][b14] == 9) {//ㅁ
        Packman[a14][b14] = 1; Packman[a14][b14 + 1] = 1; Packman[a14][b14 - 1] = 1;  Packman[a14][b14 - 2] = 1;  Packman[a14 + 1][b14] = 1; Packman[a14 + 1][b14 + 1] = 1;
        Packman[a14 + 1][b14 - 1] = 1;  Packman[a14 + 1][b14 - 2] = 1;  Packman[a14 + 2][b14] = 1; Packman[a14 + 2][b14 + 1] = 1; Packman[a14 + 2][b14 - 1] = 1;  Packman[a14 + 2][b14 - 2] = 1;
        Packman[a14 + 3][b14] = 1; Packman[a14 + 3][b14 + 1] = 1; Packman[a14 + 3][b14 - 1] = 1;  Packman[a14 + 3][b14 - 2] = 1;
    }
    if (Packman[a15][b15] == 9) {//- 뒤로 장애물 설정
        Packman[a15][b15] = 1; Packman[a15][b15 + 1] = 1; Packman[a15][b15 - 1] = 1;  Packman[a15][b15 - 2] = 1;  Packman[a15 + 1][b15] = 1;
        Packman[a15 + 1][b15 + 1] = 1; Packman[a15 + 1][b15 - 1] = 1; Packman[a15 + 1][b15 - 2] = 1;

    }
    for (int i = 0; i < 20; i += 2)
        for (int j = 0; j < 32; j += 2)
            if (Packman[i][j] == 0) {
                Packman[i][j] = 4;//장애물 구성 후 남은 공간에 과자 배치
                ScoreCount++;//과자수 
            }


}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc, mem1dc, mem2dc;
    PAINTSTRUCT ps;
    static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
    static int x, y;
    static int m1, n1, m2, n2, m3, n3;//몬스터들의 좌표 mn1:빨강, mn2: 분홍, mn3 : 민트

    static RECT rectView;
    static char s;//방향설정 변수
    static char M1, M2, M3;//몬스터들의 방향설정 변수 M1: 빨강, M2 : 분홍, M3 : 민트
    static BOOL FLAG = FALSE;//InvalidateRgn시 깜박이지 않게 하기위하고 게임 종료시 화면 전환을 위한 변수

    static int count_time = 60; // 1분간 게임 가능하게하는 변수
    static TCHAR time_announcer[1024], time_announcer2[1024], resultScore[1024], resultScore2[1024], /*초기 화면 문구 변수 -> */initial[1024], initial2[1024]; // drawtext할 문자열
    static int time_announcer_len, time_announcer_len2, resultScore_len, resultScore_len2, initial_len, initial_len2; // drawtext에서 문자열 길이를 넘겨줄 변수 
    RECT time_announcer_size;   // 게임 실행 중 문구의 위치 
    time_announcer_size.left = 1155;
    time_announcer_size.top = 50;
    time_announcer_size.right = 1600;
    time_announcer_size.bottom = 80;

    RECT time_announcer_size2;   // 게임 실행 중 문구의 위치2
    time_announcer_size2.left = 1290;
    time_announcer_size2.top = 90;
    time_announcer_size2.right = 1500;
    time_announcer_size2.bottom = 120;

    RECT resultScore_size; // 게임 종료시 출력되는 문구의 크기 
    resultScore_size.left = 550;
    resultScore_size.top = 380;
    resultScore_size.right = 950;
    resultScore_size.bottom = 480;

    RECT resultScore_size2; // 게임 종료시 출력되는 문구의 두 번째 줄 크기
    resultScore_size2.left = 550;
    resultScore_size2.top = 480;
    resultScore_size2.right = 950;
    resultScore_size2.bottom = 540;

    RECT initial_size; // 게임 시작시 출력되는 문구의 크기 
    initial_size.left = 450;
    initial_size.top = 380;
    initial_size.right = 1050;
    initial_size.bottom = 480;

    RECT initial_size2; // 게임 시작시 출력되는 문구의 두번째 줄 크기 
    initial_size2.left = 450;
    initial_size2.top = 480;
    initial_size2.right = 1050;
    initial_size2.bottom = 580;

    static int game_state = 0; // 1분이 지나면 2로 바뀌고 게임 종료 

    HFONT hFont, oldFont;

    initial_len = wsprintf(initial, TEXT("PAC-MAN GAME")); // 초기 화면 문구 
    initial_len2 = wsprintf(initial2, TEXT("'ENTER 를 눌러 게임 시작'")); // 초기 화면 문구2 

    TCHAR str[100], lpstrFile[100] = _T(""), lpstrFileTitle[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg,.jpeg)\0*.jpg;*.jpeg\0PNG(.png)\0*.png\0");
    
        switch (message)
    {
    case WM_CREATE:
        PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP); // 소리 내는 함수
        AddFontResourceA("C:\\Windows\\Fonts\\NEXONFootballGothicB.ttf"); // 폰트 불러오기 

        SetTimer(hWnd, 2, 1000, NULL); // 제한시간용 타이머
        GetClientRect(hWnd, &rectView);
        x = 1; y = 1;// 팩맨 처음 좌표
        m1 = 9; n1 = 13;
        m2 = 9; n2 = 15;
        m3 = 9; n3 = 17;//몬스터들의 처음 좌표

        Packman[y][x] = 5;//팩맨 그림을 4등분하여 왼쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 5로 설정
        Packman[y][x + 1] = 6;//팩맨 그림을 4등분하여  오른쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 6로 설정
        Packman[y + 1][x] = 7;//팩맨 그림을 4등분하여 왼쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 7로 설정
        Packman[y + 1][x + 1] = 8;//팩맨 그림을 4등분하여 오른쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 8로 설정

        Packman[m1][n1] = 15;//빨간 몬스터 줄력하기 위해 15로 설정

        Packman[m2][n2] = 16;//분홍 몬스터 줄력하기 위해 15로 설정

        Packman[m3][n3] = 17;//민트 몬스터 줄력하기 위해 15로 설정

        s = 'R';//팩맨의 기본모습
        M1 = 1;
        M2 = 2;
        M3 = 3;//몬스터들의 기본 모습

        RandomMap();
    
        hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background));// 배경 비트맵 저장

        break;

    case WM_PAINT:

        switch (game_state) { // 60초가 다되면 false로 만들어 게임을 끝낸다 
        case 0: // 게임 실행 시 첫 화면 출력
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 61))); // 배경화면 출력 
            GetClientRect(hWnd, &rectView);
            Rectangle(hdc, 0, 0, rectView.right, rectView.bottom);

            /* 첫 화면  문구 출력되는 부분 */
            SetBkMode(hdc, TRANSPARENT); // 글자 배경을 투명하게 한다 
            SetTextColor(hdc, RGB(255, 255, 0));
            hFont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, initial, initial_len, &initial_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, initial2, initial_len2, &initial_size2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            break;
        case 1: // 게임 실행중 화면 출력 
            hdc = BeginPaint(hWnd, &ps);
            mem1dc = CreateCompatibleDC(hdc);
            mem2dc = CreateCompatibleDC(mem1dc);
            GetClientRect(hWnd, &rectView);
            if (hBit1 == NULL)
                hBit1 = CreateCompatibleBitmap(hdc, rectView.right, rectView.bottom);
            oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
            oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
            MakeMap(mem2dc);
            BitBlt(mem1dc, 0, 0, 1280, 960, mem2dc, 0, 0, SRCCOPY);//배경과 장애물 출력 
            Snack(mem1dc);//과자 그리기         
            RedAnimation(mem1dc);//빨간 몬스터 그리기
            PinkAnimation(mem1dc);//분홍 몬스터 그리기
            MintAnimation(mem1dc);//민트 몬스터 그리기
            PackmanAnimation(mem1dc, s); //팩맨 출력 
            BitBlt(hdc, 0, 0, 1280, 960, mem1dc, 0, 0, SRCCOPY); //배경위에 그린것 출력 
            SelectObject(mem1dc, oldBit1);
            SelectObject(mem2dc, oldBit2);
            DeleteObject(mem2dc);
            DeleteObject(mem1dc);

            /* 게임 실행 중 문구 출력되는 부분 */
            SetTextColor(hdc, RGB(255, 255, 0));
            hFont = CreateFont(25, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            SetBkColor(hdc, RGB(0, 0, 61)); // 글자 배경을 투명하게 한다
            DrawText(hdc, time_announcer, time_announcer_len, &time_announcer_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, time_announcer2, time_announcer_len2, &time_announcer_size2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            break;

        case 2: // 게임 종료시 화면 출력
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 61)));
            GetClientRect(hWnd, &rectView);
            Rectangle(hdc, 0, 0, rectView.right, rectView.bottom);

            /*게임 종료시 문구 출력하는 부분*/
            SetBkMode(hdc, TRANSPARENT); // 글자 배경을 투명하게 한다 
            SetTextColor(hdc, RGB(255, 255, 0));
            hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, resultScore, resultScore_len, &resultScore_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, resultScore2, resultScore_len2, &resultScore_size2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            break;
        }
        break;
    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지

        if (game_state == 0) {

            memset(&OFN, 0, sizeof(OPENFILENAME));
            OFN.lStructSize = sizeof(OPENFILENAME);
            OFN.hwndOwner = hWnd;
            OFN.lpstrFilter = filter;
            OFN.lpstrFile = lpstrFile; // 파일의 경로
            OFN.nMaxFile = 100;
            OFN.lpstrInitialDir = _T(".");
            OFN.lpstrFileTitle = lpstrFileTitle; // 파일명 저장
            OFN.nMaxFileTitle = 100;
            if (GetOpenFileName(&OFN) != 0) {
                _stprintf_s(imageName, _T("/api/%s"), lpstrFileTitle); // label을 가져오기 위해 필요한 파일명 저장
                _stprintf_s(imageNameUrl, _T("/api/uploads/%s"), lpstrFileTitle); // url을 가져오기 위해 필요한 파일명 저장
                _stprintf_s(str, _T("%s 파일이 성공적으로 업로드 되었습니다."), lpstrFile);

                uploadImage(lpstrFile, imageNameUrl); // 이미지 업로드

                MessageBox(hWnd, str, _T("업로드 확인"), MB_OK);
            }

            getImageLabels(imageName, labels); // 라벨링 데이터 가져와 labels에 저장

            IsItemInLabels(result, labels); // 라벨링 데이터에서 맵에 표현가능한 오브젝트 확인
           
            switch (wParam) {
            case VK_RETURN:
                game_state = 1; // 엔터 누르면 게임 시작 
                break;
            }
        }
        if (game_state == 1) {
            SetTimer(hWnd, 1, 100, NULL);

            switch (wParam) { // 키보드가 눌렸을때 wParam에 값이 저장된다 값과 비교하여 switch 문에 진입한다
            case VK_LEFT: // 왼쪽 화살표
                s = 'L';//왼쪽
                break;
            case VK_RIGHT: // 오른쪽 화살표
                s = 'R';//오른쪽
                break;
            case VK_UP:
                s = 'U';//위쪽  
                break;
            case VK_DOWN:
                s = 'D';//아래쪽상수
                break;
            case VK_ESCAPE://ESC누르면 맵재생성/ 재시작
                RandomMap();
                hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background));
                score = 0;
                count_time = 60;
                Clear_Map(Packman);
                
                SetTimer(hWnd, 2, 1000, NULL); // 제한시간용 타이머 재시작
                x = 1; y = 1;// 팩맨 처음 좌표
                m1 = 9; n1 = 13;
                m2 = 9; n2 = 15;
                m3 = 9; n3 = 17;//몬스터들의 처음 좌표

                Packman[y][x] = 5;//팩맨 그림을 4등분하여 왼쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 5로 설정
                Packman[y][x + 1] = 6;//팩맨 그림을 4등분하여  오른쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 6로 설정
                Packman[y + 1][x] = 7;//팩맨 그림을 4등분하여 왼쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 7로 설정
                Packman[y + 1][x + 1] = 8;//팩맨 그림을 4등분하여 오른쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 8로 설정

                Packman[m1][n1] = 15;//빨간 몬스터 줄력하기 위해 15로 설정

                Packman[m2][n2] = 16;//분홍 몬스터 줄력하기 위해 15로 설정

                Packman[m3][n3] = 17;//민트 몬스터 줄력하기 위해 15로 설정

                s = 'R';//팩맨의 기본모습
                M1 = 2;
                M2 = 2;
                M3 = 2;//몬스터들의 기본 모습
                FLAG = FALSE;
                game_state = 1; // 엔터 누르면 게임 다시 시작 
                break;
            }
        }
        if (game_state == 2) { // 게임 종료 시 
            KillTimer(hWnd, 1); // 타이머 제거 
            KillTimer(hWnd, 2);
            score = 0;          // 점수와 시간 초기화 
            count_time = 60;

            switch (wParam) {
            case VK_RETURN:
                Clear_Map(Packman);
                SetTimer(hWnd, 2, 1000, NULL); // 제한시간용 타이머 재시작
                x = 1; y = 1;// 팩맨 처음 좌표
                m1 = 9; n1 = 13;
                m2 = 9; n2 = 15;
                m3 = 9; n3 = 17;//몬스터들의 처음 좌표

                Packman[y][x] = 5;//팩맨 그림을 4등분하여 왼쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 5로 설정
                Packman[y][x + 1] = 6;//팩맨 그림을 4등분하여  오른쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 6로 설정
                Packman[y + 1][x] = 7;//팩맨 그림을 4등분하여 왼쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 7로 설정
                Packman[y + 1][x + 1] = 8;//팩맨 그림을 4등분하여 오른쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 8로 설정

                Packman[m1][n1] = 15;//빨간 몬스터 줄력하기 위해 15로 설정

                Packman[m2][n2] = 16;//분홍 몬스터 줄력하기 위해 15로 설정

                Packman[m3][n3] = 17;//민트 몬스터 줄력하기 위해 15로 설정

                s = 'R';//팩맨의 기본모습
                M1 = 2;
                M2 = 2;
                M3 = 2;//몬스터들의 기본 모습
                FLAG = FALSE;
                game_state = 1; // 엔터 누르면 게임 다시 시작 
                break;
            }
        }
        break;

    case WM_TIMER:
        if (game_state == 1) {
            switch (wParam) // 타이머의 id가 1일때와 2일때의 동작 
            {
            case 1: // 타이머가 1일때
                Packman[y][x] = 3;
                Packman[y][x + 1] = 3;
                Packman[y + 1][x] = 3;
                Packman[y + 1][x + 1] = 3;//팩맨이 중복되어 출력되지 않게하기위해와 과자 출력하지않고 스코어를 위한 변수로 0이아닌 3으로 설정

                switch (s) {
                case 'L'://Left
                    x--;
                    if (Packman[y][x] == 1 || Packman[y + 1][x] == 1)//배열의 내용이 1(장애물)일때 위치 복구
                        x++;
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 'R'://Right
                    x++;
                    if (Packman[y][x + 1] == 1 || Packman[y + 1][x + 1] == 1)
                        x--;
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 'U'://Up
                    y--;
                    if (Packman[y][x] == 1 || Packman[y][x + 1] == 1)
                        y++;
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 'D'://Down
                    y++;
                    if (Packman[y + 1][x] == 1 || Packman[y + 1][x + 1] == 1)
                        y--;
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 'B'://Back
                    x = 1; y = 1;
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }

                Packman[y][x] = 5;
                Packman[y][x + 1] = 6;
                Packman[y + 1][x] = 7;
                Packman[y + 1][x + 1] = 8;//옮겨진 좌표에 팩맨을 그리기 위해 변수 저장
                            
                
                Packman[m1][n1] = 0;//과자에 영향을 주지 않기 위해 0으로 설정

                switch (M1) {//1:왼, 2: 오른, 3:위, 4:아래 빨간몬스터
                case 1://Left
                    n1--;
                    if (Packman[m1][n1] == 1 || Packman[m1 + 1][n1] == 1) {  
                        M2= 4;
                        n1++;//배열의 내용이 1(장애물)일때 위치 복구     
                        M1 = (rand() % 4) + 1;//배열의 내용이 1(장애물)일때 랜덤으로 방향 설정 모든 방향 다 같은 방식으로 작동
                    }
                    if (Packman[m1][n1] == 5 || Packman[m1 + 1][n1] == 6 || Packman[m1][n1] == 7 || Packman[m1 + 1][n1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 2://Right
                    n1++;
                    if (Packman[m1][n1 + 1] == 1 || Packman[m1 + 1][n1 + 1] == 1) {  
                        M2 = 3;
                        n1--;
                        M1 = (rand() % 4) + 1;
                    }
                    if (Packman[m1][n1 + 1] == 5 || Packman[m1 + 1][n1 + 1] == 6 || Packman[m1][n1 + 1] == 7 || Packman[m1 + 1][n1 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3:  //UP        
                    m1--;
                    if (Packman[m1][n1] == 1 || Packman[m1][n1 + 1] == 1) {
                        M2 = 1;
                        m1++;
                        M1 = (rand() % 4) + 1;
                    }
                    if (Packman[m1][n1] == 5 || Packman[m1][n1 + 6] == 12 || Packman[m1][n1] == 7 || Packman[m1][n1 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 4://Down
                    m1++;
                    if (Packman[m1 + 1][n1] == 1 || Packman[m1 + 1][n1 + 1] == 1) {         
                        M2 = 2;
                        m1--;
                        M1 = (rand() % 4) + 1;
                    }
                    if (Packman[m1 + 1][n1] == 5 || Packman[m1 + 1][n1 + 1] == 6 || Packman[m1 + 1][n1] == 7 || Packman[m1 + 1][n1 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료                       
                            FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }
                if (Packman[m1][n1] == 4) {
                    m = m1;
                    n = n1;
                    MEMORY = TRUE;
                }
                   
                Packman[m1][n1] = 15;//빨간 몬스터 위치 조정

               
                Packman[m2][n2] = 0;//과자에 영향을 주지 않기 위해 0으로 설정

                switch (M2) {//분홍 몬스터
                case 1://Left
                    n2--;
                    if (Packman[m2][n2] == 1 || Packman[m2 + 1][n2] == 1) {//배열의 내용이 1(장애물)일때 위치 복구 
                        M3 = 3;
                        n2++;
                        M2 = (rand() % 4) + 1;
                    }
                    if (Packman[m2][n2] == 5 || Packman[m2 + 1][n2] == 6 || Packman[m2][n2] == 7 || Packman[m2 + 1][n2] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료

                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 2://Right
                    n2++;
                    if (Packman[m2][n2 + 1] == 1 || Packman[m2 + 1][n2 + 1] == 1) {
                        M3 = 4;
                        n2--;
                        M2 = (rand() % 4) + 1;
                    }
                    if (Packman[m2][n2 + 1] == 5 || Packman[m2 + 1][n2 + 1] == 6 || Packman[m2][n2 + 1] == 7 || Packman[m2 + 1][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료

                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3://UP  
                    m2--;
                    if (Packman[m2][n2] == 1 || Packman[m2][n2 + 1] == 1) {      
                        M3 = 1;
                        m2++;
                        M2 = (rand() % 4) + 1;
                    }
                    if (Packman[m2][n2] == 5 || Packman[m2][n2 + 6] == 12 || Packman[m2][n2] == 7 || Packman[m2][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료

                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 4://Down
                    m2++;
                    if (Packman[m2 + 1][n2] == 1 || Packman[m2 + 1][n2 + 1] == 1) { 
                        M3 = 2;
                        m2--;
                        M2 = (rand() % 4) + 1;
                    }
                    if (Packman[m2 + 1][n2] == 5 || Packman[m2 + 1][n2 + 1] == 6 || Packman[m2 + 1][n2] == 7 || Packman[m2 + 1][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료

                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }
                if (Packman[m2][n2] == 4) {
                    m = m2;
                    n = n2;
                    MEMORY = TRUE;
                }

                Packman[m2][n2] = 16;//분홍 몬스터 위치조정
                
                Packman[m3][n3] = 0;//과자에 영향을 주지 않기 위해 0으로 설정

                switch (M3) {//민트 몬스터
                case 1://Left
                    n3--;
                    if (Packman[m3][n3] == 1 || Packman[m3 + 1][n3] == 1) {//배열의 내용이 1(장애물)일때 위치 복구
                        M1 = 4;
                        n3++;
                        M3 = (rand() % 4) + 1;
                    }
                    if (Packman[m3][n3] == 5 || Packman[m3 + 1][n3] == 6 || Packman[m3][n3] == 7 || Packman[m3 + 1][n3] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 2://Right
                    n3++;
                    if (Packman[m3][n3 + 1] == 1 || Packman[m3 + 1][n3 + 1] == 1) {
                        M1 = 3;
                        n3--;
                        M3 = (rand() % 4) + 1;
                    }
                    if (Packman[m3][n3 + 1] == 5 || Packman[m3 + 1][n3 + 1] == 6 || Packman[m3][n3 + 1] == 7 || Packman[m3 + 1][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3://UP  
                    m3--;
                    if (Packman[m3][n3] == 1 || Packman[m3][n3 + 1] == 1) {  
                        M1 = 2;
                        m3++;
                        M3 = (rand() % 4) + 1;
                    }
                    if (Packman[m3][n3] == 5 || Packman[m3][n3 + 6] == 12 || Packman[m3][n3] == 7 || Packman[m3][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 4://Down
                    m3++;
                    if (Packman[m3 + 1][n3] == 1 || Packman[m3 + 1][n3 + 1] == 1) {   
                        M1 = 1;
                        m3--;
                        M3 = (rand() % 4) + 1;
                    }
                    if (Packman[m3 + 1][n3] == 5 || Packman[m3 + 1][n3 + 1] == 6 || Packman[m3 + 1][n3] == 7 || Packman[m3 + 1][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }
                if (Packman[m3][n3] == 4) {
                    m = m3;
                    n = n3;
                    MEMORY = TRUE;
                }

                Packman[m3][n3] = 17;//민트 위치 조정


                if (count_time < 0 || score >= ScoreCount) { // 시간이 0보다 작아지거나 과자 다 먹을 시
                    game_state = 2; // false로 만들어 게임 종료 
                    FLAG = TRUE;
                }
                break;
            case 2: // 타이머가 2일때
                if (count_time >= 0)
                {
                    count_time = count_time--; // 1초씩 감소 
                }
                score = Counter(Packman);

                resultScore_len = wsprintf(resultScore, TEXT("게임종료    SCORE:  %d"), score); // 게임 중 등장하는 문구 설정하는 곳 
                resultScore_len2 = wsprintf(resultScore2, TEXT("Enter를 눌러 다시하기"));
                time_announcer_len = wsprintf(time_announcer, TEXT("남은시간: %d"), count_time);
                time_announcer_len2 = wsprintf(time_announcer2, TEXT("SCORE: %d"), score);
                InvalidateRgn(hWnd, NULL, FLAG);
                break;
            }
        }
        return 0;

    case WM_DESTROY:
        if (hBit1)
            DeleteObject(hBit1);

        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}