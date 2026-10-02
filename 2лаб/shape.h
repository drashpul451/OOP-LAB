#pragma once

namespace Project1 {

	using namespace System;
	using namespace System::Drawing;

	public enum class ShapeMode { None, Point, Line, Rect, Ellipse };

	public ref class Shape abstract
	{
	protected:
		long xs1, ys1, xs2, ys2;
	public:
		Shape() { xs1 = ys1 = xs2 = ys2 = 0; }
		void Set(long x1, long y1, long x2, long y2) { xs1 = x1; ys1 = y1; xs2 = x2; ys2 = y2; }
		property long X1 { long get() { return xs1; } }
		property long Y1 { long get() { return ys1; } }
		property long X2 { long get() { return xs2; } }
		property long Y2 { long get() { return ys2; } }
		virtual void Draw(Graphics^ g) abstract;
	};

	public ref class PointShape : public Shape
	{
	public:
		PointShape() : Shape() {}
		PointShape(long x, long y) { Set(x, y, x, y); }
		virtual void Draw(Graphics^ g) override
		{
			if (g == nullptr) return;
			g->FillRectangle(Brushes::Black, xs1, ys1, 1, 1);
		}
	};

	public ref class LineShape : public Shape
	{
	public:
		LineShape() : Shape() {}
		LineShape(long x1, long y1, long x2, long y2) { Set(x1, y1, x2, y2); }
		virtual void Draw(Graphics^ g) override
		{
			if (g == nullptr) return;
			Pen^ p = gcnew Pen(Color::Black);
			g->DrawLine(p, (int)xs1, (int)ys1, (int)xs2, (int)ys2);
			delete p;
		}
	};

	public ref class RectShape : public Shape
	{
	public:
		RectShape() : Shape() {}
		RectShape(long x1, long y1, long x2, long y2) { Set(x1, y1, x2, y2); }
		virtual void Draw(Graphics^ g) override
		{
			if (g == nullptr) return;
			Pen^ p = gcnew Pen(Color::Black);
			int left = (int)Math::Min(xs1, xs2);
			int top = (int)Math::Min(ys1, ys2);
			int width = (int)System::Math::Abs(xs2 - xs1);
			int height = (int)System::Math::Abs(ys2 - ys1);
			g->DrawRectangle(p, left, top, width, height);
			delete p;
		}
	};

	public ref class EllipseShape : public Shape
	{
	public:
		EllipseShape() : Shape() {}
		EllipseShape(long x1, long y1, long x2, long y2) { Set(x1, y1, x2, y2); }
		virtual void Draw(Graphics^ g) override
		{
			if (g == nullptr) return;
			Pen^ p = gcnew Pen(Color::Black);
			Brush^ br = gcnew SolidBrush(Color::Lime);

			int left = (int)Math::Min(xs1, xs2);
			int top = (int)Math::Min(ys1, ys2);
			int width = (int)System::Math::Abs(xs2 - xs1);
			int height = (int)System::Math::Abs(ys2 - ys1);
			if (width > 0 && height > 0) g->FillEllipse(br, left, top, width, height);
			g->DrawEllipse(p, left, top, width, height);
			delete p; delete br;
		}
	};

}
