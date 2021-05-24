// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include <time.h>
#include "resource.h"
#include "framework.h"
#include "ImageBasedMapPacman.h"

#define MAX_LOADSTRING 100

HDC MakeMap(HDC hdc);
HDC Animation(HDC mem1dc, int xPos, int yPos, int s);

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
    if (!InitInstance (hInstance, nCmdShow))
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

    return (int) msg.wParam;
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

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IMAGEBASEDMAPPACMAN));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_IMAGEBASEDMAPPACMAN);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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
     0, 0, 1280, 960, nullptr, nullptr, hInstance, nullptr);

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


int packman[10][16] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,1,1,1,1,0,0,1,1,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
    {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
    {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
    {1,0,1,1,0,0,1,1,1,1,0,0,1,1,0,1},
    {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
int mapE1 = 96;//장애물의 세로(960/10)
int mapE2 = 80;//장애물의 가로(1280/16)

HDC MakeMap(HDC hdc) //맵 장애물 표시
{
    HDC memdc;
    HBITMAP hBit;
    int i, j;
    
    memdc = CreateCompatibleDC(hdc);
    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Map));
    SelectObject(memdc, hBit);
   /* int map[10][16] =
    {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
        {1,0,1,1,0,0,1,1,1,1,0,0,1,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
        {1,0,0,0,1,1,0,0,0,0,1,1,0,0,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1},
        {1,0,1,1,0,0,1,1,1,1,0,0,1,1,0,1},
        {1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };*/
    for (i = 0; i < 10; i++)
        for (j = 0; j < 16; j++)
            if (packman[i][j] == 1)
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCCOPY);
    DeleteObject(hBit);
    return hdc;
}

HDC Animation(HDC hdc, int xPos, int yPos, int s)
    {
    HDC memdc;
    HBITMAP RunBit[2], Mask[2];
    static int count;
    int i,j;
   
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

    for (i = 0; i < 10; i++)
        for (j = 0; j < 16; j++)
            if (packman[i][j] == 2) {
                SelectObject(memdc, Mask[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCAND);//배경위에 마스크
                SelectObject(memdc, RunBit[count]);
                BitBlt(hdc, j * mapE2, i * mapE1, mapE2, mapE1, memdc, 0, 0, SRCPAINT);//배경위에 원본
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
    

    int i, j;
    

    OPENFILENAME OFN;
    TCHAR str[100], lpstrFile[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg)\0*.jpg\0PNG(.png)\0*.png\0");

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        x = 1; y = 1;
        packman[y][x] = 2;
        s = 'R';
        hBit2 = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background));

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
        hdc = BeginPaint(hWnd, &ps);    
        mem1dc = CreateCompatibleDC(hdc);
        mem2dc = CreateCompatibleDC(mem1dc);
         if (hBit1 == NULL)
            hBit1 = CreateCompatibleBitmap(hdc, 1280,960);
        oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
        oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit2);
        BitBlt(mem1dc, 0, 0, 1280, 960, MakeMap(mem2dc), 0, 0, SRCCOPY);//배경출력
        Animation(mem1dc, x, y, s);
        BitBlt(hdc, 0, 0, 1280, 960, mem1dc, 0, 0, SRCCOPY); //배경위에 그린거 출력
        SelectObject(mem1dc, oldBit1);
        SelectObject(mem2dc, oldBit2);
        DeleteObject(mem2dc);
        DeleteObject(mem1dc);
        EndPaint(hWnd, &ps);
        break;

    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지
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
        case VK_RETURN:
            s = 'B';  //처음으로돌아가기
            break;
        }
        break;

    case WM_TIMER:
         packman[y][x] = 0;
        switch (s) {
        case 'L'://Left
            x--;
            if (x <= 0)
                x++;
            if (packman[y][x] == 1)
                x++;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case 'R'://Right
            x++;
            if (x >= 15)
                x--;
            if (packman[y][x] == 1)
                x--;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case 'U'://Up
            y--;
            if (y <= 0)
                y++;
            if (packman[y][x] == 1)
                y++;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case 'D'://Down
            y++;
            if (y >= 9)
                y--;
            if (packman[y][x] == 1)
                y--;
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        case 'B'://Back
          
            InvalidateRgn(hWnd, NULL, TRUE);
            break;
        }
      packman[y][x] = 2;
                    return 0;
                    
    case WM_DESTROY:
        if (hBit1)
            DeleteObject(hBit1);
        KillTimer(hWnd, 1);
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

