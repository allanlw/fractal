#include <cmath>
#include <vector>

#include "triangle.hpp"
#include "vector2d.hpp"
#include "mathutils.hpp"

using namespace std;

Triangle::Triangle(const Point2D* point0, const Point2D* point1,
		const Point2D* point2) :  nextSibling(NULL),prevSibling(NULL), parent(NULL), terminal(true), id(0),points(3), children(0), target(NULL)
,contrast(0), brightness(0), pointMap(P012) {
	this->points[0] = point0;
	this->points[1] = point1;
	this->points[2] = point2;
}

Triangle * Triangle::getNextSibling() const {
	return this->nextSibling;
}
Triangle * Triangle::getPrevSibling() const {
	return this->prevSibling;
}
Triangle * Triangle::getParent() const {
	return this->parent;
}
Triangle * Triangle::getTarget() const {
	return this->target;
}
const std::vector<const Point2D*>* Triangle::getPoints() const {
	return &(this->points);
}
const std::vector<Triangle*>* Triangle::getChildren() const {
	return &(this->children);
}
bool Triangle::isTerminal() const {
	return this->terminal;
}
double Triangle::getBrightness() const {
	return this->brightness;
}
double Triangle::getContrast() const {
	return this->contrast;
}
unsigned int Triangle::getId() const {
	return this->id;
}
Triangle::PointMap Triangle::getPointMap() const {
	return this->pointMap;
}

Rectangle* Triangle::getBoundingBox() const {
	double maxx = this->points[0]->getX();
	double maxy = this->points[0]->getY();
	double minx = maxx;
	double miny = maxy;
	for (vector<const Point2D*>::size_type i = 1; i < this->points.size(); i++) {
		if (this->points[i]->getX() > maxx) {
			maxx = this->points[i]->getX();
		}
		if (this->points[i]->getX() < minx) {
			minx = this->points[i]->getX();
		}
		if (this->points[i]->getY() > maxy) {
			maxy = this->points[i]->getY();
		}
		if (this->points[i]->getY() < miny) {
			miny = this->points[i]->getY();
		}
	}
	return new Rectangle(minx, miny, maxx - minx, maxy - miny);
}

short int Triangle::getPoint0(PointMap pointMap) {
	switch (pointMap) {
	case P012:
	case P021:
		return 0;
		break;
	case P102:
	case P120:
		return 1;
		break;
	case P201:
	case P210:
		return 2;
		break;
	default:
		return -1;
	}
}
short int Triangle::getPoint1(PointMap pointMap) {
	switch (pointMap) {
	case P102:
	case P201:
		return 0;
		break;
	case P012:
	case P210:
		return 1;
		break;
	case P021:
	case P120:
		return 2;
		break;
	default:
		return -1;
	}
}
short int Triangle::getPoint2(PointMap pointMap) {
	switch (pointMap) {
	case P120:
	case P210:
		return 0;
		break;
	case P201:
	case P021:
		return 1;
		break;
	case P102:
	case P012:
		return 2;
		break;
	default:
		return -1;
	}
}

double Triangle::getArea() const {
	Vector2D ab (this->points[0], this->points[1]);
	Vector2D ac (this->points[0], this->points[2]);
	return abs(ab.crossProduct(&ac))/2.0;
}