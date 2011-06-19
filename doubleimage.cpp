#include "doubleimage.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <cstdio>
#include <algorithm>

#include "mathutils.hpp"
#include "constant.hpp"
#include "imageutils.hpp"
#include "output.hpp"

using namespace std;

DoubleImage::DoubleImage(gdImagePtr image)  {
	this->image = gdImageCreateTrueColor(gdImageSX(image), gdImageSY(image));
	gdImageCopy(this->image, image, 0, 0, 0, 0, gdImageSX(image), gdImageSY(image));
	this->edges = NULL;
}

DoubleImage::DoubleImage(const DoubleImage& img) {
	this->image = gdImageCreateTrueColor(gdImageSX(img.image), gdImageSY(img.image));
	gdImageCopy(this->image, img.image, 0, 0, 0, 0, gdImageSX(this->image), gdImageSY(this->image));
	if (img.edges != NULL) {
		this->edges = gdImageCreateTrueColor(gdImageSX(img.edges), gdImageSY(img.edges));
		gdImageCopy(this->edges, img.edges, 0, 0, 0, 0, gdImageSX(this->edges), gdImageSY(this->edges));
	} else {
		this->edges = NULL;
	}
}

DoubleImage::~DoubleImage() {
	gdFree(image);
	if (this->edges != NULL) {
		gdFree(edges);
	}
}

void DoubleImage::generateEdges() {
	if (EDGE_DETECT_SOBEL) {
		edges = edgeDetectSobel(image);
	} else {
		edges = edgeDetectLaplace(image);
	}
}

