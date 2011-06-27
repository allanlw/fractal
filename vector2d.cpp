#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "vector2d.hpp"
#include "mathutils.hpp"

using namespace std;

Vector2D::Vector2D(const Vector2D& other) : magnitude(other.magnitude), angle(other.angle) {
}

void Vector2D::normalize() {
	while (angle < 0) {
		angle += M_PI * 2;
	}
	while (angle > M_PI * 2) {
		angle -= M_PI * 2;
	}
	if (angle >= M_PI) {
		magnitude *= -1;
		angle -= M_PI;
	}
}

Vector2D::Vector2D(const Point2D& origin, const Point2D& point2, bool unit) :
magnitude(unit?1.0:origin.distance(point2)), angle(atan2(point2.getY() - origin.getY(), point2.getX() - origin.getX())) {
	normalize();
}

Vector2D::Vector2D(const double magnitude, const double angle) :
	magnitude(magnitude), angle(angle) {
	normalize();
}

double Vector2D::getMagnitude() const {
	return magnitude;
}

double Vector2D::angleBetween(const Vector2D & other) const {
	return abs(other.angle - angle);
}

Vector2D Vector2D::getOpposite() const {
	return Vector2D(magnitude * -1.0, angle);
}

double Vector2D::crossProduct(const Vector2D& other) const {
	return magnitude * other.magnitude * sin(angleBetween(other));
}

double Vector2D::dotProduct(const Vector2D& other) const {
	return magnitude * other.magnitude * cos(angleBetween(other));
}

double Vector2D::selfDotProduct() const {
	return magnitude*magnitude;
}

double Vector2D::getAngle() const {
	return angle;
}

bool Vector2D::operator==(const Vector2D &other) const {
	return doublesEqual(magnitude, other.magnitude) && doublesEqual(angle, other.angle);
}

std::string Vector2D::str () const {
	std::ostringstream s;
	s << "<" << magnitude << ", " << angle << ">";
	return s.str();
}
