#ifndef _POINT2D_HPP
#define _POINT2D_HPP

#include <string>

class Point2D {
private:
	double x;
	double y;
public:
	Point2D(double x, double y);
	double getX() const;
	double getY() const;
	double distance(const Point2D& other) const;
	double distanceSquared(const Point2D& other) const;
	bool operator==(const Point2D &other) const;
	std::string str () const;
};
#endif
