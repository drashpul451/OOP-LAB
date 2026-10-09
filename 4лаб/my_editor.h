#pragma once
#include <windows.h>
#include <string>
#include "native_shape.h"

class MyEditor {
public:
	MyEditor();
	~MyEditor();

	// Start editing given prototype (takes ownership of prototype)
	void Start(Shape* prototype);

	void OnLBdown(HWND hwnd, int x, int y);
	void OnMouseMove(HWND hwnd, int x, int y);
	void OnLBup(HWND hwnd, int x, int y);
	void OnPaint(HDC hdc);

	void ClearAll();
	void DeleteLast();

	bool SaveToFile(const std::wstring& path);
	bool LoadFromFile(const std::wstring& path);

private:
	Shape** pshape;
	int N;
	Shape* prototype; // owned prototype used by Start(Shape*)
	Shape* rubber; // current rubber object (owned)
	bool isDrawing;
	long x1, y1, x2, y2;
};

// Global editor instance (variant 13 requires static global)
extern MyEditor g_editor;
