#include <cmath>
#include <sstream>
#include <iostream>

#include "mathutils.hpp"
#include "point2d.hpp"

using namespace std;

Point2D::Point2D(double x, double y) :
	x(x), y(y) {
}

double Point2D::distance(const Point2D* other) const {
	return sqrt(this->distanceSquared(other));
}

double Point2D::distanceSquared(const Point2D* other) const {
	double xdist = (other->x-this->x);
	double ydist = (other->y - this->y);
	return xdist*xdist+ydist*ydist;
}

double Point2D::getX() const {
	return this->x;
}

double Point2D::getY() const {
	return this->y;
}

bool Point2D::operator==(const Point2D &other) const {
	return doublesEqual(this->x, other.x) && doublesEqual(this->y, other.y);
}

std::string Point2D::str () const {
	std::ostringstream s;
	s << "(" << this->x << ", " << this->y << ")";
	return s.str();
}