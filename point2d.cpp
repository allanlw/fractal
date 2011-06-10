#include <cmath>
#include <sstream>
#include <iostream>

#include "mathutils.hpp"
#include "point2d.hpp"

using namespace std;

Point2D::Point2D(double x, double y) :
	x(x), y(y) {
}

Point2D::Point2D(const Point2D& other) {
	x = other.x;
	y = other.y;
}

double Point2D::distance(const Point2D& other) const {
	return sqrt(distanceSquared(other));
}

double Point2D::distanceSquared(const Point2D& other) const {
	const double xdist = (other.x-x);
	const double ydist = (other.y-y);
	return xdist*xdist+ydist*ydist;
}

double Point2D::getX() const {
	return x;
}

double Point2D::getY() const {
	return y;
}

bool Point2D::operator==(const Point2D &other) const {
	return doublesEqual(this->x, other.x) && doublesEqual(this->y, other.y);
}

std::string Point2D::str () const {
	std::ostringstream s;
	s << "(" << this->x << ", " << this->y << ")";
	return s.str();
}

Vector2D Point2D::operator-(const Point2D& other) const{
	return Vector2D(other, *this);
}

Point2D& Point2D::operator=(const Point2D& other) {
	x = other.x;
	y = other.y;
	return *this;
}

void Point2D::serialize(ostream& out) const {
	out.put('P');
	serializeDouble(out, x);
	serializeDouble(out, y);
}
