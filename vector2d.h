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
