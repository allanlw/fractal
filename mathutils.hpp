#ifndef _MATHUTILS_HPP
#define _MATHUTILS_HPP

#include <cmath>
#include <ostream>
#include <istream>
#include <numeric>

const double PI = std::atan(1) * 4;
const double ZERO = 1E-10;

inline double _sum (double x, double y) {
	return x + y;
}

inline double _sumSquares(double x, double y) {
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

inline bool doublesEqual(double d1, double d2){
	return std::abs(d1-d2) < ZERO;
}

inline double signum(double d) {
	return (d>0) ? 1.0 : ((d<0) ? -1.0 : 0);
}

int doubleToInt(double d, int min, int max);

inline double predictError(double area) {
// This is just a heuristic I came up with in excel.
	return (4E07)*pow(area, 2.1398);
}

void serializeDouble(std::ostream& out, double d);
void serializeUnsignedShort(std::ostream& out, unsigned short s);

unsigned short unserializeUnsignedShort(std::istream& in);
double unserializeDouble(std::istream& in);

#endif /* MATHUTILS_HPP_ */
