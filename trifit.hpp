#ifndef TRIFIT_HPP_
#define TRIFIT_HPP_

#include <cstddef>

class TriFit {
public:
	enum PointMap {
		P012, P021, P102, P120, P201, P210
	};
	static const unsigned char NUM_MAPS = 6;
private:
	double saturation;
	double brightness;
	double error;
	PointMap pMap;
	class Triangle* best;
public:
	TriFit(double saturation, double brightness, double error, PointMap pMap, class Triangle* best);
	TriFit(const TriFit& other);
	explicit TriFit();

	TriFit& operator=(const TriFit& other);

	double getError() const;

	static PointMap pointMapFromInt(std::size_t pMap);
	static unsigned char getPoint0(PointMap pMap);
	static unsigned char getPoint1(PointMap pMap);
	static unsigned char getPoint2(PointMap pMap);
};

#include "triangle.hpp"

#endif
