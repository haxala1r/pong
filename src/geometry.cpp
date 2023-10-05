#include "geometry.hpp"
#include <iostream>
#include <limits>

using std::numeric_limits;
using namespace std;

bool between_p(double x, double l, double h) {
	if (h < l)
		return between_p(x, h, l);
	return ((l <= x) && (x <= h));
}

Point::Point() {}
Point::Point(double a, double b) : x(a), y(b) { }
double Point::SquaredDistanceTo(Point other) {
	double dx = other.x - x;
	double dy = other.y - y;
	return dx * dx + dy * dy;
}

Line::Line(Point a, Point b) {
	p1 = a;
	p2 = b;
}

double Line::GetSlope() {
	double dx = p2.x - p1.x;
	double dy = p2.y - p1.y;
	return dy / dx;
}

Point Line::ClosestPointTo(Point a) {
	/* Draw a perpendicular line from a to this line. When this is done,
	 * find the point where the two lines intersect; that's (a) projected
	 * to this line. If that point is on the line segment represented
	 * by this class, return it. Otherwise try the endpoints.
	 * the lines have the formulas:
	 *  y = sx + m for the first one.
	 *  y = -1/s * x + c for the one we draw.
	 * God everything gets so arcane when maths gets involved.
	 * sorry for the variable naming.
	 */
	double crossx;
	double crossy;
	double s = GetSlope();
	if (s == numeric_limits<double>::infinity()) {
		/* Special case. Line segment is entirely vertical. */
		crossx = p1.x;
		crossy = a.y;
	} else if (s == 0) {
		/* Special case. Line segment is entirely horizontal. */
		crossx = a.x;
		crossy = p1.y;
	} else {
		double m = p1.y - s * p1.x;
		double c = a.y + (1 / s) * a.x;
		crossx = (c - m) * (s / (s * s + 1));
		crossy = crossx * s + m;
	}
	/* Remember that the Line class actually holds a Line *Segment*,
	 * mathematically speaking. So we need to also check if this line segment
	 * actually contains the point we calculated. If not, the
	 * real closest point must be one of the ends.
	 */
	
	if (between_p(crossx, p1.x, p2.x) && between_p(crossy, p1.y, p2.y)) {
		/* NOTE: this truncates */
		return Point(crossx, crossy);
	}
	double d1 = a.SquaredDistanceTo(p1);
	double d2 = a.SquaredDistanceTo(p2);
	if (d1 < d2)
		return p1;
	return p2;
}

Circle::Circle(Point c, int r) {
	center = c;
	radius = r;
}

bool Circle::Intersects(Line line) {
	Point closest = line.ClosestPointTo(center);
	return (center.SquaredDistanceTo(closest) <= (radius * radius));
}

Rectangle::Rectangle(Point a, Point b, Point c, Point d) : a(a), b(b), c(c), d(d) { }

bool Rectangle::Intersects(Circle circle) {
	Line lines[] = { Line(a, b), Line(b, c), Line(c, d), Line(d, a)};
	for (auto i : lines) {
		if (circle.Intersects(i))
			return true;
	}
	return false;
}
