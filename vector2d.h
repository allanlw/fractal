#ifndef _VECTOR2D_H
#define _VECTOR2D_H

#include <string>

class Point2D;

#include "point2d.h"

class Vector2D {
private:
	double magnitude;
	double angle;

	void normalize();
public:
	Vector2D(const Vector2D& other);
	Vector2D(const double magnitude, const double angle);
	Vector2D(const Point2D& origin, const Point2D& point2d, bool unit=false);
	double crossProduct(const Vector2D& other) const;
	double dotProduct(const Vector2D& other) const;
	double selfDotProduct() const;
	double angleBetween(const Vector2D& other) const;
	double getAngle() const;
	double getMagnitude() const;
	Vector2D getOpposite() const;
	bool operator==(const Vector2D &other) const;
	bool operator*(const Vector2D &other) const;
	std::string str () const;
};

#endif
