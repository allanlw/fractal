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
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <iostream>

#include "mathutils.h"
#include "point2d.h"
#include "ioutils.h"

using namespace std;

Point2D::Point2D(double x, double y) :
	x(x), y(y) {
}

Point2D::Point2D(const Point2D& other) {
	x = other.x;
	y = other.y;
}

double Point2D::distance(const Point2D& other) const {
	return sqrt(distanceSquared(other));
}

double Point2D::distanceSquared(const Point2D& other) const {
	const double xdist = (other.x-x);
	const double ydist = (other.y-y);
	return xdist*xdist+ydist*ydist;
}

double Point2D::originDistance() const {
	return sqrt(originDistanceSquared());
}

double Point2D::originDistanceSquared() const {
	return x*x + y*y;
}

bool Point2D::operator==(const Point2D &other) const {
	return doublesEqual(x, other.x) && doublesEqual(y, other.y);
}

bool Point2D::operator<(const Point2D& other) const {
	return originDistanceSquared() < other.originDistanceSquared();
}

std::string Point2D::str () const {
	std::ostringstream s;
	s << "(" << x << ", " << y << ")";
	return s.str();
}

Vector2D Point2D::operator-(const Point2D& other) const{
	return Vector2D(other, *this);
}

Point2D& Point2D::operator=(const Point2D& other) {
	x = other.x;
	y = other.y;
	return *this;
}

void Point2D::serialize(ostream& out) const {
	serializeFraction(out, x, 0, 1);
	serializeFraction(out, y, 0, 1);
}

Point2D::Point2D(istream& in) {
	x = unserializeFraction(in, 0, 1);
	y = unserializeFraction(in, 0, 1);
}
