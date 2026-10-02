#pragma once
#include "shape.h"

namespace Project1 {

	using namespace System;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;

	public ref class ShapeObjectsEditor
	{
	public:
		ShapeObjectsEditor(void)
		{
			N = 113;
			pcshape = gcnew array<Shape^>(N);
			for (int i = 0; i < N; i++) pcshape[i] = nullptr;
			mode = ShapeMode::None;
			rubber = nullptr;
			isDrawing = false;
		}

		// Save shapes to a simple text file. Each line: TYPE x1 y1 x2 y2
		void SaveToFile(String^ path)
		{
			if (String::IsNullOrEmpty(path)) return;
			System::IO::StreamWriter^ w = gcnew System::IO::StreamWriter(path, false, System::Text::Encoding::UTF8);
			for (int i = 0; i < N; i++) {
				Shape^ s = pcshape[i];
				if (s == nullptr) continue;
				String^ type = "";
				if (dynamic_cast<PointShape^>(s) != nullptr) type = "POINT";
				else if (dynamic_cast<LineShape^>(s) != nullptr) type = "LINE";
				else if (dynamic_cast<RectShape^>(s) != nullptr) type = "RECT";
				else if (dynamic_cast<EllipseShape^>(s) != nullptr) type = "ELLIPSE";
				w->WriteLine(System::String::Format("{0} {1} {2} {3} {4}", type, s->X1, s->Y1, s->X2, s->Y2));
			}
			w->Close();
		}

		// Load shapes from a text file in the same format; clears existing shapes
		void LoadFromFile(String^ path, Control^ ctrl)
		{
			if (String::IsNullOrEmpty(path)) return;
			try {
				System::IO::StreamReader^ r = gcnew System::IO::StreamReader(path, System::Text::Encoding::UTF8);
				ClearAll(ctrl);
				String^ line;
				while ((line = r->ReadLine()) != nullptr) {
					array<String^>^ parts = line->Split(gcnew array<wchar_t>{' ', '\t'}, StringSplitOptions::RemoveEmptyEntries);
					if (parts->Length < 5) continue;
					String^ type = parts[0]->ToUpper();
					long x1 = System::Int64::Parse(parts[1]);
					long y1 = System::Int64::Parse(parts[2]);
					long x2 = System::Int64::Parse(parts[3]);
					long y2 = System::Int64::Parse(parts[4]);
					Shape^ obj = nullptr;
					if (type->Equals("POINT")) obj = gcnew PointShape(x1, y1);
					else if (type->Equals("LINE")) obj = gcnew LineShape(x1, y1, x2, y2);
					else if (type->Equals("RECT")) obj = gcnew RectShape(x1, y1, x2, y2);
					else if (type->Equals("ELLIPSE")) obj = gcnew EllipseShape(x1, y1, x2, y2);
					if (obj != nullptr) {
						for (int i = 0; i < N; i++) {
							if (pcshape[i] == nullptr) { pcshape[i] = obj; break; }
						}
					}
				}
				r->Close();
				if (ctrl != nullptr) ctrl->Invalidate();
			}
			catch (...) {
				// ignore errors for now
			}
		}

		// Remove last added shape
		void DeleteLast(Control^ ctrl)
		{
			for (int i = N - 1; i >= 0; i--) {
				if (pcshape[i] != nullptr) { pcshape[i] = nullptr; if (ctrl != nullptr) ctrl->Invalidate(); break; }
			}
		}

		~ShapeObjectsEditor() {}

		void StartPointEditor()
		{
			mode = ShapeMode::Point;
			// caller updates title
		}
		void StartLineEditor()
		{
			mode = ShapeMode::Line;
		}
		void StartRectEditor()
		{
			mode = ShapeMode::Rect;
		}
		void StartEllipseEditor()
		{
			mode = ShapeMode::Ellipse;
		}

		void OnLBdown(Control^ ctrl, int x, int y)
		{
			if (mode == ShapeMode::None) return;
			isDrawing = true;
			x1 = x; y1 = y; x2 = x; y2 = y;
			switch (mode) {
			case ShapeMode::Point:
				rubber = gcnew PointShape(x, y);
				break;
			case ShapeMode::Line:
				rubber = gcnew LineShape(x, y, x, y);
				break;
			case ShapeMode::Rect:
				rubber = gcnew RectShape(x, y, x, y);
				break;
			case ShapeMode::Ellipse:
				rubber = gcnew EllipseShape(x, y, x, y);
				break;
			default:
				rubber = nullptr; break;
			}
		}

		void OnMouseMove(Control^ ctrl, int x, int y)
		{
			if (!isDrawing) return;
			x2 = x; y2 = y;
			if (rubber != nullptr) {
				if (mode == ShapeMode::Rect) {
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
			if (ctrl != nullptr) ctrl->Invalidate();
		}

		void OnLBup(Control^ ctrl, int x, int y)
		{
			if (!isDrawing) return;
			isDrawing = false;
			x2 = x; y2 = y;
			Shape^ obj = nullptr;
			switch (mode) {
			case ShapeMode::Point:
				obj = gcnew PointShape(x1, y1);
				break;
			case ShapeMode::Line:
				obj = gcnew LineShape(x1, y1, x2, y2);
				break;
			case ShapeMode::Rect:
			{
				long cx = x1, cy = y1; long cornerX = x2, cornerY = y2;
				long left = cx - (cornerX - cx);
				long top = cy - (cornerY - cy);
				obj = gcnew RectShape(left, top, cornerX, cornerY);
			}
			break;
			case ShapeMode::Ellipse:
				obj = gcnew EllipseShape(x1, y1, x2, y2);
				break;
			default: break;
			}
			if (obj != nullptr) {
				for (int i = 0; i < N; i++) {
					if (pcshape[i] == nullptr) { pcshape[i] = obj; break; }
				}
			}
			rubber = nullptr;
			if (ctrl != nullptr) ctrl->Invalidate();
		}

		void OnPaint(PaintEventArgs^ e)
		{
			Graphics^ g = e->Graphics;
			for (int i = 0; i < N; i++) {
				if (pcshape[i] != nullptr) pcshape[i]->Draw(g);
			}
			if (isDrawing && rubber != nullptr) {
				Pen^ redPen = gcnew Pen(Color::Red);
				if (mode == ShapeMode::Line) {
					LineShape^ l = safe_cast<LineShape^>(rubber);
					g->DrawLine(redPen, (int)l->X1, (int)l->Y1, (int)l->X2, (int)l->Y2);
				}
				else if (mode == ShapeMode::Rect) {
					int left = (int)System::Math::Min(rubber->X1, rubber->X2);
					int top = (int)System::Math::Min(rubber->Y1, rubber->Y2);
					int width = (int)System::Math::Abs(rubber->X2 - rubber->X1);
					int height = (int)System::Math::Abs(rubber->Y2 - rubber->Y1);
					g->DrawRectangle(redPen, left, top, width, height);
				}
				else if (mode == ShapeMode::Ellipse) {
					int left = (int)System::Math::Min(rubber->X1, rubber->X2);
					int top = (int)System::Math::Min(rubber->Y1, rubber->Y2);
					int width = (int)System::Math::Abs(rubber->X2 - rubber->X1);
					int height = (int)System::Math::Abs(rubber->Y2 - rubber->Y1);
					g->DrawEllipse(redPen, left, top, width, height);
				}
				else if (mode == ShapeMode::Point) {
					g->FillRectangle(Brushes::Black, (int)rubber->X1, (int)rubber->Y1, 1, 1);
				}
				delete redPen;
			}
		}

		ShapeMode GetMode() { return mode; }

		void ClearAll(Control^ ctrl)
		{
			for (int i = 0; i < N; i++) pcshape[i] = nullptr;
			if (ctrl != nullptr) ctrl->Invalidate();
		}

	private:
		int N;
		array<Shape^>^ pcshape;
		ShapeMode mode;
		Shape^ rubber;
		bool isDrawing;
		long x1, y1, x2, y2;

		void UpdateTitle()
		{
			// caller (MyForm) updates actual title; keep for future
		}
	};

}
