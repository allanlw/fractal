#ifndef TRIFIT_HPP_
#define TRIFIT_HPP_

#include <cstddef>

struct TriFit {
public:
	enum PointMap {
		P012, P021, P102, P120, P201, P210
	};
	static const unsigned char NUM_MAPS = 6;

	double saturation;
	double brightness;
	double error;
	PointMap pMap;
	class Triangle* best;

	TriFit(double saturation, double brightness, double error, PointMap pMap, class Triangle* best) : saturation(saturation), brightness(brightness), error(error), pMap(pMap), best(best) { };
	TriFit() : saturation(0), brightness(0), error(0), pMap(P012), best(NULL) { };

	static PointMap pointMapFromInt(std::size_t pMap);
	static unsigned char getPoint0(PointMap pMap);
	static unsigned char getPoint1(PointMap pMap);
	static unsigned char getPoint2(PointMap pMap);
};

#include "triangle.hpp"

#endif
