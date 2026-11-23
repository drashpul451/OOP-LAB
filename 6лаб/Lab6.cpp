// Lab6.cpp - Головна програма (менеджер)
// Варіант 1: Керування матрицею
// Користувач вводить n, Min, Max
// Lab6 викликає Object2 (генерування матриці), потім Object3 (обчислення детермінанту)

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <commctrl.h>
#include "clipboard_utils.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

// ============================================================================
// ГЛОБАЛЬНІ ЗМІННІ
// ============================================================================

const char g_szWindowClass[] = "LAB6_MAIN";
const char g_szAppTitle[] = "Lab6 - Matrix Manager (Variant 1)";

HWND g_hWndObject2 = NULL;
HWND g_hWndObject3 = NULL;
HWND g_hMainWnd = NULL;

int g_nMatrixSize = 0;
int g_nMinValue = 0;
int g_nMaxValue = 0;

int g_iState = 0; // 0=ready, 1=waiting_obj2, 2=obj2_done, 3=waiting_obj3, 4=completed

// ============================================================================
// ФУНКЦІЇ РОБОТИ З ІНШИМИ ПРОГРАМАМИ
// ============================================================================

void FindAndStartObject2()
{
	g_hWndObject2 = FindWindowByClass("OBJECT2_MAIN");

	if (g_hWndObject2 == NULL)
	{
		// Запустити Object2
		char cmdLine[256];
		sprintf_s(cmdLine, sizeof(cmdLine), "Object2.exe %d %d", g_nMinValue, g_nMaxValue);
		StartProgram("Object2.exe", cmdLine);

		// Дати часу на запуск
		Sleep(1000);
		g_hWndObject2 = FindWindowByClass("OBJECT2_MAIN");
	}

	if (g_hWndObject2)
	{
		// Надіслати WM_COMMAND для сигналу до Object2
		PostMessage(g_hWndObject2, WM_COMMAND, 10000, (LPARAM)g_hMainWnd);

		// Надіслати параметр-розмір матриці
		PostMessage(g_hWndObject2, WM_COMMAND, 10001, (LPARAM)g_nMatrixSize);

		g_iState = 1; // Чекаємо на Object2
	}
}

void FindAndStartObject3()
{
	g_hWndObject3 = FindWindowByClass("OBJECT3_MAIN");

	if (g_hWndObject3 == NULL)
	{
		StartProgram("Object3.exe", NULL);
		Sleep(1000);
		g_hWndObject3 = FindWindowByClass("OBJECT3_MAIN");
	}

	if (g_hWndObject3)
	{
		// Надіслати сигнал до Object3
		PostMessage(g_hWndObject3, WM_COMMAND, 10000, (LPARAM)g_hMainWnd);
		g_iState = 3; // Чекаємо на Object3
	}
}

// ============================================================================
// ДІАЛОГОВЕ ВІКНО
// ============================================================================

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hEditSize, hEditMin, hEditMax;

	switch (message)
	{
		case WM_INITDIALOG:
		{
			// Приховати при init
			HWND h;

			// Поле для розміру матриці
			h = GetDlgItem(hDlg, 1001);
			if (h) {
				SetWindowTextA(h, "5");
			}

			// Поле для мін значення
			h = GetDlgItem(hDlg, 1002);
			if (h) {
				SetWindowTextA(h, "-100");
			}

			// Поле для макс значення
			h = GetDlgItem(hDlg, 1003);
			if (h) {
				SetWindowTextA(h, "100");
			}

			return (INT_PTR)TRUE;
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);

			if (wmId == IDOK || wmId == 1004) // Кнопка "Виконати" або OK
			{
				char buf[32];
				HWND h;

				// Читання параметрів
				h = GetDlgItem(hDlg, 1001);
				if (h) {
					GetWindowTextA(h, buf, sizeof(buf));
					g_nMatrixSize = atoi(buf);
				}

				h = GetDlgItem(hDlg, 1002);
				if (h) {
					GetWindowTextA(h, buf, sizeof(buf));
					g_nMinValue = atoi(buf);
				}

				h = GetDlgItem(hDlg, 1003);
				if (h) {
					GetWindowTextA(h, buf, sizeof(buf));
					g_nMaxValue = atoi(buf);
				}

				// Перевірка значимості
				if (g_nMatrixSize < 1 || g_nMatrixSize > 20)
				{
					MessageBoxA(hDlg, "Розмір матриці повинен бути від 1 до 20!", 
							   "Помилка", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}

				if (g_nMinValue >= g_nMaxValue)
				{
					MessageBoxA(hDlg, "Min повинен бути менше за Max!", 
							   "Помилка", MB_OK | MB_ICONERROR);
					return (INT_PTR)FALSE;
				}

				EndDialog(hDlg, IDOK);

				// Запустити Object2
				FindAndStartObject2();

				return (INT_PTR)TRUE;
			}

			if (wmId == IDCANCEL)
			{
				EndDialog(hDlg, IDCANCEL);
				return (INT_PTR)TRUE;
			}

			break;
		}
	}

	return (INT_PTR)FALSE;
}

