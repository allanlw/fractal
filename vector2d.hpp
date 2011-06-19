#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

class Point2D;

#include <string>
#include "point2d.hpp"

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

#endif /* VECTOR2D_HPP_ */
