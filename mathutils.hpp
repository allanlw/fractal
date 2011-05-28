/*
 * mathutils.hpp
 *
 *  Created on: May 1, 2011
 *      Author: Allan
 */

#ifndef MATHUTILS_HPP_
#define MATHUTILS_HPP_

#include <vector>
#include <cmath>

const double PI = std::atan(1) * 4;
const double ZERO = 1E-10;

double sum(std::vector<double> nums);
double avg(std::vector<double> nums);
double sumSquares(std::vector<double> nums);
double productZip(std::vector<double> nums1, std::vector<double> nums2);
inline bool doublesEqual(double d1, double d2){
	return std::abs(d1-d2) < ZERO;
};

#endif /* MATHUTILS_HPP_ */
