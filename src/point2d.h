/*
 * Copyright (c) 2011 Allan Wirth <allanlw@gmail.com>
 *
 * This file is part of Fractal Image Compressor.
 *
 * Fractal Image Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Image Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Image Compressor.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _POINT2D_H
#define _POINT2D_H

#include <string>
#include <ostream>
#include <istream>

class Vector2D;

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
