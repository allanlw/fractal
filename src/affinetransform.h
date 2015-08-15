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
#ifndef _AFFINETRANSFORM_H
#define _AFFINETRANSFORM_H

class AffineTransform;

#include <stdexcept>
#include <string>

#include "triangle.h"
#include "trifit.h"

class AffineTransform {
private:
	double m00;
	double m01;
	double m02;
	double m10;
	double m11;
	double m12;
public:
	class InvalidTransformException: public std::logic_error {
	public:
		InvalidTransformException() : std::logic_error("AffineTransform was not invertible") {
		}
	};
	AffineTransform(double m00, double m01, double m02, double m10, double m11,
			double m12);
	AffineTransform(const Triangle& source, const Triangle& dest,
			TriFit::PointMap pointMap);
	Point2D transform(const Point2D& point) const;
	Point2D inverseTransform(const Point2D& point) const;
	AffineTransform getInverse() const;
	double getDeterminant() const;
	std::string str() const;
};

#endif
