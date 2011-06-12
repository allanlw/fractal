#ifndef TRIFIT_HPP_
#define TRIFIT_HPP_

#include <cstddef>
#include <string>
#include <ostream>
#include <istream>

class TriFit {
public:
	enum PointMap {
		P012, P021, P102, P120, P201, P210, P000
	};
	static const unsigned char NUM_MAPS = 6;

	double saturation;
	double brightness;
	double error;
	PointMap pMap;
	const class Triangle* best;

	TriFit(double saturation, double brightness, double error, PointMap pMap, const class Triangle* best);
	TriFit(const TriFit& other);
	TriFit(std::istream& in, unsigned short* t);
	explicit TriFit();

	TriFit& operator=(const TriFit& other);


	std::string str() const;
	void serialize(std::ostream& out) const;

	static PointMap pointMapFromInt(std::size_t pMap);
	static char pointMapToInt(PointMap pMap);
	static unsigned char getPoint0(PointMap pMap);
	static unsigned char getPoint1(PointMap pMap);
	static unsigned char getPoint2(PointMap pMap);
};

#include "triangle.hpp"

#endif
