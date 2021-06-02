#define _CRT_SECURE_NO_WARNINGS

// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include "framework.h"
#include "ImageBasedMapPacman.h"
#include <wchar.h>
#include "ImageLabels.h"
#include "mmsystem.h" // 음악 재생을 위해 인클루드 

#pragma comment(lib,"winmm.lib") // lib 파일을 읽어들이는 메크로 

#define MAX_LOADSTRING 100

//함수들 선언
HDC MakeMap(HDC hdc);
HDC Animation(HDC mem1dc, int s);
HDC Snack(HDC hdc);
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
wchar_t items[3][100] = { L"Dog", L"Human", L"Person" }; // 맵에 표현될 수 있는 오브젝트 목록

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
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 애플리케이션 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//


int packman[20][32] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,4,1,1,0,4,0,4,1,1,1,1,1,1,0,4,1,1,1,1,1,1,0,4,1,1,1,1,0,4,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,4,1,1,1,1,0,4,0,4,0,4,0,4,0,4,0,4,1,1,0,4,0,4,0,4,0,4,0,4,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,4,1,1,0,4,4,4,0,4,1,1,0,4,1,1,0,4,0,4,0,4,1,1,0,4,0,4,0,4,1},
    {1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1},
    {1,0,4,0,4,0,4,1,1,1,1,1,1,0,4,1,1,0,4,1,1,1,1,1,1,0,4,1,1,1,1,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,4,1,1,0,4,0,4,0,4,1,1,0,4,1,1,0,4,0,4,0,4,1,1,0,4,0,4,0,4,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,4,1,1,1,1,0,4,0,4,0,4,0,4,0,4,0,4,1,1,0,4,0,4,0,4,0,4,0,4,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,4,1,1,0,4,0,4,1,1,1,1,1,1,0,4,1,1,1,1,1,1,0,4,1,1,1,1,0,4,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,0,4,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};// 1 : 벽 , 0 : 빈공간 , 4: 점수 오브젝트 
int mapE1 = 48;//장애물의 세로(960/10)
int mapE2 = 40;//장애물의 가로(1280/16)
int snackSize = 10;//먹는 오브젝트 크기
static int score = 0;//출력 스코어 저장변수

HINSTANCE g_hInst;

HDC MakeMap(HDC hdc) //맵 장애물 표시
{
    HDC memdc;
    HBITMAP hBit;
    int i, j;

    memdc = CreateCompatibleDC(hdc);
    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Map));

    SelectObject(memdc, hBit);
    for (i = 0; i < 20; i++)
        for (j = 0; j < 32; j++)
            if (packman[i][j] == 1)
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);

    DeleteObject(hBit);
    DeleteDC(memdc);

    return hdc;
}

HDC Snack(HDC hdc) {    // 과자 그리기 함수 
    HDC memdc;
    HBITMAP Snack, Mask;
    int i, j;

    memdc = CreateCompatibleDC(hdc);
    Snack = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown2));
    Mask = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));

    for (i = 0; i < 20; i += 2)
        for (j = 0; j < 32; j += 2) {
            if (packman[i][j] == 0) {
                SelectObject(memdc, Mask);
                BitBlt(hdc, j * mapE2 - 10, i * mapE1 - 10, snackSize/*10*/, snackSize, memdc, 33, 33, SRCAND);//배경위에 마스크
                SelectObject(memdc, Snack);
                BitBlt(hdc, j * mapE2 - 10, i * mapE1 - 10, snackSize, snackSize, memdc, 33, 33, SRCPAINT);//배경위에 원본
            }
        }

    DeleteObject(Snack);
    DeleteObject(Mask);

    return hdc;
}

