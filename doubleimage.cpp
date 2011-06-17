#include "doubleimage.hpp"
#include <iostream>
#include <utility>
#include <vector>
#include <list>
#include <cstdio>

#include "mathutils.hpp"
#include "constant.hpp"
#include "imageutils.hpp"

using namespace std;

DoubleImage::DoubleImage(gdImagePtr image)  {
	this->image = gdImageCreateTrueColor(gdImageSX(image), gdImageSY(image));
	gdImageCopy(this->image, image, 0, 0, 0, 0, gdImageSX(image), gdImageSY(image));
	if (EDGE_DETECT_SOBEL) {
		edges = edgeDetectSobel(this->image);
	} else {
		edges = edgeDetectLaplace(this->image);
	}
}

DoubleImage::DoubleImage(const DoubleImage& img) {
	this->image = gdImageCreateTrueColor(gdImageSX(img.image), gdImageSY(img.image));
	gdImageCopy(this->image, img.image, 0, 0, 0, 0, gdImageSX(this->image), gdImageSY(this->image));
	this->edges = gdImageCreateTrueColor(gdImageSX(img.edges), gdImageSY(img.edges));
	gdImageCopy(this->edges, img.edges, 0, 0, 0, 0, gdImageSX(this->edges), gdImageSY(this->edges));
}

DoubleImage::~DoubleImage() {
	gdFree(image);
	gdFree(edges);
}

double DoubleImage::snapXToGrid(double x) const {
	return round ( x * (double) gdImageSX(image) ) / ((double)gdImageSX(image));
}

double DoubleImage::snapYToGrid(double y) const {
	return round (y * (double) gdImageSY(image) ) / ((double) gdImageSY(image));
}

double DoubleImage::floorXToGrid(double x) const {
	return floor (x * (double) gdImageSX(image) ) / ((double) gdImageSX(image));
}

double DoubleImage::floorYToGrid(double y) const {

	return floor (y * (double) gdImageSY(image) ) / ((double) gdImageSY(image));
}

double DoubleImage::ceilXToGrid(double x) const {
	return ceil( x * (double) gdImageSX(image) ) / ((double) gdImageSX(image));
}

double DoubleImage::ceilYToGrid(double y) const {
	return ceil( y * (double) gdImageSY(image) ) / ((double) gdImageSY(image));
}

Point2D DoubleImage::snapToGrid(const Point2D& point) const {
	return Point2D(snapXToGrid(point.getX()), snapYToGrid(point.getY()));
}

int DoubleImage::doubleToInt(double x, int min, int max) {
	const int _x = (int) round (x * (double)(max-1));
	if (_x < min) {
		return min;
	} else if (_x >= max) {
		return max-1;
	} else {
		return _x;
	}
}

int DoubleImage::doubleToIntX(double x) const {
	return doubleToInt(x, 0, gdImageSX(image));
}

int DoubleImage::doubleToIntY(double y) const {
	return doubleToInt(y, 0, gdImageSY(image));
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

	double domainSum = sum(allConfigs[tm].begin(), allConfigs[tm].end());
	double domainSquaresSum = sumSquares(allConfigs[tm].begin(), allConfigs[tm].end());
	double n = allConfigs[tm].size();

	double denom = ((n * n * domainSquaresSum) - (domainSum * domainSum));

	for (char i = 0; i < TriFit::NUM_MAPS; i++) {
		TriFit::PointMap m = TriFit::pointMapFromInt(i);
		double rangeSum = sum(allConfigs[m].begin(), allConfigs[m].end());
		double rangeSquaresSum = sumSquares(allConfigs[m].begin(), allConfigs[m].end());
		double productSum = dotProduct(allConfigs[tm].begin(), allConfigs[tm].end(), allConfigs[m].begin(), allConfigs[m].end());


//		cout << " RS: " << rangeSum << " RSS: " << rangeSquaresSum << "pS: " << productSum;
//		cout << " dS: " << domainSum << " DSS: " << domainSquaresSum << " n: " << n <<  endl;

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
			best.pMap = m;
		}

	}
	return best;
}

double DoubleImage::getYInc() const {
	return 1.0 / ((double)gdImageSY(image));
}

double DoubleImage::getXInc() const {
	return 1.0 / ((double)gdImageSX(image));
}

const vector<Point2D>& DoubleImage::getPointsInside(const Triangle* t)  {

	map<const Triangle*, vector<Point2D> >::const_iterator it = pointsCache.find(t);

	if (it != pointsCache.end()) {
		return it->second;
	}

	pointsCache[t] = vector<Point2D>(t->getPoints()->begin(), t->getPoints()->end());
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
	const size_t minArea = getPointsInside(smaller).size() * MIN_SEARCH_RATIO;
	for(; start != end; start++) {
		if (getPointsInside(*start).size() < minArea) {
			continue;
		}
		TriFit f = getOptimalFit(smaller,*start);
		if (f.error < result.error || result.error < 0) {
			result = f;
		}
	}
	return result;
}

void DoubleImage::setImage(gdImagePtr image) {
	this->image = image;
}

gdImagePtr DoubleImage::getImage() const {
	return image;
}

