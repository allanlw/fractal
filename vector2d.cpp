#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "vector2d.hpp"
#include "mathutils.hpp"

using namespace std;

void Vector2D::normalize() {
	while (this->angle < 0) {
		angle += PI * 2;
	}
	while (this->angle > PI * 2) {
		angle -= PI * 2;
	}
	if (this->angle >= PI) {
		magnitude *= -1;
		angle -= PI;
	}
}

Vector2D::Vector2D(const Point2D& origin, const Point2D& point2, bool unit) {
	if (!unit) {
		magnitude = origin.distance(point2);
	} else {
		magnitude = 1.0;
	}
	angle = atan2(point2.getY() - origin.getY(),
	                    point2.getX() - origin.getX());
	normalize();
}

double Vector2D::getMagnitude() const {
	return this->magnitude;
}

double Vector2D::angleBetween(const Vector2D & other) const {
	return abs(other.getAngle() - angle);
}

Vector2D::Vector2D(const double magnitude, const double angle) :
	magnitude(magnitude), angle(angle) {
	this->normalize();
}

Vector2D Vector2D::getOpposite() const {
	return Vector2D(magnitude * -1.0, angle);
}

double Vector2D::crossProduct(const Vector2D& other) const {
	return magnitude * other.getMagnitude() * sin(angleBetween(
			other));
}

double Vector2D::getAngle() const {
	return angle;
}

bool Vector2D::operator==(const Vector2D &other) const {
	return doublesEqual(magnitude, other.magnitude) && doublesEqual(angle, other.angle);
}

std::string Vector2D::str () const {
	std::ostringstream s;
	s << "<" << this->magnitude << ", " << this->angle << ">";
	return s.str();
}
