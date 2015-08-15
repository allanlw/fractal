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
#include <string>
#include <sstream>
#include <iostream>

#include "rectangle.h"
#include "point2d.h"
#include "mathutils.h"

Rectangle::Rectangle(const double x, const double y, const double w,
		const double h) :
	x(x), y(y), w(w), h(h) {
}

double Rectangle::getX() const {
	return this->x;
}

double Rectangle::getY() const {
	return this->y;
}

double Rectangle::getWidth() const {
	return this->w;
}

double Rectangle::getHeight() const {
	return this->h;
}

double Rectangle::getArea() const {
	return this->h*this->w;
}

bool Rectangle::pointInside(const Point2D& point) const {
	double px = point.getX();
	double py = point.getY();
	return (((px >= this->x || doublesEqual(px, this->x)) &&
			 (px <= this->x+this->w || doublesEqual(px, this->x+this->w))) &&
			((py >= this->y || doublesEqual(py, this->y)) &&
			 (px <= this->y+this->h || doublesEqual(py, this->y+this->h))));
}

std::string Rectangle::str () const {
	std::ostringstream s;
	s << "(X:" << this->x << ",Y:" << this->y << ", W:" << this->w << ", H:" << this->h << ")";
	return s.str();
}

double Rectangle::getTop() const {
	return y;
}
double Rectangle::getLeft() const {
	return x;
}
double Rectangle::getBottom() const {
	return y+h;
}
double Rectangle::getRight() const {
	return x+w;
}
