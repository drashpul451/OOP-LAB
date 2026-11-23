// Object3.cpp - Обчислювач детермінанту матриці
// Читає матрицю з Clipboard та обчислює детермінант

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "clipboard_utils.h"

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "kernel32.lib")

// ============================================================================
// ГЛОБАЛЬНІ ЗМІННІ
// ============================================================================

const char g_szWindowClass[] = "OBJECT3_MAIN";
const char g_szAppTitle[] = "Object3 - Determinant Calculator";

HWND g_hMainWnd = NULL;
HWND g_hWndManager = NULL;

double **g_matrix = NULL;
int g_nSize = 0;
double g_dDeterminant = 0.0;

// ============================================================================
// ФУНКЦІЇ РОБОТИ З МАТРИЦЕЮ ТА ДЕТЕРМІНАНТОМ
// ============================================================================

// Глибока копія матриці для обчислення
double** CopyMatrix(double **src, int n)
{
	double **dest = new double*[n];
	for (int i = 0; i < n; i++)
	{
		dest[i] = new double[n];
		for (int j = 0; j < n; j++)
			dest[i][j] = src[i][j];
	}
	return dest;
}

void FreeMatrix(double **mat, int n)
{
	if (mat)
	{
		for (int i = 0; i < n; i++)
			delete[] mat[i];
		delete[] mat;
	}
}

// Обчислення детермінанту за методом Гаусса
double ComputeDeterminant(double **mat, int n)
{
	double **A = CopyMatrix(mat, n);
	double det = 1.0;
	int sign = 1;

	for (int i = 0; i < n; i++)
	{
		// Пошук ведучого елементу
		int pivot = i;
		for (int j = i + 1; j < n; j++)
		{
			if (fabs(A[j][i]) > fabs(A[pivot][i]))
				pivot = j;
		}

		if (fabs(A[pivot][i]) < 1e-10)
		{
			det = 0.0;
			FreeMatrix(A, n);
			return det;
		}

		// Обмін рядків
		if (pivot != i)
		{
			sign *= -1;
			double *tmp = A[i];
			A[i] = A[pivot];
			A[pivot] = tmp;
		}

		det *= A[i][i];

		// Гауссівське виключення
		for (int j = i + 1; j < n; j++)
		{
			double factor = A[j][i] / A[i][i];
			for (int k = i; k < n; k++)
				A[j][k] -= factor * A[i][k];
		}
	}

	FreeMatrix(A, n);
	return det * sign;
}

bool ReadMatrixFromClipboard(HWND hWnd)
{
	char buffer[65536];

	if (!GetTextFromClipboard(hWnd, buffer, sizeof(buffer)))
		return false;

	// Парсити буфер
	int offset = 0;
	if (sscanf_s(buffer, "%d", &g_nSize) != 1)
		return false;

	if (g_nSize < 1 || g_nSize > 100)
		return false;

	// Алокувати матрицю
	if (g_matrix)
		FreeMatrix(g_matrix, g_nSize);

	g_matrix = new double*[g_nSize];
	for (int i = 0; i < g_nSize; i++)
		g_matrix[i] = new double[g_nSize];

	// Знайти початок даних (після першої нової лінії)
	char *p = strchr(buffer, '\n');
	if (!p) return false;
	p++;

	// Прочитати матрицю
	for (int i = 0; i < g_nSize; i++)
	{
		for (int j = 0; j < g_nSize; j++)
		{
			if (sscanf_s(p, "%lf", &g_matrix[i][j]) != 1)
				return false;

			// Пропустити число і простір
			while (*p && *p != ' ' && *p != '\n')
				p++;
			while (*p && (*p == ' '))
				p++;
		}
		// Перейти на наступний рядок
		while (*p && *p != '\n')
			p++;
		if (*p == '\n') p++;
	}

	// Обчислити детермінант
	g_dDeterminant = ComputeDeterminant(g_matrix, g_nSize);

	return true;
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

				// Спробувати прочитати матрицю з Clipboard
				Sleep(500); // Дати часу на запис у Clipboard

				if (ReadMatrixFromClipboard(hWnd))
				{
					InvalidateRect(hWnd, NULL, TRUE);
					UpdateWindow(hWnd);

					// Повідомити менеджеру
					if (g_hWndManager)
					{
						PostMessage(g_hWndManager, WM_COMMAND, 10001, (LPARAM)10000);
					}
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

			if (g_matrix)
			{
				sprintf_s(text, sizeof(text), 
						 "Object3 - Determinant Calculator\nMatrix Size: %d x %d\n\nDeterminant = %f\n\nMatrix:\n",
						 g_nSize, g_nSize, g_dDeterminant);

				TextOutA(hdc, 10, 10, text, (int)strlen(text));

				int y = 120;

				// Вивести матрицю
				for (int i = 0; i < g_nSize && y < 400; i++)
				{
					char line[512] = "";
					for (int j = 0; j < g_nSize; j++)
					{
						char num[20];
						sprintf_s(num, sizeof(num), "%8.2f ", g_matrix[i][j]);
						strcat_s(line, sizeof(line), num);
					}
					TextOutA(hdc, 10, y, line, (int)strlen(line));
					y += 20;
				}
			}
			else
			{
				strcpy_s(text, sizeof(text), "Object3 - Determinant Calculator\n\nWaiting for matrix from Object2...");
				TextOutA(hdc, 10, 10, text, (int)strlen(text));
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
				FreeMatrix(g_matrix, g_nSize);

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
		100, 100,
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
