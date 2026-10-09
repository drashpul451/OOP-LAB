#pragma once
#include "my_editor.h"
#include <msclr/marshal_cppstd.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace Project1 {

	public enum class ManagedShapeMode { None = 0, Point = 1, Line = 2, Rect = 3, Ellipse = 4, LineOO = 5, Cube = 6 };

	public ref class ManagedEditor
	{
	public:
		ManagedEditor() {
			// nothing to construct; uses global g_editor
		}
		~ManagedEditor() {
			// do not delete global
		}

		void StartByMode(ManagedShapeMode m) {
			// Create a native prototype and hand it to native editor (takes ownership)
			Shape* proto = nullptr;
			switch (m) {
			case ManagedShapeMode::Point: proto = new PointShape(0,0); break;
			case ManagedShapeMode::Line: proto = new LineShape(0,0,0,0); break;
			case ManagedShapeMode::Rect: proto = new RectShape(0,0,0,0); break;
			case ManagedShapeMode::Ellipse: proto = new EllipseShape(0,0,0,0); break;
			case ManagedShapeMode::LineOO: proto = new LineOOShape(0,0,0,0); break;
			case ManagedShapeMode::Cube: proto = new CubeShape(0,0,0,0); break;
			default: break;
			}
			if (proto) g_editor.Start(proto);
		}

		void OnLBdown(Control^ ctrl, int x, int y) {
			HWND h = nullptr;
			if (ctrl != nullptr) {
				System::IntPtr ip = ctrl->Handle;
				h = (HWND)ip.ToPointer();
			}
			g_editor.OnLBdown(h, x, y);
		}
		void OnMouseMove(Control^ ctrl, int x, int y) {
			HWND h = nullptr;
			if (ctrl != nullptr) {
				System::IntPtr ip = ctrl->Handle;
				h = (HWND)ip.ToPointer();
			}
			g_editor.OnMouseMove(h, x, y);
		}
		void OnLBup(Control^ ctrl, int x, int y) {
			HWND h = nullptr;
			if (ctrl != nullptr) {
				System::IntPtr ip = ctrl->Handle;
				h = (HWND)ip.ToPointer();
			}
			g_editor.OnLBup(h, x, y);
		}

		void OnPaint(PaintEventArgs^ e) {
			if (e == nullptr) return;
			Graphics^ g = e->Graphics;
			System::IntPtr hdc = g->GetHdc();
			HDC nativeHdc = (HDC)hdc.ToPointer();
			g_editor.OnPaint(nativeHdc);
			g->ReleaseHdc(hdc);
		}

		void ClearAll(Control^ ctrl) {
			g_editor.ClearAll();
			if (ctrl) ctrl->Invalidate();
		}

		void DeleteLast(Control^ ctrl) {
			g_editor.DeleteLast();
			if (ctrl) ctrl->Invalidate();
		}

		bool SaveToFile(String^ path) {
			if (String::IsNullOrEmpty(path)) return false;
			std::wstring p = msclr::interop::marshal_as<std::wstring>(path);
			return g_editor.SaveToFile(p);
		}

		bool LoadFromFile(String^ path, Control^ ctrl) {
			if (String::IsNullOrEmpty(path)) return false;
			std::wstring p = msclr::interop::marshal_as<std::wstring>(path);
			bool ok = g_editor.LoadFromFile(p);
			if (ctrl) ctrl->Invalidate();
			return ok;
		}
	};
}
