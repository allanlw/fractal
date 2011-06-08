#include <cmath>
#include <vector>

#include "triangle.hpp"
#include "vector2d.hpp"
#include "mathutils.hpp"

using namespace std;

Triangle::Triangle(const Point2D* point0, const Point2D* point1,
		const Point2D* point2) :  nextSibling(NULL),prevSibling(NULL), parent(NULL), terminal(true), id(0),points(3), children(0) {
	this->points[0] = point0;
	this->points[1] = point1;
	this->points[2] = point2;
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
const std::vector<const Point2D*>* Triangle::getPoints() const {
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
	return Rectangle(minx, miny, maxx - minx, maxy - miny);
}

double Triangle::getArea() const {
	Vector2D ab (*points[0], *points[1]);
	Vector2D ac (*points[0], *points[2]);
	return abs(ab.crossProduct(ac))/2.0;
}

bool Triangle::pointInside(const Point2D& point) const {
	const Vector2D ba(*points[0], *points[1], true);
	const Vector2D pa(*points[0], point, true);
	const Vector2D ca(*points[0], *points[2], true);
	if (signum(ba.crossProduct(pa)) != signum(ba.crossProduct(ca))) {
		return false;
	}
	const Vector2D cb(*points[1], *points[2], true);
	const Vector2D pb(*points[1], point, true);
	const Vector2D ab = ba.getOpposite();
	if (signum(cb.crossProduct(pb)) != signum(cb.crossProduct(ab))) {
		return false;
	}
	const Vector2D ac = ca.getOpposite();
	const Vector2D pc(*points[2], point, true);
	const Vector2D bc = cb.getOpposite();
	if (signum(ac.crossProduct(pc)) != signum(ac.crossProduct(bc))) {
		return false;
	}
	return true;
}

void Triangle::subdivide(double r01, double r02, double r12) {
	Point2D* midpoint01 = new Point2D(points[0]->getX() +
                               ((points[1]->getX() - points[0]->getX()) * r01),
                           points[0]->getY() +
                               ((points[1]->getY() - points[0]->getY()) * r01));
	Point2D* midpoint02 = new Point2D(points[0]->getX() +
                               ((points[2]->getX() - points[0]->getX()) * r02),
                           points[0]->getY() +
                               ((points[2]->getY() - points[0]->getY()) * r02));
	Point2D* midpoint12 = new Point2D(points[1]->getX() +
                               ((points[2]->getX() - points[1]->getX()) * r12),
                           points[1]->getY() +
                               ((points[2]->getY() - points[1]->getY()) * r12));

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
