// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include "framework.h"
#include "ImageBasedMapPacman.h"

#define MAX_LOADSTRING 100

//함수들 선언
HDC MakeMap(HDC hdc);
HDC Animation(HDC mem1dc, int s);
HDC Snack(HDC hdc);

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
WCHAR szTitle[] = _T("ImageBasedMapPackman");                  // 제목 표시줄 텍스트입니다.
WCHAR szWindowClass[MAX_LOADSTRING];            // 기본 창 클래스 이름입니다.

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
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1},
    {1,0,0,0,0,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,1,1,0,0,0,0,1,1,1,1,1,1,0,0,1,1,1,1,1,1,0,0,1,1,1,1,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
};//1일때 장애물, 0일때 빈공간
int mapE1 = 48;//장애물의 세로(960/10)
int mapE2 = 40;//장애물의 가로(1280/16)
int snackSize = 10;//먹는 오브젝트 크기
static int score = 0;//출력 스코어 저장변수

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static HDC hdc, mem1dc, mem2dc;
    PAINTSTRUCT ps;
    static HBITMAP hBit1, hBit2, oldBit1, oldBit2;
    static int x, y;
    static RECT rectView;
    static char s;//방향설정 변수


    static int count_time = 60; // 1분간 게임 가능하게하는 변수
    static TCHAR time_announcer[1024], resultScore[1024]; // drawtext할 문자열
    static int time_announcer_len, resultScore_len; // drawtext에서 문자열 길이를 넘겨줄 변수 
    RECT time_announcer_size;   // drawtext 크기 (남은시간, 현재 점수 출력) 
    time_announcer_size.left = 1280;
    time_announcer_size.top = 10;
    time_announcer_size.right = 1500;
    time_announcer_size.bottom = 50;

    RECT resultScore_size; // 게임 종료시 출력되는 문구의 크기 
    resultScore_size.left = 500;
    resultScore_size.top = 380;
    resultScore_size.right = 900;
    resultScore_size.bottom = 480;

    static bool game_state = true; // 1분이 지나면 false로 바뀌고 게임 종료 

    OPENFILENAME OFN;
    TCHAR str[100], lpstrFile[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg)\0*.jpg\0PNG(.png)\0*.png\0");

    switch (message)
    {
    case WM_CREATE:
        SetTimer(hWnd, 2, 1000, NULL); // 제한시간용 타이머
        GetClientRect(hWnd, &rectView);
        AddFontResourceA("C:\Windows\Fonts\NEXONFootballGothicB.ttf"); // 폰트 불러오기 
     
        x = 1; y = 1;
        packman[y][x] = 5;//팩맨 그림을 4등분하여 왼쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 5로 설정
        packman[y][x + 1] = 6;//팩맨 그림을 4등분하여  오른쪽위를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 6로 설정
        packman[y + 1][x] = 7;//팩맨 그림을 4등분하여 왼쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 7로 설정
        packman[y + 1][x + 1] = 8;//팩맨 그림을 4등분하여 오른쪽아래를 출력하기 위해 4개의 좌표 중 왼쪽위 좌표를 8로 설정
        s = 'R';//팩맨의 기본위치 
        hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BITMAP1));//배경 비트맵 저장
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
            OFN.lpstrFile = lpstrFile;
            OFN.nMaxFile = 100;
            OFN.lpstrInitialDir = _T(".");
            if (GetOpenFileName(&OFN) != 0) {
                _stprintf_s(str, _T("%s 파일을 업로드하겠습니가?"), OFN.lpstrFile);
                MessageBox(hWnd, str, _T("업로드 확인"), MB_OK);
            }

            break;
        case IDM_DOWNLOAD:                             // 사진 가져오는 기능 
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
        case true:
            hdc = BeginPaint(hWnd, &ps);
            mem1dc = CreateCompatibleDC(hdc);
            mem2dc = CreateCompatibleDC(mem1dc);
            if (hBit1 == NULL)
                hBit1 = CreateCompatibleBitmap(hdc, 1500, 960);
            oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
            oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
            BitBlt(mem1dc, 0, 0, 1280, 960, MakeMap(mem2dc), 0, 0, SRCCOPY);//배경과 장애물 출력
            Snack(mem1dc);//과자 그리기 
            BitBlt(hdc, 0, 0, 1280, 960, Animation(mem1dc, s), 0, 0, SRCCOPY); //배경위에 그린것과 팩맨 출력 
            SelectObject(mem1dc, oldBit1);
            SelectObject(mem2dc, oldBit2);
            DeleteObject(mem2dc);
            DeleteObject(mem1dc);
            DrawText(hdc, time_announcer, time_announcer_len, &time_announcer_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hWnd, &ps);
            break;
        case false: // 게임 끝났을 때 화면 출력 
            hdc = BeginPaint(hWnd, &ps);
            SelectObject(hdc, CreateSolidBrush(RGB(255, 255, 0)));
            Rectangle(hdc, 0, 0, rectView.right, rectView.bottom);
           
            SetBkMode(hdc, TRANSPARENT); // 글자 배경을 투명하게 한다 
            HFONT hFont, oldFont;
            SetTextColor(hdc, RGB(255, 255, 255));
            hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("넥슨"));
            oldFont = (HFONT)SelectObject(hdc, hFont);
            DrawText(hdc, resultScore, resultScore_len, &resultScore_size, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            EndPaint(hWnd, &ps);
            break;
        }
        break;
    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지
        SetTimer(hWnd, 1, 100, NULL);
        if (game_state == true) {
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
        if (game_state == true) {
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
                    InvalidateRgn(hWnd, NULL, TRUE);
                    break;
                case 'R'://Right
                    x++;
                    if (packman[y][x + 1] == 1 || packman[y + 1][x + 1] == 1)
                        x--;
                    InvalidateRgn(hWnd, NULL, TRUE);
                    break;
                case 'U'://Up
                    y--;
                    if (packman[y][x] == 1 || packman[y][x + 1] == 1)
                        y++;
                    InvalidateRgn(hWnd, NULL, TRUE);
                    break;
                case 'D'://Down
                    y++;
                    if (packman[y + 1][x] == 1 || packman[y + 1][x + 1] == 1)
                        y--;
                    InvalidateRgn(hWnd, NULL, TRUE);
                    break;
                case 'B'://Back
                    x = 1; y = 1;
                    InvalidateRgn(hWnd, NULL, TRUE);
                    break;
                }

                packman[y][x] = 5;
                packman[y][x + 1] = 6;
                packman[y + 1][x] = 7;
                packman[y + 1][x + 1] = 8;//옮겨진 좌표에 팩맨을 그리기 위해 변수 저장


                if (count_time < 0) // 시간이 0보다 작아지면 
                    game_state = false; // false로 만들어 게임 종료 

                break;
            case 2: // 타이머가 2일때
                if (count_time >= 0)
                {

                    count_time = count_time--; // 1초씩 감소 
                }
                score = Counter(packman);

                resultScore_len = wsprintf(resultScore, TEXT("넥슨 풋볼 고딕"), score);
                time_announcer_len = wsprintf(time_announcer, TEXT("남은시간: %d SCORE: %d"), count_time, score);
                InvalidateRgn(hWnd, NULL, TRUE);                break;
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