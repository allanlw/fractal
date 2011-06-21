#ifndef DOUBLEIMAGE_HPP_
#define DOUBLEIMAGE_HPP_

#include <list>
#include <vector>
#include <map>
#include <iterator>
#include "gd.h"

#include "triangle.hpp"
#include "trifit.hpp"
#include "point2d.hpp"

class DoubleImage {
public:
	enum SamplingType {
		T_SUBSAMPLE,
		T_SUPERSAMPLE,
		T_BOTHSAMPLE
	};
private:
	gdImagePtr image;
	gdImagePtr edges;
	std::map<const Triangle*, std::vector<Point2D> > pointsCache;

	void mapPoint(gdImagePtr to, const TriFit& fit, const Point2D& source, const Point2D& dest);
public:
	DoubleImage(gdImagePtr image);
	DoubleImage(const DoubleImage& img);
	double snapXToGrid(double x) const;
	double snapYToGrid(double y) const;
	double floorXToGrid(double x) const;
	double floorYToGrid(double y) const;
	double ceilXToGrid(double x) const;
	double ceilYToGrid(double y) const;
	double getXInc() const;
	double getYInc() const;
	const std::vector<Point2D>& getPointsInside(const Triangle* t);
	std::vector<Point2D> getPointsOnLine(const Point2D& point1, const Point2D& point2) const;
	double valueAt(double x, double y) const;
	double valueAt(const Point2D& point) const;
	double edgeAt(double x, double y) const;
	double edgeAt(const Point2D& point) const;
	void generateEdges();
	TriFit getOptimalFit(const Triangle* smaller, const Triangle* larger);
	std::map<TriFit::PointMap, std::vector<double> > getAllConfigurations(const Triangle* smaller, const Triangle* larger);
	static std::vector<Point2D> getCorners();
	TriFit getBestMatch(const Triangle* smaller, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end);
	void mapPoints(const Triangle* t, TriFit fit, gdImagePtr to, SamplingType type);
	double getBestDivide(const Point2D& point1, const Point2D& point2, bool high=true) const;
	void setImage(gdImagePtr image);
	gdImagePtr getImage() const;
	gdImagePtr getEdges() const;
	~DoubleImage();

	int doubleToIntX(double x) const;
	int doubleToIntY(double y) const;
};

#endif
