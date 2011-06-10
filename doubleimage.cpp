#include "doubleimage.hpp"

#include <iostream>

#include "mathutils.hpp"
#include "constant.hpp"
#include "imageutils.hpp"

using namespace std;

DoubleImage::DoubleImage(gdImagePtr image) : image(image) {
	if (EDGE_DETECT_SOBEL) {
		edges = edgeDetectSobel(image);
	} else {
		edges = edgeDetectLaplace(image);
	}
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

Point2D DoubleImage::snapToGrid(const Point2D& point) const {
	return Point2D(snapXToGrid(point.getX()), snapYToGrid(point.getY()));
}

int DoubleImage::doubleToInt(double x, int min, int max) {
	int _x = (int) round (x * (double)max);
	if (_x < min) {
		_x = min;
	} else if (_x >= max) {
		_x = max-1;
	}
	return _x;
}

int DoubleImage::doubleToIntX(double x) const {
	return doubleToInt(x, 0, gdImageSX(image));
}

int DoubleImage::doubleToIntY(double y) const {
	return doubleToInt(y, 0, gdImageSY(image));
}

int DoubleImage::doubleToIntC(double c) {
	return doubleToInt(c, 0, gdRedMax);
}

double DoubleImage::valueAt(double x, double y) const {
	int _x = doubleToIntX(x);
	int _y = doubleToIntY(y);
	int c = gdImageGetPixel(image, _x, _y);
	double r = ((double)gdImageRed(image, c)) / ((double)gdRedMax);
	double g = ((double)gdImageGreen(image, c)) / ((double)gdGreenMax);
	double b = ((double)gdImageBlue(image, c)) / ((double)gdBlueMax);
	return (r+g+b)/3.;
}

double DoubleImage::valueAt(const Point2D& point) const {
	return valueAt(point.getX(), point.getY());
}

TriFit DoubleImage::getOptimalFit(const Triangle* smaller, const Triangle* larger, TriFit::PointMap pMap) const {

	list<double> targetPoints;
	list<double> rangePoints;

	insert_iterator<list<double> > rangeInserter(rangePoints, rangePoints.begin());
	insert_iterator<list<double> > domainInserter(targetPoints, targetPoints.begin());

	getInsideAndCorresponding(smaller, larger, pMap, rangeInserter, domainInserter);

	if (targetPoints.size() != rangePoints.size()) {
		throw logic_error("Target and Range Points not the same size.");
	}

	double domainSum = sum(targetPoints.begin(), targetPoints.end());
	double rangeSum = sum(rangePoints.begin(), rangePoints.end());
	double domainSquaresSum = sumSquares(targetPoints.begin(), targetPoints.end());
	double rangeSquaresSum = sumSquares(rangePoints.begin(), rangePoints.end());
	double productSum = dotProduct(targetPoints.begin(), targetPoints.end(), rangePoints.begin(), rangePoints.end());
	double n = targetPoints.size();

	double s = ((n * n * productSum) - (domainSum * rangeSum))
			/ ((n * n * domainSquaresSum) - (domainSum * domainSum));

	double o = (rangeSum - (s * domainSum)) / (n * n);

	double r = ((rangeSquaresSum + (s * ((s * domainSquaresSum) - (2.0 *
			productSum) + (2.0 * o * domainSum))) + (o * ((o * n
			* n) - (2.0 * rangeSum)))));

	TriFit result(s,o,r,pMap,larger);

	return result;
}

double DoubleImage::getYInc() const {
	return 1.0 / ((double)gdImageSY(image));
}

double DoubleImage::getXInc() const {
	return 1.0 / ((double)gdImageSX(image));
}

list<Point2D> DoubleImage::getPointsInside(const Triangle* t) const {
	list<Point2D> result;

	const Rectangle bounds = t->getBoundingBox();

	double xMax = snapXToGrid(bounds.getRight());
	double yMax = snapYToGrid(bounds.getBottom());
	double xInc = getXInc();
	double yInc = getYInc();

	for (double x = snapXToGrid(bounds.getLeft()); x <= xMax; x += xInc) {
		Point2D top(-1,-1);
		bool topSet = false;
		for (double y = snapYToGrid(bounds.getTop()); y <= yMax; y += yInc) {
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

void DoubleImage::getInsideAndCorresponding(const Triangle* smaller, const Triangle* larger, TriFit::PointMap pMap, insert_iterator<list<double> > smallerInserter, insert_iterator<list<double> > largerInserter) const {
	AffineTransform trans(*larger, *smaller, pMap);

	list<Point2D> largerPoints = getPointsInside(larger);
	for (list<Point2D>::const_iterator it = largerPoints.begin(); it != largerPoints.end(); it++) {
		*largerInserter = valueAt(*it);
		*smallerInserter = valueAt(trans.transform(*it));
	}
}

std::vector<Point2D> DoubleImage::getCorners() {
	vector<Point2D> result;
	result.push_back(Point2D(0.,0.));
	result.push_back(Point2D(1.,0.));
	result.push_back(Point2D(1.,1.));
	result.push_back(Point2D(0.,1.));
	return result;
}

TriFit DoubleImage::getBestMatch(const Triangle* smaller, list<Triangle*>::const_iterator start, list<Triangle*>::const_iterator end) const {
	TriFit result(0, 0, -1, TriFit::P012, NULL);
	double maxArea = MAX_SEARCH_RATIO*smaller->getArea();
	for(; start != end; start++) {
//		if ((*start)->getArea() > maxArea || smaller == *start) {
//			continue;
//		}
		for (unsigned char i = 0; i < TriFit::NUM_MAPS; i++) {
			TriFit::PointMap map = TriFit::pointMapFromInt(i);
			TriFit f(getOptimalFit(smaller,*start, map));
			if (f.error < result.error || result.error < 0) {
				result = f;
			}
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

void DoubleImage::mapPoints(const Triangle* t, TriFit fit, gdImagePtr to) const {
	if (gdImageSX(image) != gdImageSX(to) || gdImageSY(image) != gdImageSY(to)) {
		throw logic_error("dimensions don't match!!!");
	}
	list<Point2D> originPoints = getPointsInside(fit.best);
	AffineTransform trans(*fit.best, *t, fit.pMap);
	for (list<Point2D>::const_iterator it = originPoints.begin(); it != originPoints.end(); it++) {
		double value = valueAt(*it);
		Point2D dest = trans.transform(*it);
		int d_x = doubleToIntX(dest.getX());
		int d_y = doubleToIntY(dest.getY());
		int d_a = gdImageAlpha(to, gdImageGetPixel(to, d_x, d_y));
		double newValue = value*fit.saturation + fit.brightness;
		int newColor = doubleToIntC(newValue);
		int newAlpha = d_a+1;

		if (d_a != gdAlphaOpaque) {
			int oldColor = getPixel(to, d_x, d_y) * d_a;
			newColor = (oldColor+newColor)/newAlpha;
		}
		int c = gdTrueColorAlpha(newAlpha, boundColor(newColor),
		                         boundColor(newColor), boundColor(newColor));
		gdImageSetPixel(to, d_x, d_y, c);
	}
}
