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
#include <vector>
#include <cmath>
#include <sstream>

#include "affinetransform.h"
#include "mathutils.h"

using namespace std;

AffineTransform::AffineTransform(double m00, double m01, double m02,
		double m10, double m11, double m12) :
	m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12) {
}

AffineTransform::AffineTransform(const Triangle& source,
		const Triangle& target, TriFit::PointMap pointMap) {

	const std::vector<Point2D>& sourcePoints = source.getPoints();
	const std::vector<Point2D>& targetPoints = target.getPoints();

	double s_x[] = { sourcePoints[0].getX(), sourcePoints[1].getX(),
			sourcePoints[2].getX() };
	double s_y[] = { sourcePoints[0].getY(), sourcePoints[1].getY(),
			sourcePoints[2].getY() };

	double d_x[] = { targetPoints[TriFit::getPoint0(pointMap)].getX(),
			targetPoints[TriFit::getPoint1(pointMap)].getX(),
			targetPoints[TriFit::getPoint2(pointMap)].getX() };
	double d_y[] = { targetPoints[TriFit::getPoint0(pointMap)].getY(),
			targetPoints[TriFit::getPoint1(pointMap)].getY(),
			targetPoints[TriFit::getPoint2(pointMap)].getY() };

	double descriminant = 1.0 / (s_x[2] * (s_y[0] - s_y[1]) + s_x[0] * (s_y[1]
			- s_y[2]) + s_x[1] * (s_y[2] - s_y[0]));

	if (abs(descriminant) < ZERO) {
		throw InvalidTransformException();
	}

	m00 = (d_x[0] * (s_y[1] - s_y[2]) + d_x[1] * (s_y[2] - s_y[0])
			+ d_x[2] * (s_y[0] - s_y[1])) * descriminant;
	m01 = (d_x[0] * (s_x[2] - s_x[1]) + d_x[1] * (s_x[0] - s_x[2])
			+ d_x[2] * (s_x[1] - s_x[0])) * descriminant;
	m02 = (d_x[0] * (s_x[1] * s_y[2] - s_x[2] * s_y[1]) + d_x[1]
			* (s_x[2] * s_y[0] - s_x[0] * s_y[2]) + d_x[2] * (s_x[0] * s_y[1]
			- s_x[1] * s_y[0])) * descriminant;

	m10 = (d_y[0] * (s_y[1] - s_y[2]) + d_y[1] * (s_y[2] - s_y[0])
			+ d_y[2] * (s_y[0] - s_y[1])) * descriminant;
	m11 = (d_y[0] * (s_x[2] - s_x[1]) + d_y[1] * (s_x[0] - s_x[2])
			+ d_y[2] * (s_x[1] - s_x[0])) * descriminant;
	m12 = (d_y[0] * (s_x[1] * s_y[2] - s_x[2] * s_y[1]) + d_y[1]
			* (s_x[2] * s_y[0] - s_x[0] * s_y[2]) + d_y[2] * (s_x[0] * s_y[1]
			- s_x[1] * s_y[0])) * descriminant;
}

Point2D AffineTransform::transform(const Point2D& point) const {
	const double& x = point.getX();
	const double& y = point.getY();

	return Point2D(x * m00 + y * m01 + m02, x * m10 + y * m11 + m12);
}

Point2D AffineTransform::inverseTransform(const Point2D& point) const {
	double det = this->getDeterminant();
	if (abs(det) < ZERO) {
		throw InvalidTransformException();
	}

	double x = point.getX() - m02;
	double y = point.getY() - m12;

	return Point2D((x * m11 - y * m01)/det, (y * m00 - x * m10)/det);
}

double AffineTransform::getDeterminant() const {
	return m00 * m11 - m01 * m10;
}

AffineTransform AffineTransform::getInverse() const {
	double det = this->getDeterminant();
	if (abs(det) < ZERO) {
		throw InvalidTransformException();
	}

	return AffineTransform(m11 / det, // m00
			-m01 / det, // m01
			(m01 * m12 - m11 * m02) / det, // m02
			-m10 / det, // m10
			m00 / det, // m11
			(m10 * m02 - m00 * m12) / det // m12
	);
}

string AffineTransform::str() const{
	ostringstream s;
	s << "[[" << m00 << "," << m01;
	s << "," << m02 << "],[" << m10;
	s << "," << m11 << "," << m12 << "]]";
	return s.str();
}
