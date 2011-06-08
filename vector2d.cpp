#include <cmath>
#include <iostream>
#include <sstream>
#include <string>

#include "vector2d.hpp"
#include "mathutils.hpp"

using namespace std;

void Vector2D::normalize() {
	while (this->angle < 0) {
		this->angle += PI * 2;
	}
	while (this->angle > PI * 2) {
		this->angle -= PI * 2;
	}
	if (this->angle >= PI) {
		this->magnitude *= -1;
		this->angle -= PI;
	}
}

Vector2D::Vector2D(const Point2D& origin, const Point2D& point2) {
	this->magnitude = origin.distance(point2);
	this->angle = atan2(point2.getY()
			- origin.getY(), point2.getX() - origin.getX());
	this->normalize();
}

double Vector2D::getMagnitude() const {
	return this->magnitude;
}

double Vector2D::angleBetween(const Vector2D & other) const {
	if (this->angle > other.getAngle()) {
		return this->angle - other.getAngle();
	} else {
		return other.getAngle() - this->angle;
	}
}

Vector2D::Vector2D(const double magnitude, const double angle) :
	magnitude(magnitude), angle(angle) {
	this->normalize();
}

Vector2D Vector2D::getOpposite() const {
	return Vector2D(this->magnitude * -1.0, this->angle);
}

double Vector2D::crossProduct(const Vector2D& other) const {
	return this->magnitude * other.getMagnitude() * sin(this->angleBetween(
			other));
}

double Vector2D::getAngle() const {
	return this->angle;
}

bool Vector2D::operator==(const Vector2D &other) const {
	return doublesEqual(this->magnitude, other.magnitude) && doublesEqual(this->angle, other.angle);
}

std::string Vector2D::str () const {
	std::ostringstream s;
	s << "<" << this->magnitude << ", " << this->angle << ">";
	return s.str();
}
