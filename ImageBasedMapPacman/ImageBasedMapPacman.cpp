// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include "resource.h"
#include "framework.h"
#include "ImageBasedMapPacman.h"

#define MAX_LOADSTRING 100

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
      CW_USEDEFAULT, 0, CW_USEDEFAULT, CW_USEDEFAULT, nullptr, nullptr, hInstance, nullptr);

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

 void Animation(int xPos, int yPos, HDC hdc)
{
    HDC memdc;
    HBITMAP RunBit[2], hBit, oldBit, Mask[2];
    static int count;
    int i;
    count++;
    count = count % 2;
    RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Packman1)); 
    RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Packman2)); 
    Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Mask1));
    Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Mask2));
    memdc = CreateCompatibleDC(hdc);
    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background));
    oldBit = (HBITMAP)SelectObject(memdc, hBit);
    BitBlt(hdc, 0, 0, 1920, 1080, memdc, 0, 0, SRCAND);
    SelectObject(memdc, RunBit[count]);
    BitBlt(hdc, xPos, yPos, 77, 77, memdc, 0, 0, SRCPAINT);
    SelectObject(memdc, oldBit);
    for (i = 0; i < 2; i++)
        DeleteObject(RunBit[i]);
    DeleteDC(memdc);
    DeleteObject(hBit);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    static int x, y;
    static RECT rectView;

    OPENFILENAME OFN;
    TCHAR str[100], lpstrFile[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg)\0*.jpg\0PNG(.png)\0*.png\0");

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        x = 20; y = 20;
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
            Animation(x, y, hdc);
            EndPaint(hWnd, &ps);
            break;
    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지
        switch (wParam) { // 키보드가 눌렸을때 wParam에 값이 저장된다 값과 비교하여 switch 문에 진입한다
        case VK_LEFT: // 왼쪽 화살표
            x -= 40; // 왼쪽으로 원 이동
            if (x - 20 < rectView.left) x += 40; // x - 20 보다 rect 구조체의 left 변수가 더 크면 x좌표에 40 추가하여 윈도우 밖으로 벗어나지 못하게 한다
                break; 
        case VK_RIGHT: // 오른쪽 화살표
            x += 40; // 오른쪽으로 원 이동
            if (x + 50 > rectView.right) x -= 40; // x + 20 보다 rect 구조체의 right 변수가 더 작으면 x에 - 40 원은 윈도우 안의 Rectangle을 벗어나지 못한다!
                break;
        case VK_UP:
            y -= 40; // up, down은 수직 이동이므로 y값을 변경한다 이후 비슷
            if (y - 20 < rectView.top) y += 40;
            break;
        case VK_DOWN:
            y += 40;
            if (y + 20 > rectView.bottom) y -= 40;
            break;
        case VK_HOME:
            x = 20, y = 20;
            break;
        }
        InvalidateRgn(hWnd, NULL, TRUE); // 화면 다시그리기 함수 호출하여 WM_PAINT 메시지를 발생시키고 즉시 원을 새로 그린다
            break;
    case WM_DESTROY:
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

