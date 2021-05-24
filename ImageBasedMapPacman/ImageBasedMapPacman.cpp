// ImageBasedMapPacman.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//
#include <windows.h>
#include <TCHAR.H>
#include <time.h>
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
      0, 0, 1280, 961, nullptr, nullptr, hInstance, nullptr);

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

 void Animation(int xPos, int yPos, RECT rectObject, int rectsize, HDC hdc, int s)
{
     HDC mem1dc, mem2dc;
    HBRUSH hBrush, oldBrush;
    HBITMAP RunBit[2], Mask[2], hBit, old1Bit, old2Bit ;
    static int count;
    int i;

    count++;
    count = count % 2;

    switch (s) {
    case 1://왼쪽으로 갈때 PackmanLeft비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanLeft1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanLeft2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskLeft));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 2://오른쪽으로 갈때 PackmanRightt비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanRight2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskRight));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 3://위로 갈때 PackmanUp비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanUp1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanUp2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskUp));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    case 4://아래로 갈때 PackmanDown비트맵 사용
        RunBit[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown1));
        RunBit[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_PackmanDown2));
        Mask[0] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskDown));
        Mask[1] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MaskCLose));
        break;
    }
    mem1dc = CreateCompatibleDC(hdc);
    mem2dc = CreateCompatibleDC(hdc);

    hBit = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_Background)); // 배경로드
    old2Bit = (HBITMAP)SelectObject(mem2dc, hBit);
    BitBlt(hdc, 0, 0, 1280, 961, mem2dc, 0, 0, SRCCOPY); //배경출력

    old1Bit = (HBITMAP)SelectObject(mem1dc, Mask[count]);
    BitBlt(mem1dc, xPos, yPos, 77, 77, mem1dc, 0, 0, SRCAND);//마스크

    SelectObject(mem2dc, RunBit[count]);
    BitBlt(mem1dc, xPos, yPos, 77, 77, mem2dc, 0, 0, SRCPAINT);//원본

    BitBlt(hdc, xPos, yPos, 77, 77, mem1dc, 0, 0, SRCAND);
    BitBlt(hdc, xPos, yPos, 77, 77, mem2dc, 0, 0, SRCPAINT);

    SelectObject(mem1dc, old1Bit);
    SelectObject(mem2dc, old2Bit);
    for (i = 0; i < 2; i++) {
        DeleteObject(Mask[i]);
        DeleteObject(RunBit[i]);
    }

    DeleteDC(mem1dc);
    DeleteDC(mem2dc);
    DeleteObject(hBit);

    hBrush = CreateSolidBrush(RGB(0, 255, 0));
    oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Rectangle(hdc, rectObject.left, rectObject.top, rectObject.right, rectObject.bottom);
    SelectObject(hdc, oldBrush);
    DeleteObject(hBrush);
}
#define rectsize 50
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool go1, go2, go3, go4 = true;
    HDC hdc;
    PAINTSTRUCT ps;
    static int x, y, objx, objy;
    static RECT rectView, rectObject; // 장애물용 RECT 구조체 추가 
    static int s = 1;//방향설정 변수
    static bool go = false; // 팩맨이 움직일지 말지 결정하는 불 변수 

    OPENFILENAME OFN;
    TCHAR str[100], lpstrFile[100] = _T("");
    TCHAR filter[] = _T("JPG(.jpg)\0*.jpg\0PNG(.png)\0*.png\0");

    switch (message)
    {
    case WM_CREATE:
        GetClientRect(hWnd, &rectView);
        x = 20; y = 20;
        srand((unsigned)time(NULL));    // 랜덤으로 장애물 생성하기 
        objx = rand() % rectView.right;
        objy = rand() % rectView.bottom;
        rectObject.left = objx;         // 장애물 좌표 구조체 
        rectObject.top = objy;
        rectObject.right = objx + rectsize;
        rectObject.bottom = objy - rectsize;

        SetTimer(hWnd, 1, 70, NULL);    // 팩맨의 움직임을 위한 타이머 설정
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
            Animation(x, y, rectObject, rectsize, hdc, s);
            EndPaint(hWnd, &ps);
            break;
    case WM_KEYDOWN: //키보드의 어떤 버튼이 내려간 것을 감지했을 때 발생되는 메시지
        // 키보드가 눌렸을때 wParam에 값이 저장된다 값과 비교하여 switch 문에 진입한다
        if (wParam == VK_LEFT)
        { // 왼쪽 화살표
            s = 1;//왼쪽상수
            go = true;  // 팩맨 이동 허용 
        }
                     
        else if (wParam == VK_RIGHT) // 오른쪽 화살표
        {
            s = 2;//오른쪽상수
            go = true;
        }    

        else if (wParam == VK_UP) // 위쪽 화살표
        {
            s = 3;//위쪽상수
            go = true;
        }

        else if (wParam == VK_DOWN)
        {
            s = 4;// 아래쪽상수
            go = true;
        }
           
       if(wParam == VK_RETURN)  // 엔터키 누르면 초기 위치로 이동 && 움직임 정지 
       {
            x = 20, y = 20;
            go = false; // 팩맨 이동 금지
            InvalidateRgn(hWnd, NULL, TRUE);
       }
    case WM_TIMER:
        if (go) { // go 가 true 일때만 팩맨이 움직인다 
            switch (s)
            {
            case(1):    // 왼쪽
                x -= 20; // 왼쪽으로 원 이동
                if (x - 20 < rectView.left) x += 20; // x - 20 보다 rect 구조체의 left 변수가 더 크면 x좌표에 40 추가하여 윈도우 밖으로 벗어나지 못하게 한다
                if (x  < rectObject.right && y > rectObject.top - 120 && y < rectObject.bottom + 30 && go1)
                    x += 20;
                {
                    if (x < rectObject.right) go1 = false;
                    else go1 = true;
                }

                break;
            case(2):    // 오른쪽 
                x += 20; // 오른쪽으로 원 이동
                if (x + 40 > rectView.right - 40) x -= 20; // x + 40 보다 rect 구조체의 right 변수가 더 작으면 x에 - 40 원은 윈도우 안의 Rectangle을 벗어나지 못한다!
                if (x + 50 > rectObject.left && y > rectObject.top - 120 && y < rectObject.bottom + 30 && go2)
                    x -= 20;
                {
                    if (x + 51 > rectObject.left) go2 = false;
                    else go2 = true;
                }
                break;
            case(3):    // 위
                y -= 20; // up, down은 수직 이동이므로 y값을 변경한다 이후 비슷
                if (y - 20 < rectView.top) y += 20;
                if (y - 24 < rectObject.bottom && x - 40 < rectObject.left && x + 120 > rectObject.right && go3)
                    y += 20;
                {
                    if (y - 25 < rectObject.bottom) go3 = false;    // 이게 없으면 장애물에 처박혀버린다
                    else go3 = true;
                }
                break;
            case(4):    // 아래 
                y += 20;
                if (y + 20 > rectView.bottom) y -= 20;
                if (y + 90 > rectObject.top && x - 40 < rectObject.left && x + 120 > rectObject.right && go4)
                    y -= 20;
                {
                    if (y + 90 > rectObject.top) go4 = false;
                    else go4 = true;
                }
                break;
            }
            InvalidateRgn(hWnd, NULL, TRUE); // 화면 다시그리기 함수 호출하여 WM_PAINT 메시지를 발생시키고 즉시 팩맨을 새로 그린다
        }
        
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

