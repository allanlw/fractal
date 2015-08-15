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
#include <sstream>
#include <stdexcept>

#include "trifit.h"
#include "mathutils.h"
#include "ioutils.h"

using namespace std;

TriFit::TriFit(double saturation, double brightness, double error, TriFit::PointMap pMap, const Triangle* best) :
saturation(saturation), brightness(brightness), error(error), pMap(pMap), best(best) { }

TriFit::TriFit(const TriFit& other) : saturation(other.saturation), brightness(other.brightness), error(other.error),
pMap(other.pMap), best(other.best) { }

TriFit::TriFit() : saturation(0), brightness(0), error(-1), pMap(P000), best(NULL) { }

char TriFit::pointMapToInt(TriFit::PointMap pMap) {
	switch(pMap) {
	case P012:
		return 0;
		break;
	case P021:
		return 1;
		break;
	case P102:
		return 2;
		break;
	case P120:
		return 3;
		break;
	case P201:
		return 4;
		break;
	case P210:
		return 5;
		break;
	default:
	case P000:
		return 6;
		break;
	}
}

TriFit::PointMap TriFit::pointMapFromInt(std::size_t pMap) {
	switch(pMap) {
	case 0:
		return P012;
		break;
	case 1:
		return P021;
		break;
	case 2:
		return P102;
		break;
	case 3:
		return P120;
		break;
	case 4:
		return P201;
		break;
	case 5:
		return P210;
		break;
	default:
	case 6:
		return P000;
		break;
	}
}

unsigned char TriFit::getPoint0(PointMap pointMap) {
	switch (pointMap) {
	case P012:
	case P021:
		return 0;
		break;
	case P102:
	case P120:
		return 1;
		break;
	case P201:
	case P210:
		return 2;
		break;
	default:
	case P000:
		return -1;
	}
}

unsigned char TriFit::getPoint1(PointMap pointMap) {
	switch (pointMap) {
	case P102:
	case P201:
		return 0;
		break;
	case P012:
	case P210:
		return 1;
		break;
	case P021:
	case P120:
		return 2;
		break;
	default:
	case P000:
		return -1;
	}
}

unsigned char TriFit::getPoint2(PointMap pointMap) {
	switch (pointMap) {
	case P120:
	case P210:
		return 0;
		break;
	case P201:
	case P021:
		return 1;
		break;
	case P102:
	case P012:
		return 2;
		break;
	default:
	case P000:
		return -1;
	}
}

TriFit& TriFit::operator=(const TriFit& other) {
	if (this != &other) {
		saturation = other.saturation;
		brightness = other.brightness;
		error = other.error;
		best = other.best;
		pMap = other.pMap;
	}
	return *this;
}

string TriFit::str() const {
	ostringstream st;
	st << "[s:" << saturation << ",o:" << brightness;
	st << ",r:" << error << ",p:";
	switch (pMap) {
	case P012:
		st << "012";
		break;
	case P021:
		st << "021";
		break;
	case P102:
		st << "102";
		break;
	case P120:
		st << "120";
		break;
	case P201:
		st << "201";
		break;
	case P210:
		st << "210";
		break;
	case P000:
		st << "000";
		break;
	}
	st << ",t" << best->str() << "]";
	return st.str();
}

void TriFit::serialize(ostream& out) const {
	serializeFraction(out, saturation, 0, 1);
	serializeFraction(out, brightness, -255, +255);
	serializeFraction(out, error, 0, 255);
	out.put(pointMapToInt(pMap));
	if (best != NULL) {
		best->serializeID(out);
	} else {
		serializeUnsignedShort(out, 0xFFFF);
	}
}


TriFit::TriFit(istream& in, unsigned short* t) {
	saturation = unserializeFraction(in, 0, 1);
	brightness = unserializeFraction(in, -255, +255);
	error = unserializeFraction(in, 0, +255);
	pMap = pointMapFromInt(in.get());
	*t = unserializeUnsignedShort(in);
	best = NULL;
}
