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

DoubleImage::DoubleImage() : image(NULL), sType(DEFAULT_SAMPLING_TYPE), dType(DEFAULT_DIVISION_TYPE), metric(DEFAULT_METRIC) {
}

DoubleImage::DoubleImage(gdImagePtr image) : sType(DEFAULT_SAMPLING_TYPE), dType(DEFAULT_DIVISION_TYPE), metric(DEFAULT_METRIC) {
	copyImage(&this->image, image);
}

DoubleImage::DoubleImage(const DoubleImage& img) : sType(img.sType), dType(img.dType), metric(img.metric) {
	copyImage(&(this->image), img.image);
	for(map<Channel, gdImagePtr>::const_iterator it = img.edges.begin(); it != img.edges.end(); it++) {
		copyImage(&edges[it->first], it->second);
	}
}

DoubleImage::DoubleImage(gdImagePtr image, SamplingType sType, DivisionType dType, Metric metric) :
	sType(sType), dType(dType), metric(metric) {
	copyImage(&this->image, image);
}

DoubleImage& DoubleImage::operator=(const DoubleImage& img) {
	if (this != &img) {
		copyImage(&(this->image), img.image);
		for(map<Channel, gdImagePtr>::const_iterator it = img.edges.begin(); it != img.edges.end(); it++) {
			copyImage(&edges[it->first], it->second);
		}
		this->sType = img.sType;
		this->dType = img.dType;
		this->metric = img.metric;
	}
	return *this;
}

DoubleImage::~DoubleImage() {
	if (this->image != NULL) {
		gdFree(image);
	}
	for(map<Channel, gdImagePtr>::const_iterator it = edges.begin(); it != edges.end(); it++) {
		if (it->second != NULL) {
			gdFree(it->second);
		}
	}
}

int DoubleImage::getWidth() const {
	return gdImageSX(image);
}

int DoubleImage::getHeight() const {
	return gdImageSY(image);
}

DoubleImage::Metric DoubleImage::getMetric() const {
	return metric;
}

void DoubleImage::setMetric(DoubleImage::Metric metric) {
	this->metric = metric;
}

DoubleImage::SamplingType DoubleImage::getSamplingType() const {
	return sType;
}

void DoubleImage::setSamplingType(DoubleImage::SamplingType sType) {
	this->sType = sType;
}

DoubleImage::DivisionType DoubleImage::getDivisionType() const {
	return dType;
}

void DoubleImage::setDivisionType(DoubleImage::DivisionType dType) {
	this->dType = dType;
}

bool DoubleImage::hasEdges() const {
	return !this->edges.empty();
}

void DoubleImage::copyImage(gdImagePtr* to, gdImagePtr from) {
	if (from != NULL) {
		*to = gdImageCreateTrueColor(gdImageSX(from), gdImageSY(from));
		gdImageCopy(*to, from, 0, 0, 0, 0, gdImageSX(from), gdImageSY(from));
	} else {
		*to = NULL;
	}
}

void DoubleImage::setImage(gdImagePtr image) {
	gdFree(this->image);
	copyImage(&this->image, image);
}

gdImagePtr DoubleImage::getImage() const {
	return image;
}

const map<Channel, gdImagePtr>& DoubleImage::getEdges() const {
	return edges;
}

void DoubleImage::generateEdges() {
	if (EDGE_DETECT_SOBEL) {
		edges[C_GREY] = edgeDetectSobel(image, C_GREY);
		edges[C_RED] = edgeDetectSobel(image, C_RED);
		edges[C_GREEN] = edgeDetectSobel(image, C_GREEN);
		edges[C_BLUE] = edgeDetectSobel(image, C_BLUE);
	} else {
		edges[C_GREY] = edgeDetectLaplace(image, C_GREY);
		edges[C_RED] = edgeDetectLaplace(image, C_RED);
		edges[C_GREEN] = edgeDetectLaplace(image, C_GREEN);
		edges[C_BLUE] = edgeDetectLaplace(image, C_BLUE);
	}
}

