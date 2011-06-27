#ifndef _POINT2D_H
#define _POINT2D_H

#include <string>
#include <ostream>
#include <istream>

class Vector2D;

#include "vector2d.h"

class Point2D {
private:
	double x;
	double y;
public:
	Point2D(double x, double y);
	Point2D(const Point2D& other);
	Point2D(std::istream& in);
	const double& getX() const;
	const double& getY() const;
	double distance(const Point2D& other) const;
	double distanceSquared(const Point2D& other) const;
	double originDistance() const;
	double originDistanceSquared() const;
	bool operator==(const Point2D &other) const;
	bool operator<(const Point2D& other) const;
	Vector2D operator-(const Point2D& other) const;
	Point2D& operator=(const Point2D& other);
	std::string str () const;
	void serialize(std::ostream& out) const;
};

inline const double& Point2D::getX() const {
	return x;
}

inline const double& Point2D::getY() const {
	return y;
}

#endif
