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
	double snapXToGrid(double x) const;
	double snapYToGrid(double y) const;
	Point2D snapToGrid(const Point2D& point) const;
	double valueAt(double x, double y) const;
	double valueAt(const Point2D& point) const;
	TriFit getOptimalFit(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap);
/*
	std::list<double> getPointsInside(Triangle* t);
	std::list<double> getCorrespondingPoints(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap);
*/
	void getInsideAndCorresponding(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap, std::insert_iterator<std::list<double> > smallerInserter, std::insert_iterator<std::list<double> > largerInserter);
	std::vector<Point2D> getCorners();
	TriFit getBestMatch(Triangle* smaller, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end);
};

#endif
