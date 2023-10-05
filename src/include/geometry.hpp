#pragma once

/* I had to create these stuff just because the geometry
 * was getting a bit complicated without any abstractions.
 * NOTE: this is NOT a complete geometry library. It has
 * only the limited functionality I needed for this pong
 * clone.
 */

struct Point {
	double x, y;
	
	double SquaredDistanceTo(Point other);

	Point();
	Point(double x, double y);
};

class Line {
	Point p1, p2;
public:
	Line(Point a, Point b);
	double GetSlope();
	Point ClosestPointTo(Point a);
};

class Circle {
private:
	Point center;
	int radius;
public:
	Circle(Point c, int r);
	bool Intersects(Line l);
};

class Rectangle {
private:
	Point a, b, c, d;
public:
	Rectangle(Point a, Point b, Point c, Point d);
	bool Intersects(Circle c);
};
