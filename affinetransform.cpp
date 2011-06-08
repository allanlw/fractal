/*
 * affinetransform.cpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#include <vector>

#include "affinetransform.hpp"
#include "mathutils.hpp"
AffineTransform::AffineTransform(double m00, double m01, double m02,
		double m10, double m11, double m12) :
	m00(m00), m01(m01), m02(m02), m10(m10), m11(m11), m12(m12) {
}

AffineTransform::AffineTransform(const Triangle * source,
		const Triangle * target, Triangle::PointMap pointMap) {
	const std::vector<const Point2D*> sourcePoints = *(source->getPoints());
	const std::vector<const Point2D*> targetPoints = *(target->getPoints());
	double s_x[] = { sourcePoints[0]->getX(), sourcePoints[1]->getX(),
			sourcePoints[2]->getX() };
	double s_y[] = { sourcePoints[0]->getY(), sourcePoints[1]->getY(),
			sourcePoints[2]->getY() };

	double d_x[] = { targetPoints[Triangle::getPoint0(pointMap)]->getX(),
			targetPoints[Triangle::getPoint1(pointMap)]->getX(),
			targetPoints[Triangle::getPoint2(pointMap)]->getX() };
	double d_y[] = { targetPoints[Triangle::getPoint0(pointMap)]->getY(),
			targetPoints[Triangle::getPoint1(pointMap)]->getY(),
			targetPoints[Triangle::getPoint2(pointMap)]->getY() };

	double descriminant = 1.0 / (s_x[2] * (s_y[0] - s_y[1]) + s_x[0] * (s_y[1]
			- s_y[2]) + s_x[1] * (s_y[2] - s_y[0]));

	if (descriminant < ZERO) {
		throw InvalidTransformException();
	}

	double tm00 = (d_x[0] * (s_y[1] - s_y[2]) + d_x[1] * (s_y[2] - s_y[0])
			+ d_x[2] * (s_y[0] - s_y[1])) * descriminant;
	double tm01 = (d_x[0] * (s_x[2] - s_x[1]) + d_x[1] * (s_x[0] - s_x[2])
			+ d_x[2] * (s_x[1] - s_x[0])) * descriminant;
	double tm02 = (d_x[0] * (s_x[1] * s_y[2] - s_x[2] * s_y[1]) + d_x[1]
			* (s_x[2] * s_y[0] - s_x[0] * s_y[2]) + d_x[2] * (s_x[0] * s_y[1]
			- s_x[1] * s_y[0])) * descriminant;

	double tm10 = (d_y[0] * (s_y[1] - s_y[2]) + d_y[1] * (s_y[2] - s_y[0])
			+ d_y[2] * (s_y[0] - s_y[1])) * descriminant;
	double tm11 = (d_y[0] * (s_x[2] - s_x[1]) + d_y[1] * (s_x[0] - s_x[2])
			+ d_y[2] * (s_x[1] - s_x[0])) * descriminant;
	double tm12 = (d_y[0] * (s_x[1] * s_y[2] - s_x[2] * s_y[1]) + d_y[1]
			* (s_x[2] * s_y[0] - s_x[0] * s_y[2]) + d_y[2] * (s_x[0] * s_y[1]
			- s_x[1] * s_y[0])) * descriminant;

	AffineTransform(tm00, tm01, tm02, tm10, tm11, tm12);
}

Point2D AffineTransform::transform(const Point2D& point) const {
	double x = point.getX();
	double y = point.getY();

	return Point2D(x * m00 + y * m01 + m02, x * m10 + y * m11 + m12);
}

double AffineTransform::getDeterminant() const {
	return m00 * m11 - m01 * m10;
}

AffineTransform AffineTransform::getInverse() const {
	double det = this->getDeterminant();
	if (det < ZERO) {
		throw InvalidTransformException();
	}

	return AffineTransform(m11 / det, // m00
			-m10 / det, // m10
			-m01 / det, // m01
			m00 / det, // m11
			(m01 * m12 - m11 * m02) / det, // m02
			(m10 * m02 - m00 * m12) / det // m12
	);
}
