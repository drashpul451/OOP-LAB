#pragma once
#include <windows.h>
#include <string>

class MyTable {
private:
	HWND hWndDlg;
	HWND hListBox; // Оголошення поля для ListBox
	static INT_PTR CALLBACK DlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	MyTable();
	~MyTable();

	void Show(HWND hWndParent);
	void Hide();
	void AddRow(const std::wstring& name, long x1, long y1, long x2, long y2);
	void Clear();
	bool IsOpen() const { return hWndDlg != NULL; }
};