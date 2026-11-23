// Object2.cpp - Генератор матриці
// За командою Lab6 генерує матрицю n×n та записує її у Clipboard

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "clipboard_utils.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

// ============================================================================
// ГЛОБАЛЬНІ ЗМІННІ
// ============================================================================

const char g_szWindowClass[] = "OBJECT2_MAIN";
const char g_szAppTitle[] = "Object2 - Matrix Generator";

HWND g_hMainWnd = NULL;
HWND g_hWndManager = NULL;

int **g_matrix = NULL;
int g_nSize = 0;
int g_nMinValue = -100;
int g_nMaxValue = 100;

// ============================================================================
// ФУНКЦІЇ РОБОТИ З МАТРИЦЕЮ
// ============================================================================

void GenerateMatrix(int n, int minVal, int maxVal)
{
	if (g_matrix)
	{
		for (int i = 0; i < g_nSize; i++)
			delete[] g_matrix[i];
		delete[] g_matrix;
	}

	g_nSize = n;
	g_nMinValue = minVal;
	g_nMaxValue = maxVal;

	// Виділити пам'ять
	g_matrix = new int*[n];
	for (int i = 0; i < n; i++)
		g_matrix[i] = new int[n];

	// Генерувати випадкові числа
	srand((unsigned)time(NULL));
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			g_matrix[i][j] = minVal + rand() % (maxVal - minVal + 1);
		}
	}
}

void MatrixToClipboard(HWND hWnd)
{
	if (!g_matrix) return;

	// Сформувати текстовий формат матриці
	char buffer[65536];
	int offset = 0;

	// Першу лінію - розмір матриці
	offset += sprintf_s(buffer + offset, sizeof(buffer) - offset, "%d\n", g_nSize);

	// Дані матриці
	for (int i = 0; i < g_nSize; i++)
	{
		for (int j = 0; j < g_nSize; j++)
		{
			if (j > 0) offset += sprintf_s(buffer + offset, sizeof(buffer) - offset, " ");
			offset += sprintf_s(buffer + offset, sizeof(buffer) - offset, "%d", g_matrix[i][j]);
		}
		offset += sprintf_s(buffer + offset, sizeof(buffer) - offset, "\n");
	}

	// Записати у Clipboard
	PutTextToClipboard(hWnd, buffer);
}

void DisplayMatrix(HWND hWnd)
{
	if (!g_matrix) return;

	// Очистити вікно та перерисувати
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);
}

// ============================================================================
// ОБРОБНИК ПОВІДОМЛЕНЬ
// ============================================================================

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
		{
			g_hMainWnd = hWnd;
			return 0;
		}

		case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);

			// WM_COMMAND 10000 - повідомлення від менеджера з його HWND
			if (wmId == 10000)
			{
				g_hWndManager = (HWND)lParam;
				return 0;
			}

			// WM_COMMAND 10001 - розмір матриці
			if (wmId == 10001)
			{
				int size = (int)lParam;
				GenerateMatrix(size, g_nMinValue, g_nMaxValue);
				MatrixToClipboard(hWnd);
				DisplayMatrix(hWnd);

				// Повідомити менеджеру, що матриця готова
				if (g_hWndManager)
				{
					PostMessage(g_hWndManager, WM_COMMAND, 10000, (LPARAM)10000);
				}

				return 0;
			}

			break;
		}

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			char text[1024];
			sprintf_s(text, sizeof(text), "Object2 - Matrix Generator\nSize: %d x %d\n", g_nSize, g_nSize);

			TextOutA(hdc, 10, 10, text, (int)strlen(text));

			int y = 40;

			// Вивести матрицю
			if (g_matrix)
			{
				for (int i = 0; i < g_nSize && y < 400; i++)
				{
					char line[512] = "";
					for (int j = 0; j < g_nSize; j++)
					{
						char num[16];
						sprintf_s(num, sizeof(num), "%6d ", g_matrix[i][j]);
						strcat_s(line, sizeof(line), num);
					}
					TextOutA(hdc, 10, y, line, (int)strlen(line));
					y += 20;
				}
			}

			EndPaint(hWnd, &ps);
			return 0;
		}

		case WM_SIZE:
		{
			InvalidateRect(hWnd, NULL, TRUE);
			return 0;
		}

		case WM_CLOSE:
		{
			DestroyWindow(hWnd);
			return 0;
		}

		case WM_DESTROY:
		{
			// Звільнити пам'ять
			if (g_matrix)
			{
				for (int i = 0; i < g_nSize; i++)
					delete[] g_matrix[i];
				delete[] g_matrix;
				g_matrix = NULL;
			}

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
	// Парсити параметри
	if (lpCmdLine && strlen(lpCmdLine) > 0)
	{
		int minVal, maxVal;
		if (sscanf_s(lpCmdLine, "%d %d", &minVal, &maxVal) == 2)
		{
			g_nMinValue = minVal;
			g_nMaxValue = maxVal;
		}
	}

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
		600, 400,
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
