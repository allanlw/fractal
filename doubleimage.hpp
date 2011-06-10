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
	gdImagePtr edges;
public:
	DoubleImage(gdImagePtr image);
	double snapXToGrid(double x) const;
	double snapYToGrid(double y) const;
	double getXInc() const;
	double getYInc() const;
	std::list<Point2D> getPointsInside(const Triangle* t) const;
	Point2D snapToGrid(const Point2D& point) const;
	double valueAt(double x, double y) const;
	double valueAt(const Point2D& point) const;
	TriFit getOptimalFit(const Triangle* smaller, const Triangle* larger, TriFit::PointMap pMap) const;
	void getInsideAndCorresponding(const Triangle* smaller, const Triangle* larger, TriFit::PointMap pMap, std::insert_iterator<std::list<double> > smallerInserter, std::insert_iterator<std::list<double> > largerInserter) const;
	static std::vector<Point2D> getCorners();
	TriFit getBestMatch(const Triangle* smaller, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end) const;
	void mapPoints(const Triangle* t, TriFit fit, gdImagePtr to) const;
	void setImage(gdImagePtr image);
	gdImagePtr getImage() const;
	~DoubleImage();

	static int doubleToIntC(double c);
	int doubleToIntX(double x) const;
	int doubleToIntY(double y) const;
	static int doubleToInt(double x, int min, int max);
};

#endif
