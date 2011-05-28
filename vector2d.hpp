/*
 * vector2d.hpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#ifndef VECTOR2D_HPP_
#define VECTOR2D_HPP_

#include <string>
#include "point2d.hpp"

class Vector2D {
private:
	double magnitude;
	double angle;

	void normalize();
public:
	Vector2D(const double magnitude, const double angle);
	Vector2D(const Point2D* const origin, const Point2D* const point2d);
	double crossProduct(const Vector2D* const other) const;
	double angleBetween(const Vector2D* const other) const;
	double getAngle() const;
	double getMagnitude() const;
	Vector2D* getOpposite() const;
	bool operator==(const Vector2D &other) const;
	std::string str () const;
};

#endif /* VECTOR2D_HPP_ */
