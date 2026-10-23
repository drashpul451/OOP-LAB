#pragma once
#include <windows.h>
#include <string>
#include "native_shape.h"

class MyEditor {
private:
	MyEditor();
	~MyEditor();
	MyEditor(const MyEditor&) = delete;
	MyEditor& operator=(const MyEditor&) = delete;

	Shape** pshape;
	int N;
	Shape* prototype; // owned prototype used by Start(Shape*)
	Shape* rubber;    // current rubber object (owned)
	bool isDrawing;
	long x1, y1, x2, y2;

public:
	// Singleton Меєрса
	static MyEditor& getInstance() {
		static MyEditor instance;
		return instance;
	}

	void Start(Shape* prototype);
	void OnLBdown(HWND hwnd, int x, int y);
	void OnMouseMove(HWND hwnd, int x, int y);
	Shape* OnLBup(HWND hwnd, int x, int y);
	void OnPaint(HDC hdc);

	void ClearAll();
	void DeleteLast();

	bool SaveToFile(const std::wstring& path);
	bool LoadFromFile(const std::wstring& path);

	// Автоматичний запис об'єкта у файл
	void AppendShapeToFile(Shape* s);
};