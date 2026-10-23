#include "my_editor.h"
#include <fstream>
#include <sstream>

MyEditor::MyEditor()
{
	N = 113; // variant 13
	pshape = new Shape * [N];
	for (int i = 0; i < N; ++i) pshape[i] = nullptr;
	prototype = nullptr;
	rubber = nullptr;
	isDrawing = false;
	x1 = y1 = x2 = y2 = 0;
}

MyEditor::~MyEditor()
{
	if (pshape) {
		for (int i = 0; i < N; ++i) {
			if (pshape[i]) delete pshape[i];
		}
		delete[] pshape;
		pshape = nullptr;
	}
	if (prototype) { delete prototype; prototype = nullptr; }
	if (rubber) { delete rubber; rubber = nullptr; }
}

void MyEditor::Start(Shape* proto)
{
	if (prototype) delete prototype;
	prototype = proto;
}

void MyEditor::AppendShapeToFile(Shape* s)
{
	if (!s) return;
	FILE* fout = nullptr;
	fopen_s(&fout, "shapes_log.txt", "a+t");
	if (fout) {
		std::wstring t = s->TypeName();
		fprintf(fout, "%ls\t%ld\t%ld\t%ld\t%ld\n", t.c_str(), s->X1(), s->Y1(), s->X2(), s->Y2());
		fclose(fout);
	}
}

void MyEditor::OnLBdown(HWND hwnd, int x, int y)
{
	if (!prototype) return;
	isDrawing = true;
	x1 = x; y1 = y; x2 = x; y2 = y;
	rubber = prototype->CreateInstance(x, y, x, y);
}

void MyEditor::OnMouseMove(HWND hwnd, int x, int y)
{
	if (!isDrawing || !rubber) return;
	x2 = x; y2 = y;
	if (prototype) {
		std::wstring t = prototype->TypeName();
		if (t == L"RECT" || t == L"CUBE") {
			long cx = x1, cy = y1;
			long cornerX = x; long cornerY = y;
			long left = cx - (cornerX - cx);
			long top = cy - (cornerY - cy);
			rubber->Set(left, top, cornerX, cornerY);
		}
		else {
			rubber->Set(x1, y1, x2, y2);
		}
	}
	else {
		rubber->Set(x1, y1, x2, y2);
	}
}

Shape* MyEditor::OnLBup(HWND hwnd, int x, int y)
{
	if (!isDrawing) return nullptr;
	isDrawing = false;
	x2 = x; y2 = y;
	Shape* obj = nullptr;
	if (rubber) {
		obj = rubber;
		rubber = nullptr;
		if (prototype) {
			std::wstring t = prototype->TypeName();
			if (t == L"RECT" || t == L"CUBE") {
				long cx = x1, cy = y1;
				long cornerX = x2, cornerY = y2;
				long left = cx - (cornerX - cx);
				long top = cy - (cornerY - cy);
				obj->Set(left, top, cornerX, cornerY);
			}
			else {
				obj->Set(x1, y1, x2, y2);
			}
		}
		else {
			obj->Set(x1, y1, x2, y2);
		}
		for (int i = 0; i < N; ++i) {
			if (!pshape[i]) {
				pshape[i] = obj;
				AppendShapeToFile(obj);
				break;
			}
		}
	}
	return obj; // Повертаємо створений об'єкт
}

