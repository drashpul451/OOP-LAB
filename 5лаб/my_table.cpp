#include "my_table.h"
#include <cstdio>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

#define IDC_LISTBOX_TABLE 1001

MyTable::MyTable() : hWndDlg(NULL), hListBox(NULL) {}

MyTable::~MyTable() {
	Hide();
}

INT_PTR CALLBACK MyTable::DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_SYSCOMMAND:
		if ((wParam & 0xFFF0) == SC_CLOSE) {
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL || LOWORD(wParam) == IDOK) {
			DestroyWindow(hDlg);
			return TRUE;
		}
		break;
	case WM_DESTROY:
		return TRUE;
	}
	return FALSE;
}

void MyTable::Show(HWND hWndParent) {
	if (hWndDlg) {
		SetFocus(hWndDlg);
		return;
	}

	hWndDlg = CreateWindowExW(
		WS_EX_TOOLWINDOW,
		L"#32770",
		L"Objects Table", // Заголовок англійською (без проблем із кодуванням ANSI/UTF)
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		100, 100, 420, 320,
		hWndParent,
		NULL,
		GetModuleHandle(NULL),
		NULL
	);

	if (hWndDlg) {
		hListBox = CreateWindowExW(
			0, L"LISTBOX", NULL,
			WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_NOTIFY | LBS_HASSTRINGS,
			10, 10, 384, 250,
			hWndDlg,
			(HMENU)IDC_LISTBOX_TABLE,
			GetModuleHandle(NULL),
			NULL
		);

		if (hListBox) {
			// Латинські назви колонок гарантовано відобразяться ідеально чітко
			SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)L"TYPE        X1    Y1    X2    Y2");
			SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)L"-------------------------------------");
		}
	}
}

void MyTable::Hide() {
	if (hWndDlg) {
		DestroyWindow(hWndDlg);
		hWndDlg = NULL;
		hListBox = NULL;
	}
}

void MyTable::AddRow(const std::wstring& name, long x1, long y1, long x2, long y2) {
	if (!hWndDlg || !hListBox) return;
	wchar_t buf[256];
	swprintf_s(buf, L"%-10ls  %-4ld  %-4ld  %-4ld  %-4ld", name.c_str(), x1, y1, x2, y2);
	SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)buf);
}

void MyTable::Clear() {
	if (!hWndDlg || !hListBox) return;
	SendMessageW(hListBox, LB_RESETCONTENT, 0, 0);
	SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)L"TYPE        X1    Y1    X2    Y2");
	SendMessageW(hListBox, LB_ADDSTRING, 0, (LPARAM)L"-------------------------------------");
}