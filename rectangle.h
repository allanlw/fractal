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
#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include <string>

#include "point2d.h"

class Rectangle {
private:
	double x;
	double y;
	double w;
	double h;
public:
	Rectangle(const double x, const double y, const double w, const double h);
	double getX() const;
	double getY() const;
	double getWidth() const;
	double getHeight() const;
	double getLeft() const;
	double getRight() const;
	double getTop() const;
	double getBottom() const;
	double getArea() const;
	bool pointInside(const Point2D& point) const;
	std::string str() const;
};
#endif
