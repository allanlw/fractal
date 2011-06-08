#ifndef DOUBLEIMAGE_HPP_
#define DOUBLEIMAGE_HPP_

#include <list>
#include "gd.h"

#include "triangle.hpp"
#include "trifit.hpp"
#include "point2d.hpp"

class DoubleImage {
private:
	gdImagePtr image;
public:
	DoubleImage(gdImagePtr image);
	double valueAt(double x, double y);
	double valueAt(Point2D* point);
	TriFit getOptimalFit(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap);
	std::list<double> getPointsInside(Triangle* t);
	std::list<double> getCorrespondingPoints(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap);
	void getInsideAndCorresponding(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap, std::list<double>* smallerPoints, std::list<double>* largerPoints);
	std::list<Point2D*> getCorners();
	Triangle* getBestMatch(Triangle* smaller, TriFit* optimal, Triangle::PointMap* pMap, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end);
};

#endif
