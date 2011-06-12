#include <sstream>
#include <stdexcept>

#include "trifit.hpp"
#include "mathutils.hpp"

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
	out.put('F');
	serializeDouble(out, saturation);
	serializeDouble(out, brightness);
	serializeDouble(out, error);
	out.put(pointMapToInt(pMap));
	if (best != NULL) {
		best->serializeID(out);
	} else {
		serializeUnsignedShort(out, 0xFFFF);
	}
}


TriFit::TriFit(istream& in, unsigned short* t) {
	if (!(in.get() == 'F')) {
		throw logic_error("Malformed TriFit");
	}
	saturation = unserializeDouble(in);
	brightness = unserializeDouble(in);
	error = unserializeDouble(in);
	pMap = pointMapFromInt(in.get());
	*t = unserializeUnsignedShort(in);
	best = NULL;
}