double DoubleImage::snapXToGrid(double x) const {
	return round( x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::snapYToGrid(double y) const {
	return round(y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

double DoubleImage::floorXToGrid(double x) const {
	return floor(x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::floorYToGrid(double y) const {
	return floor(y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

double DoubleImage::ceilXToGrid(double x) const {
	return ceil (x * (gdImageSX(image)-1) ) / (gdImageSX(image) - 1);
}

double DoubleImage::ceilYToGrid(double y) const {
	return ceil (y * (gdImageSY(image)-1) ) / (gdImageSY(image) - 1);
}

int DoubleImage::doubleToIntX(double x) const {
	return doubleToInt(x, 0, gdImageSX(image)-1);
}

int DoubleImage::doubleToIntY(double y) const {
	return doubleToInt(y, 0, gdImageSY(image)-1);
}

double DoubleImage::valueAt(double x, double y, Channel channel) const {
	const int _x = doubleToIntX(x);
	const int _y = doubleToIntY(y);

	const int val = getPixel(image, _x, _y, channel, false);
	return val;
}

double DoubleImage::edgeAt(double x, double y, Channel channel) const {
	const int _x = doubleToIntX(x);
	const int _y = doubleToIntY(y);

	const int val = getPixel(edges.find(channel)->second, _x, _y, C_GREY, false);
	return val;
}

double DoubleImage::edgeAt(const Point2D& point, Channel channel) const {
	return edgeAt(point.getX(), point.getY(), channel);
}

double DoubleImage::valueAt(const Point2D& point, Channel channel) const {
	return valueAt(point.getX(), point.getY(), channel);
}

TriFit DoubleImage::getOptimalFit(const Triangle* smaller, const Triangle* larger, Channel channel) {

	map<TriFit::PointMap, vector<double> > allConfigs = getAllConfigurations(smaller, larger, channel);
	TriFit best(0, 0, -1, TriFit::P000, larger);

	const TriFit::PointMap tm = TriFit::P000;

	const vector<double>& largerPoints = allConfigs[tm];

	for (map<TriFit::PointMap, vector<double> >::const_iterator it = allConfigs.begin(); it != allConfigs.end(); it++) {
		if (it->first == TriFit::P000) {
			continue;
		}
		const vector<double>& smallerPoints = it->second;
		double domainSum = sum(largerPoints.begin(), largerPoints.end());
		double domainSquaresSum = sumSquares(largerPoints.begin(), largerPoints.end());
		double productSum = dotProduct(largerPoints.begin(), largerPoints.end(),
		                               smallerPoints.begin(), smallerPoints.end());

		double rangeSum = sum(smallerPoints.begin(), smallerPoints.end());
		double rangeSquaresSum = sumSquares(smallerPoints.begin(), smallerPoints.end());
		double n = smallerPoints.size();

		double denom = ((n * n * domainSquaresSum) - (domainSum * domainSum));

		double s;
		double o;
		if (doublesEqual(denom, 0.0)) {
			s = 0;
			o = rangeSum / (n);
		} else {
			s = ((n * n * productSum) - (domainSum * rangeSum)) / denom;

			if (s > 1) {
				s = 1;
			} else if (s < 0) {
				s = 0;
			}

			o = (rangeSum - (s*domainSum)) / n;

			if (o < -gdRedMax) {
				o = -gdRedMax;
			} else if (o > gdRedMax) {
				o = gdRedMax;
			}
		}

/*		double r = ((rangeSquaresSum + (s * ((s * domainSquaresSum) - (2.0 *
				productSum) + (2.0 * o * domainSum))) + (o * ((o * n
				* n) - (2.0 * rangeSum))))) / n;
*/


//		s^2a^2 + soa -sab + osa + o^2 - ob - sab - bo + b^2
//		s*(sa^2 + 2oa - 2ab) + o(o - 2b) + b^2


		double r = 0;
		switch(metric) {
		default:
		case M_RMS:
			r = sqrt((s*(s*domainSquaresSum + 2*o*domainSum - 2*productSum) + o*(o*n - 2*rangeSum) + rangeSquaresSum)/n);
			break;
		case M_SUP:
			for(size_t j = 0; j < allConfigs[tm].size(); j++) {
				double t = (s*smallerPoints[j]+o - largerPoints[j]);
				if (t > r) {
					r = t;
				}
			}
			break;
		}
/*		double r = 0;

		for (size_t j = 0; j < allConfigs[tm].size(); j ++) {
			double t = (s * smallerPoints[j] + o - largerPoints[j]);
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

TriFit DoubleImage::getBestMatch(const Triangle* smaller, list<Triangle*>::const_iterator start, list<Triangle*>::const_iterator end, Channel channel) {
	TriFit result(0, 0, -1, TriFit::P000, NULL);
	const size_t minArea = getPointsInside(smaller).size() * MIN_SEARCH_RATIO;
	for(; start != end; start++) {
		if (getPointsInside(*start).size() < minArea) {
			continue;
		}
		TriFit f = getOptimalFit(smaller,*start, channel);
		if (f.error < result.error || result.error < 0) {
			result = f;
		}
	}
	if (outputDebug() && result.best != NULL) {
		output << " - ratio S:L " << (smaller->getArea() / result.best->getArea())<< endl;
	}
	return result;
}

void DoubleImage::mapPoints(const Triangle* t, TriFit fit, gdImagePtr to, Channel channel) {
	if (gdImageSX(image) != gdImageSX(to) || gdImageSY(image) != gdImageSY(to)) {
		throw logic_error("dimensions don't match!!!");
	}

	switch(sType) {
	case T_BOTHSAMPLE:
	case T_SUBSAMPLE: {
		const vector<Point2D>& points = getPointsInside(t);

//		const AffineTransform trans = AffineTransform(*t, *fit.best, fit.pMap);
		const AffineTransform trans = AffineTransform(*fit.best, *t, fit.pMap).getInverse();

		for (vector<Point2D>::const_iterator it = points.begin(); it != points.end(); it++) {
			mapPoint(to, fit, trans.transform(*it), *it, channel);
		}
		if (sType != T_BOTHSAMPLE) {
			break;
		}
	}
	case T_SUPERSAMPLE: {
		const vector<Point2D>& points = getPointsInside(fit.best);

		const AffineTransform trans = AffineTransform(*fit.best, *t, fit.pMap);

		for (vector<Point2D>::const_iterator it = points.begin(); it != points.end(); it++) {
			mapPoint(to, fit, *it, trans.transform(*it), channel);
		}

		break;
	}
	}
}

void DoubleImage::mapPoint(gdImagePtr to, const TriFit& fit, const Point2D& source, const Point2D& dest, Channel channel) {
	const int d_x = doubleToIntX(dest.getX());
	const int d_y = doubleToIntY(dest.getY());

	double newVal = (valueAt(source, channel) * fit.saturation) + fit.brightness;

	const int d_a = gdImageAlpha(to, gdImageGetPixel(to, d_x, d_y));
	const int newAlpha = d_a+1;

	if (newAlpha > 1) {
		const double oldVal = getPixel(to, d_x, d_y, channel, false);
		newVal = ((oldVal*d_a)+newVal)/(newAlpha);
	}

	const int newColor = boundColor(round(newVal));

	setPixel(to, d_x, d_y, newColor, channel, (newAlpha>=gdAlphaTransparent)?gdAlphaTransparent:newAlpha);
}

// result[P000] is the DOMAIN e.g. the larger triangle
map<TriFit::PointMap, vector<double> > DoubleImage::getAllConfigurations(const Triangle* smaller, const Triangle* larger, Channel channel) {
	map<TriFit::PointMap, vector<double> > result;

	vector<Point2D> largerPoints;

	switch (sType) {
	case T_BOTHSAMPLE:
	case T_SUPERSAMPLE:
		largerPoints = getPointsInside(larger);
		if (sType != T_BOTHSAMPLE) {
			break;
		}
	case T_SUBSAMPLE: {
		const vector<Point2D>& smallerPoints = getPointsInside(smaller);

		for (char i = 0; i < TriFit::NUM_MAPS; i++) {
			const TriFit::PointMap m = TriFit::pointMapFromInt(i);
			const AffineTransform trans = AffineTransform(*larger, *smaller, m).getInverse();
			for (vector<Point2D>::const_iterator it = smallerPoints.begin(); it != smallerPoints.end(); it++) {
				largerPoints.push_back(trans.transform(*it));
			}
		}
		break;
	}
	}

	for (char i = 0; i < TriFit::NUM_MAPS; i++) {
		const TriFit::PointMap m = TriFit::pointMapFromInt(i);

		result[m] = vector<double>(0);
		vector<double>& v = result[m];

		v.reserve(largerPoints.size());

		const AffineTransform trans = AffineTransform(*larger, *smaller, m);
		for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
			v.push_back(valueAt(trans.transform(*it), channel));
		}
	}

	result[TriFit::P000] = vector<double>(0);
	vector<double>& v = result[TriFit::P000];

	v.reserve(largerPoints.size());

	for (vector<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
		v.push_back(valueAt(*it, channel));
	}

	return result;
}

double DoubleImage::getBestDivide(const Point2D& first, const Point2D& second, Channel channel) const {

	if (dType == T_MIDDLE) {
		return .5;
	}

	bool high = (dType == T_HIGHENTROPY);

	if (!this->hasEdges()) {
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
		const double val = edgeAt(*it, channel);
		if ( ((high)?(bestVal < val):(bestVal > val)) || bestR < 0) {
			const double rx = (it->getX() - first.getX())/initialXDiff;
			const double ry = (it->getY() - first.getY())/initialYDiff;
			double r;
			if (doublesEqual(initialXDiff, 0)) {
				r = ry;
			} else if (doublesEqual(initialYDiff, 0)) {
				r = rx;
			} else {
				r = (rx+ry)/2.;
			}
			if (r > MIN_SUBDIVIDE_RATIO && r < 1-MIN_SUBDIVIDE_RATIO) {
				bestR = r;
				bestVal = val;
			}
		}
	}

	if (bestR < 0) {
		return 1/2.0;
	} else {
		return bestR;
	}
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
