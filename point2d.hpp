#ifndef _POINT2D_HPP
#define _POINT2D_HPP

#include <string>
#include <ostream>
#include <istream>

class Vector2D;

#include "vector2d.hpp"

class Point2D {
private:
	double x;
	double y;
public:
	Point2D(double x, double y);
	Point2D(const Point2D& other);
	Point2D(std::istream& in);
	inline double getX() const {
		return x;
	}
	inline double getY() const {
		return y;
	}
	double distance(const Point2D& other) const;
	double distanceSquared(const Point2D& other) const;
	bool operator==(const Point2D &other) const;
	Vector2D operator-(const Point2D& other) const;
	Point2D& operator=(const Point2D& other);
	std::string str () const;
	void serialize(std::ostream& out) const;
};
#endif
