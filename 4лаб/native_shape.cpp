#include "native_shape.h"
#include <windows.h>
#include <algorithm>
#include <cmath>
#pragma comment(lib, "gdi32.lib")

// Helper to create a GDI solid brush and select it, restoring previous
static HBRUSH CreateAndSelectSolidBrush(HDC hdc, COLORREF color, HBRUSH &oldBrush) {
	HBRUSH hBrush = CreateSolidBrush(color);
	oldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	return hBrush;
}

void PointShape::Draw(HDC hdc) {
	SetPixel(hdc, (int)xs1, (int)ys1, RGB(0,0,0));
}

void LineShape::Draw(HDC hdc) {
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hOld = (HPEN)SelectObject(hdc, hPen);
	MoveToEx(hdc, (int)Shape::X1(), (int)Shape::Y1(), NULL);
	LineTo(hdc, (int)Shape::X2(), (int)Shape::Y2());
	SelectObject(hdc, hOld);
	DeleteObject(hPen);
}

void RectShape::Draw(HDC hdc) {
	// Draw outline only (no fill)
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	int left = (int)min(Shape::X1(), Shape::X2());
	int top = (int)min(Shape::Y1(), Shape::Y2());
	int right = (int)max(Shape::X1(), Shape::X2());
	int bottom = (int)max(Shape::Y1(), Shape::Y2());
	Rectangle(hdc, left, top, right, bottom);
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}

void EllipseShape::Draw(HDC hdc) {
	// Draw filled ellipse with yellow fill and black contour
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HBRUSH hBrush = CreateSolidBrush(RGB(255,255,0));
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	int left = (int)min(Shape::X1(), Shape::X2());
	int top = (int)min(Shape::Y1(), Shape::Y2());
	int right = (int)max(Shape::X1(), Shape::X2());
	int bottom = (int)max(Shape::Y1(), Shape::Y2());
	Ellipse(hdc, left, top, right, bottom);
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hBrush);
}

void LineOOShape::Draw(HDC hdc) {
	// Use LineShape's drawing for the line part
	LineShape::Draw(hdc);
	// Draw circles at endpoints using EllipseShape drawing by creating small ellipses
	int r = 8;
	// left/top for first endpoint
	int cx1 = (int)Shape::X1(); int cy1 = (int)Shape::Y1();
	int cx2 = (int)Shape::X2(); int cy2 = (int)Shape::Y2();
	// draw filled black circles
	HBRUSH hBrush = CreateSolidBrush(RGB(0,0,0));
	HBRUSH old = (HBRUSH)SelectObject(hdc, hBrush);
	Ellipse(hdc, cx1 - r/2, cy1 - r/2, cx1 + r/2, cy1 + r/2);
	Ellipse(hdc, cx2 - r/2, cy2 - r/2, cx2 + r/2, cy2 + r/2);
	SelectObject(hdc, old);
	DeleteObject(hBrush);
}

void CubeShape::Draw(HDC hdc) {
	// Draw cube as two outline-only rectangles connected with edges (no fill)
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0,0,0));
	HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

	int left = (int)min(Shape::X1(), Shape::X2());
	int top = (int)min(Shape::Y1(), Shape::Y2());
	int w = (int)abs(Shape::X2()-Shape::X1());
	int h = (int)abs(Shape::Y2()-Shape::Y1());
	// back rectangle offset (visual perspective)
	int dx = w/4;
	int dy = -h/4;

	// front rectangle (outline only)
	Rectangle(hdc, left, top, left + w, top + h);
	// back rectangle (outline only)
	Rectangle(hdc, left + dx, top + dy, left + dx + w, top + dy + h);

	// Connect corners with lines (edges)
	MoveToEx(hdc, left, top, NULL);
	LineTo(hdc, left + dx, top + dy);
	MoveToEx(hdc, left + w, top, NULL);
	LineTo(hdc, left + dx + w, top + dy);
	MoveToEx(hdc, left, top + h, NULL);
	LineTo(hdc, left + dx, top + dy + h);
	MoveToEx(hdc, left + w, top + h, NULL);
	LineTo(hdc, left + dx + w, top + dy + h);

	// restore GDI objects
	SelectObject(hdc, hOldPen);
	SelectObject(hdc, hOldBrush);
	DeleteObject(hPen);
}