double DoubleImage::snapXToGrid(double x) const {
	return round ( x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::snapYToGrid(double y) const {
	return round (y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

double DoubleImage::floorXToGrid(double x) const {
	return floor (x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::floorYToGrid(double y) const {
	return floor (y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

double DoubleImage::ceilXToGrid(double x) const {
	return ceil( x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::ceilYToGrid(double y) const {
	return ceil( y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

int DoubleImage::doubleToInt(double x, int min, int max) {
	const int _x = (int) round (x * max);
	if (_x < min) {
		return min;
	} else if (_x >= max) {
		return max;
	} else {
		return _x;
	}
}

int DoubleImage::doubleToIntX(double x) const {
	return doubleToInt(x, 0, gdImageSX(image)-1);
}

int DoubleImage::doubleToIntY(double y) const {
	return doubleToInt(y, 0, gdImageSY(image)-1);
}

double DoubleImage::valueAt(double x, double y) const {
	const int _x = doubleToIntX(x);
	const int _y = doubleToIntY(y);

	const int val = getPixel(image, _x, _y, false);
	return val;
}

double DoubleImage::edgeAt(double x, double y) const {
	const int _x = doubleToIntX(x);
	const int _y = doubleToIntY(y);

	const int val = getPixel(edges, _x, _y, false);
	return val;
}

double DoubleImage::edgeAt(const Point2D& point) const {
	return edgeAt(point.getX(), point.getY());
}

double DoubleImage::valueAt(const Point2D& point) const {
	return valueAt(point.getX(), point.getY());
}

TriFit DoubleImage::getOptimalFit(const Triangle* smaller, const Triangle* larger) {

	map<TriFit::PointMap, vector<double> > allConfigs = getAllConfigurations(smaller, larger);
	TriFit best(0, 0, -1, TriFit::P000, larger);

	const TriFit::PointMap tm = TriFit::P000;

	const vector<double>& largerPoints = allConfigs[tm];

	double domainSum = sum(largerPoints.begin(), largerPoints.end());
	double domainSquaresSum = sumSquares(largerPoints.begin(), largerPoints.end());
	double n = largerPoints.size();

	double denom = ((n * n * domainSquaresSum) - (domainSum * domainSum));

	for (map<TriFit::PointMap, vector<double> >::const_iterator it = allConfigs.begin(); it != allConfigs.end(); it++) {
		if (it->first == TriFit::P000) {
			continue;
		}
		double rangeSum = sum(it->second.begin(), it->second.end());
		double rangeSquaresSum = sumSquares(it->second.begin(), it->second.end());
		double productSum = dotProduct(largerPoints.begin(), largerPoints.end(), it->second.begin(), it->second.end());

		double s;
		double o;
//		if (doublesEqual(denom, 0.0)) {
//			s = 0;
//			o = rangeSum / (n*n);
//		} else {
			s = ((n * n * productSum) - (domainSum * rangeSum)) / denom;

/*			if (s > 1) {
				s = 1;
			} else if (s < 0) {
				s = 0;
			}
*/
//			o = (rangeSum - (s * domainSum)) / (n * n);
//			o = ((domainSum * domainSum) - (rangeSum * rangeSum))/ denom;
			o = (rangeSum - (s*domainSum)) / n;
//		}

		double r = ((rangeSquaresSum + (s * ((s * domainSquaresSum) - (2.0 *
				productSum) + (2.0 * o * domainSum))) + (o * ((o * n
				* n) - (2.0 * rangeSum))))) / n;

/*
		double r = 0;
		for (size_t j = 0; j < allConfigs[tm].size(); j ++) {
			double t = (s * allConfigs[m][j] + o - allConfigs[tm][j]);
			r += t * t;
		}
		r /= n;
*/
		if (r < best.error || best.error == -1) {
			best.saturation = s;
			best.brightness = o;
			best.error = r;
			best.pMap = it->first;
		}

	}
	return best;
}

double DoubleImage::getYInc() const {
	return 1.0 / ((double)gdImageSY(image)-1);
}

double DoubleImage::getXInc() const {
	return 1.0 / ((double)gdImageSX(image)-1);
}

const vector<Point2D>& DoubleImage::getPointsInside(const Triangle* t) {

	map<const Triangle*, vector<Point2D> >::const_iterator it = pointsCache.find(t);

	if (it != pointsCache.end()) {
		return it->second;
	}

	pointsCache[t] = vector<Point2D>(t->getPoints().begin(), t->getPoints().end());
	vector<Point2D>& result = pointsCache[t];

	const Rectangle bounds = t->getBoundingBox();

	double xMax = ceilXToGrid(bounds.getRight());
	double yMax = ceilYToGrid(bounds.getBottom());
	double xInc = getXInc();
	double yInc = getYInc();

	for (double x = floorXToGrid(bounds.getLeft()); x <= xMax; x += xInc) {
		Point2D top(-1,-1);
		bool topSet = false;
		for (double y = floorYToGrid(bounds.getTop()); y <= yMax; y += yInc) {
			Point2D point(x,y);
			if (t->pointInside(point)) {
				top = point;
				topSet = true;
				break;
			}
		}
		if (!topSet) {
			continue;
		}
		Point2D bottom(-1,-1);
		bool bottomSet = false;
		double limit = top.getY();
		for (double y = yMax; y > limit; y -= yInc) {
			Point2D point(x,y);
			if (t->pointInside(point)) {
				bottom = point;
				bottomSet = true;
				break;
			}
		}
		if (!bottomSet || top == bottom) {
			result.push_back(top);
		} else {
			for (double y = bottom.getY(); y >= limit; y -= yInc) {
				result.push_back(Point2D(x,y));
			}
		}
	}

	const pair<const Point2D&, const Point2D&> pointPairs[3] = {
		pair<const Point2D&, const Point2D&>(t->getPoints()[0], t->getPoints()[1]),
		pair<const Point2D&, const Point2D&>(t->getPoints()[0], t->getPoints()[2]),
		pair<const Point2D&, const Point2D&>(t->getPoints()[1], t->getPoints()[2])
	};

	for (unsigned char i = 0; i < 3; i++) {
		const vector<Point2D> edgePoints = getPointsOnLine(pointPairs[i].first, pointPairs[i].second);

		result.insert(result.end(), edgePoints.begin(), edgePoints.end());
	}

	return result;
}

std::vector<Point2D> DoubleImage::getCorners() {
	vector<Point2D> result;
	result.push_back(Point2D(0.,0.));
	result.push_back(Point2D(1.,0.));
	result.push_back(Point2D(1.,1.));
	result.push_back(Point2D(0.,1.));
	return result;
}

TriFit DoubleImage::getBestMatch(const Triangle* smaller, list<Triangle*>::const_iterator start, list<Triangle*>::const_iterator end) {
	TriFit result(0, 0, -1, TriFit::P000, NULL);
#if USE_HEURISTICS == 1
	const size_t minArea = getPointsInside(smaller).size() * MIN_SEARCH_RATIO;
#endif
	for(; start != end; start++) {
#if USE_HEURISTICS == 1
		if (getPointsInside(*start).size() < minArea) {
			continue;
		}
#endif
		TriFit f = getOptimalFit(smaller,*start);
		if (f.error < result.error || result.error < 0) {
			result = f;
		}
	}
	if (outputDebug() && result.best != NULL) {
		output << " - ratio S:L " << (smaller->getArea() / result.best->getArea())<< endl;
	}
	return result;
}

void DoubleImage::setImage(gdImagePtr image) {
	this->image = image;
}

gdImagePtr DoubleImage::getImage() const {
	return image;
}

gdImagePtr DoubleImage::getEdges() const {
	return edges;
}

void DoubleImage::mapPoints(const Triangle* t, TriFit fit, gdImagePtr to) {
	if (gdImageSX(image) != gdImageSX(to) || gdImageSY(image) != gdImageSY(to)) {
		throw logic_error("dimensions don't match!!!");
	}

	const vector<Point2D>& targetPoints = getPointsInside(t);

	const AffineTransform trans = AffineTransform(*t, *fit.best, fit.pMap);

	for (vector<Point2D>::const_iterator it = targetPoints.begin(); it != targetPoints.end(); it++) {
		mapPoint(to, fit, trans.transform(*it), *it);
	}

}

void DoubleImage::mapPoint(gdImagePtr to, const TriFit& fit, const Point2D& source, const Point2D& dest) {
	const int d_x = doubleToIntX(dest.getX());
	const int d_y = doubleToIntY(dest.getY());

	double newVal = (valueAt(source) * fit.saturation) + fit.brightness;

	const int d_a = gdImageAlpha(to, gdImageGetPixel(to, d_x, d_y));
	const int newAlpha = d_a+1;

	if (newAlpha > 1) {
		const double oldVal = getPixel(to, d_x, d_y, false);
		newVal = ((oldVal*d_a)+newVal)/(newAlpha);
	}

	const int newColor = boundColor(round(newVal));

	const int c = gdTrueColorAlpha(newColor, newColor, newColor, (newAlpha>=gdAlphaTransparent)?gdAlphaTransparent:newAlpha);

	gdImageSetPixel(to, d_x, d_y, c);
}

map<TriFit::PointMap, vector<double> > DoubleImage::getAllConfigurations(const Triangle* smaller, const Triangle* larger) {
	const vector<Point2D>& largerPoints = getPointsInside(larger);

	map<TriFit::PointMap, vector<double> > result;

	for (char i = 0; i < TriFit::NUM_MAPS; i++) {
		const TriFit::PointMap m = TriFit::pointMapFromInt(i);

		result[m] = vector<double>(0);
		vector<double>& v = result[m];

		v.reserve(largerPoints.size());

		const AffineTransform trans(*larger, *smaller, m);
		for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
			v.push_back(valueAt(trans.transform(*it)));
		}
	}

	result[TriFit::P000] = vector<double>(0);
	vector<double>& v = result[TriFit::P000];

	v.reserve(largerPoints.size());

	for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
		v.push_back(valueAt(*it));
	}

	return result;
}

double DoubleImage::getBestDivide(const Point2D& first, const Point2D& second, bool high) const {

	if (this->edges == NULL) {
		throw logic_error("Edges not generated!");
	}

	double initialXDiff = second.getX() - first.getX();
	double initialYDiff = second.getY() - first.getY();

	Point2D point1(first.getX() + MIN_SUBDIVIDE_RATIO*(initialXDiff),
	               first.getY() + MIN_SUBDIVIDE_RATIO*(initialYDiff));
	Point2D point2(first.getX() + (1-MIN_SUBDIVIDE_RATIO)*(initialXDiff),
	               first.getY() + (1-MIN_SUBDIVIDE_RATIO)*(initialYDiff));

	vector<Point2D> points = getPointsOnLine(point1, point2);

	double bestVal = -1;
	double bestR = -1;

	for (vector<Point2D>::const_iterator it = points.begin(); it != points.end(); it++) {
		const double val = edgeAt(*it);
		if ( ((high)?(bestVal < val):(bestVal > val)) || bestR < 0) {
			const double rx = (it->getX() - first.getX())/initialXDiff;
			const double ry = (it->getY() - first.getY())/initialYDiff;
			if (doublesEqual(initialXDiff, 0)) {
				bestR = ry;
			} else if (doublesEqual(initialYDiff, 0)) {
				bestR = rx;
			} else {
				bestR = (rx+ry)/2.;
			}
			bestVal = val;
		}
	}

	return bestR;
}

vector<Point2D> DoubleImage::getPointsOnLine(const Point2D& point1, const Point2D& point2) const {

	vector<Point2D> result;

	double xDiff = point2.getX() - point1.getX();
	double yDiff = point2.getY() - point1.getY();

	if (abs(xDiff) > abs(yDiff)) {
		double xStep = getXInc();

		if (xDiff < 0) {
			xStep *= -1;
		}

		double xMin = (point1.getX() < point2.getX())?floorXToGrid(point1.getX()):ceilXToGrid(point1.getX());
		double xMax = (point2.getX() < point1.getX())?floorXToGrid(point2.getX()):ceilXToGrid(point2.getX());

		for (double x = xMin; (xDiff < 0)?(x > xMax):(x < xMax); x += xStep) {
			double r = (x - point1.getX())/xDiff;
			double y = snapYToGrid( r * yDiff + point1.getY());
			result.push_back(Point2D(x,y));
		}
	} else {
		double yStep = getYInc();

		if (yDiff < 0) {
			yStep *= -1;
		}

		double yMin = (point1.getY() < point2.getY())?floorYToGrid(point1.getY()):ceilYToGrid(point1.getY());
		double yMax = (point2.getY() < point1.getY())?floorYToGrid(point2.getY()):ceilYToGrid(point2.getY());

		for (double y = yMin; (yDiff < 0)?(y > yMax):(y < yMax); y += yStep) {
			double r = (y - point1.getY())/yDiff;
			double x = snapXToGrid( r * xDiff + point1.getX());
			result.push_back(Point2D(x,y));
		}
	}

	return result;
}
