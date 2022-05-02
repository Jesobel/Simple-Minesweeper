#include "stdafx.h"
#include "Resource.h"

#define MAX_LOADSTRING 100

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

CMineSweeper* game;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);


	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MINESWEEPER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	game = CMineSweeper::getInst();
	game->newGame(16, 30, 99);

	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	MSG msg;
	

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINESWEEPER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance;
	HWND hWnd = CreateWindowW(szWindowClass, szTitle,
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	RECT rt = { 0, 0, game->getWidth() * 20 + 20, game->getHeight() * 20 + 70 };
	AdjustWindowRect(&rt, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, true);
	SetWindowPos(hWnd, NULL, 100, 100, rt.right - rt.left, rt.bottom - rt.top, 0);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

// 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static TCHAR statusMsg[20] = _T("RESTART");
	static RECT statusBox = { 10, 10, game->getWidth() * 20 + 10, 60 };
	const static TCHAR NUMBS[][2] = { _T(" "), _T("1"), _T("2"), _T("3"), _T("4"), _T("5"), _T("6"), _T("7"), _T("8") };

	switch (message)
	{
	case WM_LBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		// 초기화 버튼 영역
		if (y >= 10 && y < 60) {
			game->init();
			_tcscpy_s(statusMsg, 20, _T("RESTART"));
		}
		// 그리드 영역
		else if (y >= 70 && y < game->getHeight() * 20 + 70 && x >= 10 && x < game->getWidth() * 20 + 10)
			game->click((y - 70) / 20, (x - 10) / 20);

		if (game->GameResult() == 1)
			_tcscpy_s(statusMsg, 20, _T("GAME OVER"));
		else if (game->GameResult() == 2)
			_tcscpy_s(statusMsg, 20, _T("CLEAR!!"));

		InvalidateRect(hWnd, NULL, false);
		break;
	}

	case WM_RBUTTONUP:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		// 그리드 영역
		if (y >= 70 && y < game->getHeight() * 20 + 70 && x >= 10 && x < game->getWidth() * 20 + 10)
			game->flag((y - 70) / 20, (x - 10) / 20);

		InvalidateRect(hWnd, NULL, false);
		break;
	}
	case WM_PAINT:
	{
		// double buffering
		PAINTSTRUCT ps;
		static HDC hdc, MemDC;
		static HBITMAP BackBit, oldBackBit;
		static RECT bufferRT;
		MemDC = BeginPaint(hWnd, &ps);

		GetClientRect(hWnd, &bufferRT);
		hdc = CreateCompatibleDC(MemDC);
		BackBit = CreateCompatibleBitmap(MemDC, bufferRT.right, bufferRT.bottom);
		oldBackBit = (HBITMAP)SelectObject(hdc, BackBit);
		PatBlt(hdc, 0, 0, bufferRT.right, bufferRT.bottom, WHITENESS);

		// 상단 초기화 버튼 그리기
		Rectangle(hdc, 10, 10, game->getWidth() * 20 + 10, 60);
		DrawText(hdc, statusMsg, -1, &statusBox, DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		// 그리드 그리기
		for (int i = 0; i < game->getHeight(); ++i) {
			for (int j = 0; j < game->getWidth(); ++j) {
				if (game->at(i, j).isCovered()) {
					HBRUSH gray_brush = CreateSolidBrush(RGB(200, 200, 200));
					HGDIOBJ old_brush = SelectObject(hdc, gray_brush);
					Rectangle(hdc, j * 20 + 10, i * 20 + 70, (j + 1) * 20 + 10, (i + 1) * 20 + 70);

					if (game->at(i, j).isFlagged())
						TextOut(hdc, j * 20 + 15, i * 20 + 72, _T("#"), 1);

					SelectObject(hdc, old_brush);
					DeleteObject(gray_brush);
				}
				else {
					Rectangle(hdc, j * 20 + 10, i * 20 + 70, (j + 1) * 20 + 10, (i + 1) * 20 + 70);
					if (game->at(i, j).isMine())
						TextOut(hdc, j * 20 + 11, i * 20 + 72, _T("◆"), 1);
					else
						TextOut(hdc, j * 20 + 15, i * 20 + 72, NUMBS[game->at(i, j).getNumber()], 1);
				}
			}
		}

		GetClientRect(hWnd, &bufferRT);
		BitBlt(MemDC, 0, 0, bufferRT.right, bufferRT.bottom, hdc, 0, 0, SRCCOPY);
		SelectObject(hdc, oldBackBit);
		DeleteObject(BackBit);
		DeleteDC(hdc);
		EndPaint(hWnd, &ps);
		break;
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}