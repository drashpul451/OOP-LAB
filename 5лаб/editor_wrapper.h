#pragma once
#include "my_editor.h"
#include "my_table.h"
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Project1 {

	public enum class ManagedShapeMode { None = 0, Point = 1, Line = 2, Rect = 3, Ellipse = 4, LineOO = 5, Cube = 6 };

	public ref class ManagedEditor
	{
	private:
		MyTable* pTable;

	public:
		ManagedEditor() {
			pTable = new MyTable();
		}

		~ManagedEditor() {
			if (pTable) {
				delete pTable;
				pTable = nullptr;
			}
		}

		void ToggleTableWindow(Control^ parentCtrl) {
			if (!pTable) return;
			if (!pTable->IsOpen()) {
				HWND h = parentCtrl ? (HWND)parentCtrl->Handle.ToPointer() : NULL;
				pTable->Show(h);
			}
			else {
				pTable->Hide();
			}
		}

		void CloseTableWindow() {
			if (pTable) pTable->Hide();
		}

		void StartByMode(ManagedShapeMode m) {
			Shape* proto = nullptr;
			switch (m) {
			case ManagedShapeMode::Point: proto = new PointShape(0, 0); break;
			case ManagedShapeMode::Line: proto = new LineShape(0, 0, 0, 0); break;
			case ManagedShapeMode::Rect: proto = new RectShape(0, 0, 0, 0); break;
			case ManagedShapeMode::Ellipse: proto = new EllipseShape(0, 0, 0, 0); break;
			case ManagedShapeMode::LineOO: proto = new LineOOShape(0, 0, 0, 0); break;
			case ManagedShapeMode::Cube: proto = new CubeShape(0, 0, 0, 0); break;
			default: break;
			}
			if (proto) MyEditor::getInstance().Start(proto);
		}

		void OnLBdown(Control^ ctrl, int x, int y) {
			HWND h = ctrl ? (HWND)ctrl->Handle.ToPointer() : nullptr;
			MyEditor::getInstance().OnLBdown(h, x, y);
		}

		void OnMouseMove(Control^ ctrl, int x, int y) {
			HWND h = ctrl ? (HWND)ctrl->Handle.ToPointer() : nullptr;
			MyEditor::getInstance().OnMouseMove(h, x, y);
		}

		void OnLBup(Control^ ctrl, int x, int y) {
			HWND h = ctrl ? (HWND)ctrl->Handle.ToPointer() : nullptr;
			Shape* createdShape = MyEditor::getInstance().OnLBup(h, x, y);
			if (createdShape && pTable && pTable->IsOpen()) {
				pTable->AddRow(createdShape->TypeName(), createdShape->X1(), createdShape->Y1(), createdShape->X2(), createdShape->Y2());
			}
		}

		void OnPaint(PaintEventArgs^ e) {
			if (e == nullptr) return;
			Graphics^ g = e->Graphics;
			System::IntPtr hdc = g->GetHdc();
			MyEditor::getInstance().OnPaint((HDC)hdc.ToPointer());
			g->ReleaseHdc(hdc);
		}

		void ClearAll(Control^ ctrl) {
			MyEditor::getInstance().ClearAll();
			if (pTable) pTable->Clear();
			if (ctrl) ctrl->Invalidate();
		}

		void DeleteLast(Control^ ctrl) {
			MyEditor::getInstance().DeleteLast();
			if (ctrl) ctrl->Invalidate();
		}

		bool SaveToFile(String^ path) {
			if (String::IsNullOrEmpty(path)) return false;
			std::wstring p = msclr::interop::marshal_as<std::wstring>(path);
			return MyEditor::getInstance().SaveToFile(p);
		}

		bool LoadFromFile(String^ path, Control^ ctrl) {
			if (String::IsNullOrEmpty(path)) return false;
			std::wstring p = msclr::interop::marshal_as<std::wstring>(path);
			bool ok = MyEditor::getInstance().LoadFromFile(p);
			if (ctrl) ctrl->Invalidate();
			return ok;
		}
	};
}