void MyEditor::OnPaint(HDC hdc)
{
	if (!hdc) return;
	for (int i = 0; i < N; ++i) {
		if (pshape[i]) pshape[i]->Draw(hdc);
	}
	if (isDrawing && rubber) {
		HPEN hPen = CreatePen(PS_DASH, 1, RGB(255, 0, 0));
		HPEN oldPen = (HPEN)SelectObject(hdc, hPen);
		std::wstring t = prototype ? prototype->TypeName() : L"";
		if (t == L"LINE" || t == L"LINEOO") {
			MoveToEx(hdc, (int)rubber->X1(), (int)rubber->Y1(), NULL);
			LineTo(hdc, (int)rubber->X2(), (int)rubber->Y2());
		}
		else if (t == L"RECT") {
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			int left = (int)min(rubber->X1(), rubber->X2());
			int top = (int)min(rubber->Y1(), rubber->Y2());
			int right = (int)max(rubber->X1(), rubber->X2());
			int bottom = (int)max(rubber->Y1(), rubber->Y2());
			Rectangle(hdc, left, top, right, bottom);
			SelectObject(hdc, oldBrush);
		}
		else if (t == L"CUBE") {
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			int left = (int)min(rubber->X1(), rubber->X2());
			int top = (int)min(rubber->Y1(), rubber->Y2());
			int w = (int)abs(rubber->X2() - rubber->X1());
			int h = (int)abs(rubber->Y2() - rubber->Y1());
			int dx = w / 4;
			int dy = -h / 4;
			Rectangle(hdc, left, top, left + w, top + h);
			Rectangle(hdc, left + dx, top + dy, left + dx + w, top + dy + h);
			MoveToEx(hdc, left, top, NULL);
			LineTo(hdc, left + dx, top + dy);
			MoveToEx(hdc, left + w, top, NULL);
			LineTo(hdc, left + dx + w, top + dy);
			MoveToEx(hdc, left, top + h, NULL);
			LineTo(hdc, left + dx, top + dy + h);
			MoveToEx(hdc, left + w, top + h, NULL);
			LineTo(hdc, left + dx + w, top + dy + h);
			SelectObject(hdc, oldBrush);
		}
		else if (t == L"ELLIPSE") {
			HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
			int left = (int)min(rubber->X1(), rubber->X2());
			int top = (int)min(rubber->Y1(), rubber->Y2());
			int right = (int)max(rubber->X1(), rubber->X2());
			int bottom = (int)max(rubber->Y1(), rubber->Y2());
			Ellipse(hdc, left, top, right, bottom);
			SelectObject(hdc, oldBrush);
		}
		else if (t == L"POINT") {
			SetPixel(hdc, (int)rubber->X1(), (int)rubber->Y1(), RGB(0, 0, 0));
		}
		SelectObject(hdc, oldPen);
		DeleteObject(hPen);
	}
}

void MyEditor::ClearAll()
{
	for (int i = 0; i < N; ++i) {
		if (pshape[i]) { delete pshape[i]; pshape[i] = nullptr; }
	}
}

void MyEditor::DeleteLast()
{
	for (int i = N - 1; i >= 0; --i) {
		if (pshape[i]) { delete pshape[i]; pshape[i] = nullptr; break; }
	}
}

bool MyEditor::SaveToFile(const std::wstring& path)
{
	std::wofstream ofs(path);
	if (!ofs) return false;
	for (int i = 0; i < N; ++i) {
		if (!pshape[i]) continue;
		Shape* s = pshape[i];
		ofs << s->TypeName() << L" " << s->X1() << L" " << s->Y1() << L" " << s->X2() << L" " << s->Y2() << L"\n";
	}
	return true;
}

bool MyEditor::LoadFromFile(const std::wstring& path)
{
	std::wifstream ifs(path);
	if (!ifs) return false;
	ClearAll();
	std::wstring type;
	long a, b, c, d;
	while (ifs >> type >> a >> b >> c >> d) {
		Shape* obj = nullptr;
		if (type == L"POINT") obj = new PointShape(a, b);
		else if (type == L"LINEOO") obj = new LineOOShape(a, b, c, d);
		else if (type == L"LINE") obj = new LineShape(a, b, c, d);
		else if (type == L"RECT") obj = new RectShape(a, b, c, d);
		else if (type == L"ELLIPSE") obj = new EllipseShape(a, b, c, d);
		else if (type == L"CUBE") obj = new CubeShape(a, b, c, d);
		if (obj) {
			for (int i = 0; i < N; ++i) {
				if (!pshape[i]) { pshape[i] = obj; break; }
			}
		}
	}
	return true;
}