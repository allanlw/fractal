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

double DoubleImage::valueAt(double x, double y) const {
	int _x = (int) round ( x * (double)gdImageSX(image) );
	int _y = (int) round ( y * (double)gdImageSY(image) );
	if (_x < 0) {
		_x = 0;
	} else if (_x >= gdImageSX(image)) {
		_x = gdImageSX(image);
	}
	if (_y < 0) {
		_y = 0;
	} else if (_y >= gdImageSY(image) ) {
		_y = gdImageSY(image);
	}
	int c = gdImageGetPixel(image, _x, _y);
	double r = double(gdImageRed(image, c)) / double(gdRedMax);
	double g = double(gdImageGreen(image, c)) / double(gdGreenMax);
	double b = double(gdImageBlue(image, c)) / double(gdBlueMax);
	return (r+g+b)/3.;
}

double DoubleImage::valueAt(const Point2D& point) const {
	return valueAt(point.getX(), point.getY());
}

TriFit DoubleImage::getOptimalFit(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap) {

	list<double> targetPoints(0);
	list<double> rangePoints(0);

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

void DoubleImage::getInsideAndCorresponding(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap, insert_iterator<list<double> > smallerInserter, insert_iterator<list<double> > largerInserter) {
	Rectangle bounds = larger->getBoundingBox();

	AffineTransform trans(*larger, *smaller, pMap);

	double xMax = snapXToGrid(bounds.getRight());
	double yMax = snapYToGrid(bounds.getBottom());
	double xInc = 1.0 / double(gdImageSX(image));
	double yInc = 1.0 / double(gdImageSY(image));

	for (double x = snapXToGrid(bounds.getLeft()); x <= xMax; x += xInc) {
		Point2D top(-1,-1);
		bool topSet = false;
		for (double y = snapYToGrid(bounds.getTop()); y <= yMax; y += yInc) {
			Point2D point(x,y);
			if (larger->pointInside(point)) {
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
			if (larger->pointInside(point)) {
				bottom = point;
				bottomSet = true;
				break;
			}
		}
		if (!bottomSet || top == bottom) {
			*largerInserter = valueAt(top);
			*smallerInserter = valueAt(trans.transform(top));
		} else {
			for (double y = bottom.getY(); y >= limit; y -= yInc) {
				Point2D point(x,y);
				*largerInserter = valueAt(point);
				*smallerInserter = valueAt(trans.transform(point));
			}
		}
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

TriFit DoubleImage::getBestMatch(Triangle* smaller, list<Triangle*>::const_iterator start, list<Triangle*>::const_iterator end) {
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
