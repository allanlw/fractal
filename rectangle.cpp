#include <string>
#include <sstream>
#include <iostream>

#include "rectangle.hpp"
#include "point2d.hpp"
#include "mathutils.hpp"

Rectangle::Rectangle(const double x, const double y, const double w,
		const double h) :
	x(x), y(y), w(w), h(h) {
}

double Rectangle::getX() const {
	return this->x;
}

double Rectangle::getY() const {
	return this->y;
}

double Rectangle::getWidth() const {
	return this->w;
}

double Rectangle::getHeight() const {
	return this->h;
}

double Rectangle::getArea() const {
	return this->h*this->w;
}

bool Rectangle::pointInside(const Point2D& point) const {
	double px = point.getX();
	double py = point.getY();
	return (((px >= this->x || doublesEqual(px, this->x)) &&
			 (px <= this->x+this->w || doublesEqual(px, this->x+this->w))) &&
			((py >= this->y || doublesEqual(py, this->y)) &&
			 (px <= this->y+this->h || doublesEqual(py, this->y+this->h))));
}

std::string Rectangle::str () const {
	std::ostringstream s;
	s << "(X:" << this->x << ",Y:" << this->y << ", W:" << this->w << ", H:" << this->h << ")";
	return s.str();
}

double Rectangle::getTop() const {
	return y;
}
double Rectangle::getLeft() const {
	return x;
}
double Rectangle::getBottom() const {
	return y+h;
}
double Rectangle::getRight() const {
	return x+w;
}
