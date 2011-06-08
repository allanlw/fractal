/*
 * affinetransform.hpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#ifndef AFFINETRANSFORM_HPP_
#define AFFINETRANSFORM_HPP_

class AffineTransform;

#include <stdexcept>
#include "triangle.hpp"
#include "trifit.hpp"

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
	AffineTransform(const Triangle* source, const Triangle* dest,
			TriFit::PointMap pointMap);
	Point2D transform(const Point2D& point) const;
	AffineTransform getInverse() const;
	double getDeterminant() const;
};

#endif /* AFFINETRANSFORM_HPP_ */
