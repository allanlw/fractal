#ifndef _DOUBLEIMAGE_H
#define _DOUBLEIMAGE_H

#include <list>
#include <vector>
#include <map>
#include <iterator>
#include "gd.h"

#include "triangle.h"
#include "trifit.h"
#include "point2d.h"
#include "imageutils.h"

class DoubleImage {
public:
	enum SamplingType {
		T_SUBSAMPLE,
		T_SUPERSAMPLE,
		T_BOTHSAMPLE
	};
	enum DivisionType {
		T_HIGHENTROPY,
		T_LOWENTROPY,
		T_MIDDLE
	};
	enum Metric {
		M_RMS,
		M_SUP
	};
	enum EdgeDetectionMethod {
		M_SOBEL,
		M_LAPLACE
	};
private:
	gdImagePtr image;
	std::map<Channel, gdImagePtr> edges;
	std::map<const Triangle*, std::vector<Point2D> > pointsCache;
	SamplingType sType;
	DivisionType dType;
	Metric metric;
	EdgeDetectionMethod edMethod;

	void mapPoint(gdImagePtr to, const TriFit& fit, const Point2D& source, const Point2D& dest, Channel channel);
	static void copyImage(gdImagePtr* to, gdImagePtr from);
public:
	DoubleImage();
	DoubleImage(gdImagePtr image);
	DoubleImage(gdImagePtr image, SamplingType sType, DivisionType dType, Metric metric, EdgeDetectionMethod edMethod);
	DoubleImage(const DoubleImage& img);
	DoubleImage& operator=(const DoubleImage& img);
	~DoubleImage();

	int getWidth() const;
	int getHeight() const;
	Metric getMetric() const;
	void setMetric(Metric metric);
	SamplingType getSamplingType() const;
	void setSamplingType(SamplingType sType);
	DivisionType getDivisionType() const;
	void setDivisionType(DivisionType dType);
	EdgeDetectionMethod getEdgeDetectionMethod() const;
	void setEdgeDetectionMethod(EdgeDetectionMethod edMethod);
	bool hasEdges() const;
	void setImage(gdImagePtr image);
	gdImagePtr getImage() const;
	void generateEdges();
	const std::map<Channel, gdImagePtr>& getEdges() const;

	double snapXToGrid(double x) const;
	double snapYToGrid(double y) const;
	double floorXToGrid(double x) const;
	double floorYToGrid(double y) const;
	double ceilXToGrid(double x) const;
	double ceilYToGrid(double y) const;
	double getXInc() const;
	double getYInc() const;
	int doubleToIntX(double x) const;
	int doubleToIntY(double y) const;

	double valueAt(double x, double y, Channel channel) const;
	double valueAt(const Point2D& point, Channel channel) const;
	double edgeAt(double x, double y, Channel channel) const;
	double edgeAt(const Point2D& point, Channel channel) const;

	const std::vector<Point2D>& getPointsInside(const Triangle* t);
	std::vector<Point2D> getPointsOnLine(const Point2D& point1, const Point2D& point2) const;
	TriFit getOptimalFit(const Triangle* smaller, const Triangle* larger, Channel channel);
	std::map<TriFit::PointMap, std::vector<double> > getAllConfigurations(const Triangle* smaller, const Triangle* larger, Channel channel);
	TriFit getBestMatch(const Triangle* smaller, std::list<Triangle*>::const_iterator start, std::list<Triangle*>::const_iterator end, Channel channel);
	double getBestDivide(const Point2D& point1, const Point2D& point2, Channel channel) const;
	void mapPoints(const Triangle* t, TriFit fit, gdImagePtr to, Channel channel);

	static std::vector<Point2D> getCorners();
};

#endif
