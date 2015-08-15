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
#ifndef _TRIFIT_H
#define _TRIFIT_H

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

#include "triangle.h"

#endif
