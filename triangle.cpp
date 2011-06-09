#include <cmath>
#include <vector>
#include <sstream>

#include "triangle.hpp"
#include "vector2d.hpp"
#include "mathutils.hpp"
#include "constant.hpp"

using namespace std;

Triangle::Triangle(const Point2D& point0, const Point2D& point1,
		const Point2D& point2) :  nextSibling(NULL),prevSibling(NULL), parent(NULL), terminal(true), id(0), children(0) {
	points.reserve(3);
	points.push_back(point0);
	points.push_back(point1);
	points.push_back(point2);
}

Triangle * Triangle::getNextSibling() const {
	return this->nextSibling;
}
void Triangle::setNextSibling(Triangle* next) {
	this->nextSibling = next;
	next->prevSibling = this;
}
Triangle * Triangle::getPrevSibling() const {
	return this->prevSibling;
}
void Triangle::setPrevSibling(Triangle* prev) {
	this->prevSibling = prev;
	prev->nextSibling = this;
}
Triangle * Triangle::getParent() const {
	return this->parent;
}
void Triangle::setParent(Triangle* parent) {
	this->parent = parent;
}
TriFit Triangle::getTarget() const {
	return this->target;
}
void Triangle::setTarget(TriFit target) {
	this->target = target;
}
const std::vector<Point2D>* Triangle::getPoints() const {
	return &(this->points);
}
const std::vector<Triangle*>* Triangle::getChildren() const {
	return &(this->children);
}
bool Triangle::isTerminal() const {
	return this->terminal;
}
size_t Triangle::getId() const {
	return this->id;
}
void Triangle::setId(size_t id) {
	this->id = id;
}

Rectangle Triangle::getBoundingBox() const {
	double maxx = this->points[0].getX();
	double maxy = this->points[0].getY();
	double minx = maxx;
	double miny = maxy;
	for (vector<const Point2D*>::size_type i = 1; i < this->points.size(); i++) {
		if (this->points[i].getX() > maxx) {
			maxx = this->points[i].getX();
		}
		if (this->points[i].getX() < minx) {
			minx = this->points[i].getX();
		}
		if (this->points[i].getY() > maxy) {
			maxy = this->points[i].getY();
		}
		if (this->points[i].getY() < miny) {
			miny = this->points[i].getY();
		}
	}
	return Rectangle(minx, miny, maxx - minx, maxy - miny);
}

double Triangle::getArea() const {
	Vector2D ab (points[0], points[1]);
	Vector2D ac (points[0], points[2]);
	return abs(ab.crossProduct(ac))/2.0;
}

bool Triangle::pointInside(const Point2D& point) const {
	if (SAME_SIDE_TECHNIQUE) {
		return pointInsideSameSide(point);
	} else {
		return pointInsideBarycentric(point);
	}
}
bool Triangle::pointInsideSameSide(const Point2D& point) const {
	const Vector2D ba(points[0], points[1], true);
	const Vector2D pa(points[0], point, true);
	const Vector2D ca(points[0], points[2], true);
	if (signum(ba.crossProduct(pa)) != signum(ba.crossProduct(ca))) {
		return false;
	}
	const Vector2D cb(points[1], points[2], true);
	const Vector2D pb(points[1], point, true);
	const Vector2D ab = ba.getOpposite();
	if (signum(cb.crossProduct(pb)) != signum(cb.crossProduct(ab))) {
		return false;
	}
	const Vector2D ac = ca.getOpposite();
	const Vector2D pc(points[2], point, true);
	const Vector2D bc = cb.getOpposite();
	if (signum(ac.crossProduct(pc)) != signum(ac.crossProduct(bc))) {
		return false;
	}
	return true;
}

bool Triangle::pointInsideBarycentric(const Point2D& point) const {
	const Vector2D v0(points[0], points[2]);
	const Vector2D v1(points[0], points[1]);
	const Vector2D v2(points[0], point);

	double dot00 = v0.selfDotProduct();
	double dot01 = v0.dotProduct(v1);
	double dot02 = v0.dotProduct(v2);
	double dot11 = v1.selfDotProduct();
	double dot12 = v1.dotProduct(v2);

	double invDenom = 1/(dot00*dot11 - dot01*dot01);
	double u = (dot11*dot02 - dot01*dot12) * invDenom;
	double v = (dot00*dot12 - dot01*dot02) * invDenom;

	return (u > 0) && (v > 0) && ((u + v) < 1.0);
}
void Triangle::subdivide(double r01, double r02, double r12) {
	Point2D midpoint01(points[0].getX() +
                               ((points[1].getX() - points[0].getX()) * r01),
                           points[0].getY() +
                               ((points[1].getY() - points[0].getY()) * r01));
	Point2D midpoint02(points[0].getX() +
                              ((points[2].getX() - points[0].getX()) * r02),
                           points[0].getY() +
                               ((points[2].getY() - points[0].getY()) * r02));
	Point2D midpoint12(points[1].getX() +
                               ((points[2].getX() - points[1].getX()) * r12),
                           points[1].getY() +
                               ((points[2].getY() - points[1].getY()) * r12));

	children.resize(4);
	children[0] = new Triangle(points[0], midpoint01, midpoint02);
	children[1] = new Triangle(points[1], midpoint01, midpoint12);
	children[2] = new Triangle(points[2], midpoint02, midpoint12);
	children[3] = new Triangle(midpoint01, midpoint02, midpoint12);

	children[0]->setParent(this);
	children[1]->setParent(this);
	children[2]->setParent(this);
	children[3]->setParent(this);

	children[0]->setNextSibling(children[1]);

	children[1]->setNextSibling(children[2]);

	children[2]->setNextSibling(children[3]);

	assignNextChildSibling(nextSibling, children[3]);

	assignPrevChildSibling(prevSibling, children[0]);

	this->terminal = false;
}

void Triangle::assignNextChildSibling(Triangle* next, Triangle* t) {
	if (next != NULL) {
		const vector<Triangle*>* nextChildren = next->getChildren();
		if (nextChildren != NULL && nextChildren->size() > 0) {
			t->setNextSibling((*nextChildren)[0]);
		} else {
			assignNextChildSibling(next->getNextSibling(), t);
		}
	}
}

void Triangle::assignPrevChildSibling(Triangle* prev, Triangle* t) {
	if (prev != NULL) {
		const vector<Triangle*>* prevChildren = prev->getChildren();
		if (prevChildren != NULL && prevChildren->size() > 0) {
			t->setPrevSibling((*prevChildren)[prevChildren->size()-1]);
		} else {
			assignPrevChildSibling(prev->getPrevSibling(), t);
		}
	}
}

string Triangle::str() const {
	ostringstream s;
	s << "(" << points[0].str() <<",";
	s <<  points[1].str() << ",";
	s << points[2].str() << ")";
	return s.str();
}

Point2D Triangle::calcCenteroid() const {
	double x = (points[0].getX() + points[1].getX() + points[2].getX())/3;
	double y = (points[0].getY() + points[1].getY() + points[2].getY())/3;
	return Point2D(x,y);
}
