/*
 * affinetransform.hpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#ifndef AFFINETRANSFORM_HPP_
#define AFFINETRANSFORM_HPP_

class AffineTransform;

#include <exception>
#include "triangle.hpp"

class AffineTransform {
private:
	double m00;
	double m01;
	double m02;
	double m10;
	double m11;
	double m12;
public:
	class InvalidTransformException: public std::exception {
		virtual const char* what() const throw () {
			return "AffineTransform was not invertible";
		}
	};
	AffineTransform(double m00, double m01, double m02, double m10, double m11,
			double m12);
	AffineTransform(const Triangle* source, const Triangle* dest,
			Triangle::PointMap pointMap) throw (InvalidTransformException);
	Point2D* transform(const Point2D* point) const;
	AffineTransform* getInverse() const throw (InvalidTransformException);
	double getDeterminant() const;
};

#endif /* AFFINETRANSFORM_HPP_ */
