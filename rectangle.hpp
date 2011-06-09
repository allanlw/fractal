#ifndef _RECTANGLE_HPP
#define _RECTANGLE_HPP

#include <string>

#include "point2d.hpp"

class Rectangle {
private:
	double x;
	double y;
	double w;
	double h;
public:
	Rectangle(const double x, const double y, const double w, const double h);
	double getX() const;
	double getY() const;
	double getWidth() const;
	double getHeight() const;
	double getLeft() const;
	double getRight() const;
	double getTop() const;
	double getBottom() const;
	double getArea() const;
	bool pointInside(const Point2D& point) const;
	std::string str() const;
};
#endif
