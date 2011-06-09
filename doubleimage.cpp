#include "doubleimage.hpp"

#include <iostream>

#include "mathutils.hpp"
#include "constant.hpp"

using namespace std;

DoubleImage::DoubleImage(gdImagePtr image) : image(image) {
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
	int c = gdImageGetPixel(image, x, y);
	double r = gdImageRed(image, c);
	double g = gdImageGreen(image, c);
	double b = gdImageBlue(image, c);
	return (r+g+b)/3.;
}

double DoubleImage::valueAt(const Point2D& point) const {
	return valueAt(point.getX(), point.getY());
}

TriFit DoubleImage::getOptimalFit(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap) {
	list<double> targetPoints(0);
	list<double> rangePoints(0);
	getInsideAndCorresponding(smaller, larger, pMap, insert_iterator<list<double> >(rangePoints, rangePoints.begin()), insert_iterator<list<double> >(targetPoints, targetPoints.begin()));

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

	double r = ((rangeSquaresSum + (s * (s * domainSquaresSum - 2.0 *
			productSum + 2.0 * o * domainSum)) + (o * (o * n
			* n - 2.0 * rangeSum))));
	cout << n << ","<< "," << domainSum << "," << rangeSum << ",";
	cout << domainSquaresSum << "," << rangeSquaresSum << "," << productSum << ",";
	cout <<s << "," << o << "," << r << endl;
	TriFit result(s,o,r,pMap,larger);

	return result;
}

/*
std::list<double> DoubleImage::getPointsInside(Triangle* t) {
	Rectangle bounds = t->getBoundingBox();
	list<double> results;

	for (double x = bounds.getX(); x <= bounds.getX() + bounds.getWidth(); x += 1 / ((double) gdImageSX(image))) {
		for (double y = bounds.getY(); y <= bounds.getY() + bounds.getHeight(); y += 1/((double)gdImageSY(image))) {
			Point2D point(x, y);
			if (t->pointInside(point)) {
				results.push_back(valueAt(point));
			}
		}
	}

	return results;
}

std::list<double> DoubleImage::getCorrespondingPoints(Triangle* smaller, Triangle* larger, Triangle::PointMap pMap) {
	Rectangle bounds = larger->getBoundingBox();

	AffineTransform trans(larger, smaller, pMap);

	list<double> results;

	for(double x= bounds.getX(); x <= bounds.getX() + bounds.getWidth(); x += 1/((double)gdImageSX(image))) {
		for (double y = bounds.getY(); y <= bounds.getY() + bounds.getHeight(); y += ((double)gdImageSY(image))) {
			Point2D point(x,y);
			if (larger->pointInside(point)) {
				Point2D transformed = trans.transform(point);
				results.push_back(valueAt(transformed));
			}
		}
	}
	return results;
}
*/

void DoubleImage::getInsideAndCorresponding(Triangle* smaller, Triangle* larger, TriFit::PointMap pMap, insert_iterator<list<double> > smallerInserter, insert_iterator<list<double> > largerInserter) {
	Rectangle bounds = larger->getBoundingBox();

	AffineTransform trans(*larger, *smaller, pMap);
	double xMax = snapXToGrid(bounds.getRight());
	double yMax = snapYToGrid(bounds.getBottom());
	double xInc = 1.0 / ((double)gdImageSX(image));
	double yInc = 1.0 / ((double)gdImageSY(image));

	cout << trans.str() << endl;

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
	for(list<Triangle*>::const_iterator it = start; it!=end;it++) {
		if ((*it)->getArea() > maxArea || smaller == *it) {
			continue;
		}
		for (unsigned char i = 0; i < TriFit::NUM_MAPS; i++) {
			TriFit::PointMap map = TriFit::pointMapFromInt(i);
			TriFit f(getOptimalFit(smaller,*it, map));
			if (f.getError() < result.getError() || result.getError() < 0) {
				result = f;
			}
		}
	}
	return result;
}