void IsItemInLabels(int* result, wchar_t* labels)
{
    wchar_t* pwc;
    wchar_t* pt;

    pwc = wcstok(labels, L" ,", &pt);
    while (pwc != NULL)
    {
        for (int i = 0; i < 3; i++) {

            if (wcsstr(pwc, items[i]) != NULL) {
                result[i] = 1;
            }
        }
        
        pwc = wcstok(NULL, L" ,", &pt);
    }

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

HDC Animation(HDC hdc, int s)//팩맨 그리기
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
            if (packman[i][j] == 5) {//팩맨 그림을 4등분하여 왼쪽위을 영역 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }
            if (packman[i][j + 1] == 6) {//팩맨 그림을 4등분하여 오른쪽위 영역을 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, 0, mapE1, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1 + mapE1, mapE2, mapE1, memdc, 0, mapE1, SRCPAINT);//배경위에 원본
            }
            if (packman[i + 1][j] == 7) {//팩맨 그림을 4등분하여 왼쪽아래 영역을 배경에 저장
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1, mapE2, mapE1, memdc, mapE2, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2 + mapE2, i * mapE1, mapE2, mapE1, memdc, mapE2, 0, SRCPAINT);//배경위에 원본
            }
            if (packman[i + 1][j + 1] == 8) {//팩맨 그림을 4등분하여 오른쪽아래 영역을 배경에 저장
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
    return hdc;
}


HDC RedAnimation(HDC hdc)//빨간 몬스터 그리기
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
            if (packman[i][j] == 15) {
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2 , mapE1 * 2, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2 * 2, mapE1 * 2, memdc, 0, 0, SRCPAINT);//배경위에 원본
            }
           
        }

    for (i = 0; i < 3; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }
    DeleteDC(memdc);
    return hdc;
}
HDC PinkAnimation(HDC hdc)//분홍 몬스터 그리기
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
            if (packman[i][j] == 16) {
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
    return hdc;
}

