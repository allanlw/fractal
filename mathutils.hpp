/*
 * mathutils.hpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

#include <list>
#include <cmath>

const double PI = std::atan(1) * 4;
const double ZERO = 1E-10;

double sum(std::list<double>::const_iterator start, std::list<double>::const_iterator end);
double avg(std::list<double>::const_iterator start, std::list<double>::const_iterator end);
double sumSquares(std::list<double>::const_iterator start, std::list<double>::const_iterator end);
double productZip(std::list<double>::const_iterator start1, std::list<double>::const_iterator end1,
                  std::list<double>::const_iterator start2, std::list<double>::const_iterator end2);
inline bool doublesEqual(double d1, double d2){
	return std::abs(d1-d2) < ZERO;
};
inline double signum(double d) {
	return (d>0) ? 1.0 : ((d<0) ? -1.0 : 0);
}

#endif /* MATHUTILS_HPP_ */
