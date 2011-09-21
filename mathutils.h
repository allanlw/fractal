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
#ifndef _MATHUTILS_H
#define _MATHUTILS_H

#include <cmath>
#include <ostream>
#include <istream>
#include <numeric>

#ifndef M_PI
#define M_PI (std::atan(1) * 4)
#endif

const double ZERO = 1E-10;

static inline double _sum (double x, double y) {
	return x + y;
}

static inline double _sumSquares(double x, double y) {
	return x + (y*y);
}

template<class InputIterator> double sum(InputIterator start, InputIterator end) {
	return std::accumulate<InputIterator, double>(start, end, 0, _sum);
}

template<class InputIterator> double avg(InputIterator start, InputIterator end) {
	return sum<InputIterator>(start, end)/distance(start, end);
}

template<class InputIterator> double sumSquares(InputIterator start, InputIterator end) {
	return std::accumulate<InputIterator, double>(start, end, 0, _sumSquares);
}

template<class InputIterator1, class InputIterator2> double dotProduct(
		InputIterator1 start1, InputIterator1 end1,
		InputIterator2 start2, InputIterator2 end2) {
	double result = 0;
	for (; start1 != end1 && start2 != end2; start1++, start2++) {
		result += (*start1) * (*start2);
	}
	return result;
}

static inline bool doublesEqual(double d1, double d2){
	return std::abs(d1-d2) < ZERO;
}

static inline double signum(double d) {
	return (d>0) ? 1.0 : ((d<0) ? -1.0 : 0);
}

static inline int doubleToInt(double d, int min, int max) {
	const int _x = (int)(min + round(d * max));
	if (_x <= min) {
		return min;
	} else if (_x >= max) {
		return max;
	} else {
		return _x;
	}
}

#endif