// ============================================================================
// ОБРОБНИК ПОВІДОМЛЕНЬ ОСНОВНОГО ВІКНА
// ============================================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			g_hMainWnd = hWnd;

			// Показати меню з вибором операції
			HMENU hMenu = CreateMenu();
			HMENU hSubMenu = CreatePopupMenu();

			AppendMenuA(hSubMenu, MF_STRING, 1001, "Process Matrix (Variant 1)");
			AppendMenuA(hSubMenu, MF_SEPARATOR, 0, NULL);
			AppendMenuA(hSubMenu, MF_STRING, 1002, "Exit");

			AppendMenuA(hMenu, MF_POPUP, (UINT_PTR)hSubMenu, "File");

			SetMenu(hWnd, hMenu);

			return 0;
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);

			if (wmId == 1001) // Process Matrix
			{
				// Показати діалог введення параметрів
				DialogBoxA((HINSTANCE)GetModuleHandle(NULL), 
						  MAKEINTRESOURCEA(101), hWnd, DialogProc);
				return 0;
			}

			if (wmId == 1002) // Exit
			{
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				return 0;
			}

			// Повідомлення від Object2 - матриця готова
			if (wmId == 10000 && wParam == 10000)
			{
				MessageBoxA(hWnd, "Object2 повідомив: матриця готова!", 
						   "Message from Object2", MB_OK);

				// Запустити Object3
				FindAndStartObject3();
				return 0;
			}

			// Повідомлення від Object3 - результат готов
			if (wmId == 10001 && wParam == 10000)
			{
				MessageBoxA(hWnd, "Object3 повідомив: детермінант обчислено!", 
						   "Message from Object3", MB_OK);
				g_iState = 4; // Завершено
				return 0;
			}

			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			char text[256];
			sprintf_s(text, sizeof(text), 
					 "Lab6 - Matrix Manager\nState: %d\nMatrix Size: %d\nMin: %d, Max: %d",
					 g_iState, g_nMatrixSize, g_nMinValue, g_nMaxValue);

			TextOutA(hdc, 10, 10, text, (int)strlen(text));

			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_CLOSE:
		{
			// Закрити Object2 та Object3
			if (g_hWndObject2) PostMessage(g_hWndObject2, WM_CLOSE, 0, 0);
			if (g_hWndObject3) PostMessage(g_hWndObject3, WM_CLOSE, 0, 0);

			DestroyWindow(hWnd);
			return 0;
		}

		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

// ============================================================================
// ТОЧКА ВХОДУ
// ============================================================================

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
				   LPSTR lpCmdLine, int nCmdShow)
{
	// Реєстрація класу вікна
	WNDCLASSA wcex;
	ZeroMemory(&wcex, sizeof(wcex));

	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = g_szWindowClass;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassA(&wcex);

	// Створення вікна
	HWND hWnd = CreateWindowA(
		g_szWindowClass, g_szAppTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 300,
		NULL, NULL, hInstance, NULL
	);

	if (!hWnd) return 1;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Цикл повідомлень
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}