void DoubleImage::mapPoints(const Triangle* t, TriFit fit, gdImagePtr to) {

	if (gdImageSX(image) != gdImageSX(to) || gdImageSY(image) != gdImageSY(to)) {
		throw logic_error("dimensions don't match!!!");
	}

	const vector<Point2D>& targetPoints = getPointsInside(t);
	const vector<Point2D>& sourcePoints = getPointsInside(fit.best);
	vector<Point2D> transPoints;
	vector<pair<const Point2D*, const Point2D*> > allPoints;

	transPoints.reserve(targetPoints.size() + sourcePoints.size());
	allPoints.reserve(targetPoints.size() + sourcePoints.size());

	const AffineTransform trans(*fit.best, *t, fit.pMap);

	for (vector<Point2D>::const_iterator it = targetPoints.begin(); it != targetPoints.end(); it++) {
		transPoints.push_back(trans.inverseTransform(*it));
		allPoints.push_back(pair<const Point2D*, const Point2D*>(&transPoints.back(), &(*it)));
	}

	for (vector<Point2D>::const_iterator it = sourcePoints.begin(); it != sourcePoints.end(); it++) {
		transPoints.push_back(trans.transform(*it));
		allPoints.push_back(pair<const Point2D*, const Point2D*>(&(*it), &transPoints.back()));
	}

	for (vector<pair<const Point2D*, const Point2D*> >::const_iterator it = allPoints.begin(); it != allPoints.end(); it++) {

		const Point2D& source = *(it->first);
		const Point2D& dest = *(it->second);

		const int d_x = doubleToIntX(dest.getX());
		const int d_y = doubleToIntY(dest.getY());

		double newVal = (valueAt(source) * fit.saturation) + fit.brightness;

		const int d_a = gdImageAlpha(to, gdImageGetPixel(to, d_x, d_y));
		int newAlpha = d_a+1;

		if (newAlpha > 1) {
			const double oldVal = getPixel(to, d_x, d_y, false);
			newVal = ((oldVal*d_a)+newVal)/(newAlpha);
		}

		const int newColor = boundColor(round(newVal));

		if (newAlpha > gdAlphaTransparent) {
			newAlpha = gdAlphaTransparent;
		}

		const int c = gdTrueColorAlpha(newColor, newColor, newColor, newAlpha);

		gdImageSetPixel(to, d_x, d_y, c);
	}
}

map<TriFit::PointMap, vector<double> > DoubleImage::getAllConfigurations(const Triangle* smaller, const Triangle* larger) {
	const vector<Point2D>& largerPoints = getPointsInside(larger);

	map<TriFit::PointMap, vector<double> > result;

	for (char i = 0; i < TriFit::NUM_MAPS + 1; i++) {
		const TriFit::PointMap m = TriFit::pointMapFromInt(i);

		result[m] = vector<double>();
		vector<double>& v = result[m];

		v.reserve(largerPoints.size());

		if (m != TriFit::P000) {
			AffineTransform trans(*larger, *smaller, m);
			for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
				v.push_back(valueAt(trans.transform(*it)));
			}
		} else {
			for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
				v.push_back(valueAt(*it));
			}
		}
	}
	return result;
}

double DoubleImage::getBestDivide(const Point2D& first, const Point2D& second, bool high) const {

	double initialXDiff = second.getX() - first.getX();
	double initialYDiff = second.getY() - first.getY();

	Point2D point1(first.getX() + MIN_SUBDIVIDE_RATIO*(initialXDiff),
	               first.getY() + MIN_SUBDIVIDE_RATIO*(initialYDiff));
	Point2D point2(first.getX() + (1-MIN_SUBDIVIDE_RATIO)*(initialXDiff),
	               first.getY() + (1-MIN_SUBDIVIDE_RATIO)*(initialYDiff));

	double xDiff = point2.getX() - point1.getX();
	double yDiff = point2.getY() - point1.getY();

	double bestVal = -1;
	double bestR = -1;

	if (abs(xDiff) > abs(yDiff)) {
		double xStep = getXInc();

		if (xDiff < 0) {
			xStep *= -1;
		}

		double xMin = snapXToGrid(point1.getX());
		double xMax = snapXToGrid(point2.getX());

		for (double x = xMin; (xDiff < 0)?(x > xMax):(x < xMax); x += xStep) {
			double r = (x - xMin)/xDiff;
			double y = snapYToGrid( r * yDiff + point1.getY());
			double val = edgeAt(x, y);
			if (high && (bestR < 0 || bestVal < val)) {
				bestR = r;
				bestVal = val;
			} else if (!high && (bestR < 0 || bestVal > val)) {
				bestR = r;
				bestVal = val;
			}
		}
	} else {
		double yStep = getYInc();

		if (yDiff < 0) {
			yStep *= -1;
		}

		double yMin = snapYToGrid(point1.getY());
		double yMax = snapYToGrid(point2.getY());

		for (double y = yMin; (yDiff < 0)?(y > yMax):(y < yMax); y += yStep) {
			double r = (y - yMin)/yDiff;
			double x = snapXToGrid( r * xDiff + point1.getX());
			double val = edgeAt(x, y);
			if (high && (bestR < 0 || bestVal < val)) {
				bestR = r;
				bestVal = val;
			} else if (!high && (bestR < 0 || bestVal > val)) {
				bestR = r;
				bestVal = val;
			}
		}
	}

	double actualR = MIN_SUBDIVIDE_RATIO + (1-(2*MIN_SUBDIVIDE_RATIO))*bestR;

	return actualR;
}
