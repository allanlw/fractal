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