HDC MintAnimation(HDC hdc)//민트색 몬스터 그리기
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
            if (packman[i][j] == 17) {
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
    return hdc;
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
    static BOOL FLAG = FALSE;

    static int count_time = 60; // 1분간 게임 가능하게하는 변수
    static TCHAR time_announcer[1024], time_announcer2[1024], resultScore[1024], /*초기 화면 문구 변수 -> */initial[1024], initial2[1024]; // drawtext할 문자열
    static int time_announcer_len, time_announcer_len2, resultScore_len, initial_len, initial_len2; // drawtext에서 문자열 길이를 넘겨줄 변수 
    RECT time_announcer_size;   // drawtext 크기 (남은시간, 현재 점수 출력) 
    time_announcer_size.left = 1250;
    time_announcer_size.top = 10;
    time_announcer_size.right = 1500;
    time_announcer_size.bottom = 50;

    RECT time_announcer_size2;   // drawtext 크기 (남은시간, 현재 점수 출력) 
    time_announcer_size2.left = 1280;
    time_announcer_size2.top = 50;
    time_announcer_size2.right = 1500;
    time_announcer_size2.bottom = 100;
    
    RECT resultScore_size; // 게임 종료시 출력되는 문구의 크기 
    resultScore_size.left = 550;
    resultScore_size.top = 380;
    resultScore_size.right = 950;
    resultScore_size.bottom = 480;

    RECT initial_size; // 게임 종료시 출력되는 문구의 크기 
    initial_size.left = 350;
    initial_size.top = 380;
    initial_size.right = 1050;
    initial_size.bottom = 480;
    
    RECT initial_size2; // 게임 시작시 출력되는 문구의 크기2 
    initial_size2.left = 350;
    initial_size2.top = 480;
    initial_size2.right = 1050;
    initial_size2.bottom = 580;

    static int game_state = 0; // 1분이 지나면 2로 바뀌고 게임 종료 

    HFONT hFont, oldFont;

    initial_len = wsprintf(initial, TEXT("PAC-MAN GAME")); // 초기 화면 문구 
    initial_len2 = wsprintf(initial2, TEXT("'ENTER 를 눌러 게임 시작'")); // 초기 화면 문구2 

    
    TCHAR str[100], lpstrFile[100] = _T(""), lpstrFileTitle[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg,.jpeg)\0*.jpg;*.jpeg\0PNG(.png)\0*.png\0");

    /**
       * imageName: /api/ + 사진 파일 이름
       * labels: 사진 라벨링 데이터가 저장될 배열
    */
    static wchar_t imageName[100];
    static wchar_t imageNameUrl[100];
    static wchar_t labels[500];
    static int result[3] = { 0, }; // 오브젝트 목록 여부

    switch (message)
    {
    case WM_CREATE:
        PlaySound(MAKEINTRESOURCE(IDR_WAVE1), g_hInst, SND_RESOURCE | SND_ASYNC | SND_LOOP); // 소리 내는 함수
        AddFontResourceA("C:\Windows\Fonts\NEXONFootballGothicB.ttf"); // 폰트 불러오기 

        SetTimer(hWnd, 2, 1000, NULL); // 제한시간용 타이머
        GetClientRect(hWnd, &rectView);
        x = 1; y = 1;// 팩맨 처음 좌표
        m1 = 9; n1 = 13;
        m2 = 11; n2 = 13;
        m3 = 7; n3 = 13;//몬스터들의 처음 좌표

        packman[y][x] = 5;//팩맨 그림을 4등분하여 왼쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 5로 설정
        packman[y][x + 1] = 6;//팩맨 그림을 4등분하여  오른쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 6로 설정
        packman[y + 1][x] = 7;//팩맨 그림을 4등분하여 왼쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 7로 설정
        packman[y + 1][x + 1] = 8;//팩맨 그림을 4등분하여 오른쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 8로 설정

        packman[m1][n1] = 15;//빨간 몬스터 줄력하기 위해 15로 설정

        packman[m2][n2] = 16;//분홍 몬스터 줄력하기 위해 15로 설정
     
        packman[m3][n3] = 17;//민트 몬스터 줄력하기 위해 15로 설정
      
        s = 'R';//팩맨의 기본모습
        M1 = 2;
        M2 = 2;
        M3 = 2;//몬스터들의 기본 모습
           
        hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background));// 배경 비트맵 저장
        break;

    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_UPLOAD:                                // 사진 업로드 기능
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

                break;
            case IDM_DOWNLOAD:                             // 사진 라벨링 데이터 가져오는 버튼, 실험적으로 사용
                
                getImageLabels(imageName, labels); // 라벨링 데이터 가져와 labels에 저장

                IsItemInLabels(result, labels); // 라벨링 데이터에서 맵에 표현가능한 오브젝트 확인
                
                MessageBox(hWnd, labels,
                    _T("이미지 라벨링 데이터 확인"), MB_OKCANCEL);
                break;

        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;

    case WM_PAINT:
        
        switch (game_state) { // 60초가 다되면 false로 만들어 게임을 끝낸다 
        case 0:
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 61)));
            Rectangle(hdc, 0, 0, rectView.right, rectView.bottom);
            SetBkMode(hdc, TRANSPARENT); // 글자 배경을 투명하게 한다 
            SetTextColor(hdc, RGB(255, 255, 0));
            hFont = CreateFont(70, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, initial, initial_len, &initial_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, initial2, initial_len2, &initial_size2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
        case 1:
            hdc = BeginPaint(hWnd, &ps);
            mem1dc = CreateCompatibleDC(hdc);
            mem2dc = CreateCompatibleDC(mem1dc);

            if (hBit1 == NULL)
                hBit1 = CreateCompatibleBitmap(hdc, 1500, 960);
            oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
            oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);

            MakeMap(mem2dc);
            BitBlt(mem1dc, 0, 0, 1280, 960, mem2dc, 0, 0, SRCCOPY);//배경과 장애물 출력 
            Snack(mem1dc);//과자 그리기         
            RedAnimation(mem1dc);//빨간 몬스터 그리기
            PinkAnimation(mem1dc);//분홍 몬스터 그리기
            MintAnimation(mem1dc);//민트 몬스터 그리기
            Animation(mem1dc, s); //팩맨 출력 
            BitBlt(hdc, 0, 0, 1280, 960, mem1dc, 0, 0, SRCCOPY); //배경위에 그린것 출력 
            SelectObject(mem1dc, oldBit1);
            SelectObject(mem2dc, oldBit2);
            DeleteObject(mem2dc);
            DeleteObject(mem1dc);
            hFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, time_announcer, time_announcer_len, &time_announcer_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DrawText(hdc, time_announcer2, time_announcer_len2, &time_announcer_size2, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            break;
        case 2: // 게임 끝났을 때 화면 출력 
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, CreateSolidBrush(RGB(0, 0, 61)));
            Rectangle(hdc, 0, 0, rectView.right, rectView.bottom);           
            SetBkMode(hdc, TRANSPARENT); // 글자 배경을 투명하게 한다 
            SetTextColor(hdc, RGB(255, 255, 0));
            hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨 풋볼고딕 L"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, resultScore, resultScore_len, &resultScore_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            DeleteObject(hFont);
            EndPaint(hWnd, &ps);
            break;
        }
        break;
    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지
        
        if (game_state == 0) {
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
            case VK_RETURN://엔터
                s = 'B';  //처음으로돌아가기
                break;
            }
        }
        break;

    case WM_TIMER:
        if (game_state == 1) {
            switch (wParam) // 타이머의 id가 1일때와 2일때의 동작 
            {
            case 1: // 타이머가 1일때
                packman[y][x] = 3;
                packman[y][x + 1] = 3;
                packman[y + 1][x] = 3;
                packman[y + 1][x + 1] = 3;//팩맨이 중복되어 출력되지 않게하기위해와 과자 출력하지않고 스코어를 위한 변수로 0이아닌 3으로 설정
                              
                switch (s) {
                case 'L'://Left
                    x--;
                    if (packman[y][x] == 1 || packman[y + 1][x] == 1)//배열의 내용이 1(장애물)일때 위치 복구
                        x++;   
                    InvalidateRgn(hWnd, NULL, FALSE);
                    break;
                case 'R'://Right
                    x++;
                    if (packman[y][x + 1] == 1 || packman[y + 1][x + 1] == 1)
                        x--;
                    InvalidateRgn(hWnd, NULL, FALSE);
                    break;
                case 'U'://Up
                    y--;
                    if (packman[y][x] == 1 || packman[y][x + 1] == 1)
                        y++;
                    InvalidateRgn(hWnd, NULL, FALSE);
                    break;
                case 'D'://Down
                    y++;
                    if (packman[y + 1][x] == 1 || packman[y + 1][x + 1] == 1)
                        y--;                 
                    InvalidateRgn(hWnd, NULL, FALSE);
                    break;
                case 'B'://Back
                    x = 1; y = 1;
                    InvalidateRgn(hWnd, NULL, FALSE);
                    break;
                }
                              
                packman[y][x] = 5;
                packman[y][x + 1] = 6;
                packman[y + 1][x] = 7;
                packman[y + 1][x + 1] = 8;//옮겨진 좌표에 팩맨을 그리기 위해 변수 저장
            

               packman[m1][n1] = 0;//과자에 영향을 주지 않기 위해 0으로 설정
           
               switch (M1) {//1:왼, 2: 오른, 3:위, 4:아래
                case 1://Left
                    n1--;
                    if (packman[m1][n1] == 1 || packman[m1 + 1][n1] == 1) {   
                        M1 = 3;
                        n1++;//배열의 내용이 1(장애물)일때 위치 복구     
                        M1 = rand() % 4 + 1;//배열의 내용이 1(장애물)일때 랜덤으로 방향 설정 모든 방향 다 같은 방식으로 작동
                    }
                    if (packman[m1][n1] == 5 || packman[m1 + 1][n1] == 6 || packman[m1][n1] == 7 || packman[m1 + 1][n1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL,FLAG);
                    break;
                case 2://Right
                    n1++;
                    if (packman[m1][n1 + 1] == 1 || packman[m1 + 1][n1 + 1] == 1) {
                        M1 = 4;
                        n1--;
                        M1 = rand() % 4 + 1;
                    }
                    if (packman[m1][n1 + 1] == 5 || packman[m1 + 1][n1 + 1] == 6 || packman[m1][n1 + 1] == 7 || packman[m1+ 1][n1 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3:  //UP        
                    m1--;
                    if (packman[m1][n1] == 1 || packman[m1][n1 + 1] == 1) {         
                        M1 = 2;
                        m1++;
                        M1 = rand() % 4 + 1;
                    }
                    if (packman[m1][n1] == 5 || packman[m1][n1 + 6] == 12 || packman[m1][n1] == 7 || packman[m1][n1 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break; 
                case 4://Down
                    m1++;
                    if (packman[m1 + 1][n1] == 1 || packman[m1 + 1 ][n1 + 1] == 1) {
                        M1 = 1;
                        m1--;
                        M1 = rand() % 4 + 1;
                    }
                    if (packman[m1 + 1][n1] == 5 || packman[m1 + 1][n1 + 1] == 6 || packman[m1 + 1][n1] == 7 || packman[m1 + 1][n1+ 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                } 

                packman[m1][n1] = 15;//빨간 몬스터 위치 조정 
            
                
                packman[m2][n2] = 0;
              
                switch (M2) {
                case 1://Left
                    n2--;
                    if (packman[m2][n2] == 1 || packman[m2 + 1][n2] == 1) {//배열의 내용이 1(장애물)일때 위치 복구
                        M2 = 3;
                        n2++;
                        M2 = rand() % 4 + 1;
                    }
                    if (packman[m2][n2] == 5 || packman[m2 + 1][n2] == 6 || packman[m2][n2] == 7 || packman[m2 + 1][n2] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 2://Right
                    n2++;
                    if (packman[m2][n2 + 1] == 1 || packman[m2 + 1][n2 + 1] == 1) {
                        M2 = 4;
                        n2--;
                        M2 = rand() % 4 + 1;
                    }
                    if (packman[m2][n2 + 1] == 5 || packman[m2 + 1][n2 + 1] == 6 || packman[m2][n2 + 1] == 7 || packman[m2 + 1][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3://UP  
                    m2--;
                    if (packman[m2][n2] == 1 || packman[m2][n2 + 1] == 1) {
                        M2 = 2;
                        m2++;
                        M2 = rand() % 4 + 1;
                    }
                    if (packman[m2][n2] == 5 || packman[m2][n2 + 6] == 12 || packman[m2][n2] == 7 || packman[m2][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 4://Down
                    m2++;
                    if (packman[m2 + 1][n2] == 1 || packman[m2 + 1][n2 + 1] == 1) {
                        M2 = 1;
                        m2--;
                        M1 = rand() % 4 + 1;
                    }
                    if (packman[m2 + 1][n2] == 5 || packman[m2 + 1][n2 + 1] == 6 || packman[m2 + 1][n2] == 7 || packman[m2 + 1][n2 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }


                packman[m2][n2] = 16;//분홍 몬스터 위치조정
             

                packman[m3][n3] = 0;
             

                switch (M3) {
                case 1://Left
                    n3--;
                    if (packman[m3][n3] == 1 || packman[m3 + 1][n3] == 1) {//배열의 내용이 1(장애물)일때 위치 복구
                        M3 = 3;
                        n3++;
                        M1 = rand() % 4 + 1;
                    }
                    if (packman[m3][n3] == 5 || packman[m3 + 1][n3] == 6 || packman[m3][n3] == 7 || packman[m3 + 1][n3] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 2://Right
                    n3++;
                    if (packman[m3][n3 + 1] == 1 || packman[m3 + 1][n3 + 1] == 1) {
                        M3 = 4;
                        n3--;
                        M3 = rand() % 4 + 1;
                    }
                    if (packman[m3][n3 + 1] == 5 || packman[m3 + 1][n3 + 1] == 6 || packman[m3][n3 + 1] == 7 || packman[m3 + 1][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 3://UP  
                    m3--;
                    if (packman[m3][n3] == 1 || packman[m3][n3 + 1] == 1) {
                        M3 = 2;
                        m3++;
                        M3 = rand() % 4 + 1;
                    }
                    if (packman[m3][n3] == 5 || packman[m3][n3 + 6] == 12 || packman[m3][n3] == 7 || packman[m3][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                case 4://Down
                    m3++;
                    if (packman[m3 + 1][n3] == 1 || packman[m3 + 1][n3 + 1] == 1) {
                        M3 = 1;
                        m3--;
                        M3 = rand() % 4 + 1;
                    }
                    if (packman[m3 + 1][n3] == 5 || packman[m3 + 1][n3 + 1] == 6 || packman[m3 + 1][n3] == 7 || packman[m3 + 1][n3 + 1] == 8) {
                        game_state = 2;// 몬스터에게 닿을 시 게임 종료
                        FLAG = TRUE;
                    }
                    InvalidateRgn(hWnd, NULL, FLAG);
                    break;
                }


                packman[m3][n3] = 17;//민트 위치 조정
            
                
                if (count_time < 0 || score >= 93) { // 시간이 0보다 작아지거나 과자 다 먹을 시
                    game_state = 2; // false로 만들어 게임 종료 
                    FLAG = TRUE;
                }
                break;
            case 2: // 타이머가 2일때
                if (count_time >= 0)
                {
                    count_time = count_time--; // 1초씩 감소 
                }
                score = Counter(packman);

                resultScore_len = wsprintf(resultScore, TEXT("게임종료    SCORE:  %d"), score);
                time_announcer_len = wsprintf(time_announcer, TEXT("남은시간: %d SCORE: %d"), count_time, score);
                InvalidateRgn(hWnd, NULL, FLAG);         
                break;
            }
        }
        return 0;
      
    case WM_DESTROY:
        if (hBit1)
            DeleteObject(hBit1);
        KillTimer(hWnd, 1);
        KillTimer(hWnd, 2);

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