#ifndef DOUBLEIMAGE_HPP_
#define DOUBLEIMAGE_HPP_

#include <list>
#include <vector>
#include <iterator>
#include "gd.h"

#include "triangle.hpp"
#include "trifit.hpp"
#include "point2d.hpp"

class DoubleImage {
private:
	gdImagePtr image;
public:
	DoubleImage(gdImagePtr image);
	double valueAt(double x, double y) const;
	double valueAt(const Point2D& point) const;
	TriFit getOptimalFit(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap);
	std::list<double> getPointsInside(Triangle* t);
	std::list<double> getCorrespondingPoints(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap);
	void getInsideAndCorresponding(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap, std::list<double>* smallerPoints, std::list<double>* largerPoints);
	std::vector<Point2D> getCorners();
	Triangle* getBestMatch(Triangle* smaller, TriFit* optimal, Triangle::PointMap* pMap, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end);
};

#endif
