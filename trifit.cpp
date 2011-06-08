#include "trifit.hpp"

using namespace std;

TriFit::PointMap TriFit::pointMapFromInt(std::size_t pMap) {
	switch(pMap) {
	default:
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
		return -1;
	}
}
