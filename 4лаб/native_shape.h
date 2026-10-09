#pragma once
#include <windows.h>
#include <string>

class Shape {
protected:
	long xs1, ys1, xs2, ys2;
public:
	Shape() : xs1(0), ys1(0), xs2(0), ys2(0) {}
	virtual ~Shape() {}
	void Set(long x1, long y1, long x2, long y2) { xs1 = x1; ys1 = y1; xs2 = x2; ys2 = y2; }
	long X1() const { return xs1; }
	long Y1() const { return ys1; }
	long X2() const { return xs2; }
	long Y2() const { return ys2; }
	virtual void Draw(HDC hdc) = 0;
	// Create a new instance of this shape initialized with coordinates
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) = 0;
	// For saving / identifying type
	virtual std::wstring TypeName() const = 0;
};

class PointShape : public virtual Shape {
public:
	PointShape() : Shape() {}
	PointShape(long x, long y) { Set(x,y,x,y); }
	virtual ~PointShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new PointShape(x1,y1); }
	virtual std::wstring TypeName() const override { return L"POINT"; }
};

class LineShape : public virtual Shape {
public:
	LineShape() : Shape() {}
	LineShape(long x1,long y1,long x2,long y2) { Set(x1,y1,x2,y2); }
	virtual ~LineShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new LineShape(x1,y1,x2,y2); }
	virtual std::wstring TypeName() const override { return L"LINE"; }
};

class RectShape : public virtual Shape {
public:
	RectShape() : Shape() {}
	RectShape(long x1,long y1,long x2,long y2) { Set(x1,y1,x2,y2); }
	virtual ~RectShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new RectShape(x1,y1,x2,y2); }
	virtual std::wstring TypeName() const override { return L"RECT"; }
};

class EllipseShape : public virtual Shape {
public:
	EllipseShape() : Shape() {}
	EllipseShape(long x1,long y1,long x2,long y2) { Set(x1,y1,x2,y2); }
	virtual ~EllipseShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new EllipseShape(x1,y1,x2,y2); }
	virtual std::wstring TypeName() const override { return L"ELLIPSE"; }
};

// Line with circles at endpoints: multiple inheritance from LineShape and EllipseShape
class LineOOShape : public LineShape, public EllipseShape {
public:
	LineOOShape() : LineShape(), EllipseShape() {}
	LineOOShape(long x1,long y1,long x2,long y2) { Shape::Set(x1,y1,x2,y2); }
	virtual ~LineOOShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new LineOOShape(x1,y1,x2,y2); }
	virtual std::wstring TypeName() const override { return L"LINEOO"; }
};

// Cube wireframe: multiple inheritance from LineShape and RectShape
class CubeShape : public LineShape, public RectShape {
public:
	CubeShape() : LineShape(), RectShape() {}
	CubeShape(long x1,long y1,long x2,long y2) { Shape::Set(x1,y1,x2,y2); }
	virtual ~CubeShape() {}
	virtual void Draw(HDC hdc) override;
	virtual Shape* CreateInstance(long x1, long y1, long x2, long y2) override { return new CubeShape(x1,y1,x2,y2); }
	virtual std::wstring TypeName() const override { return L"CUBE"; }
};
// No-op patch: touch file to update timestamp
