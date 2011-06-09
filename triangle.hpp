#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

class Triangle;

#include <vector>
#include <cstddef>
#include <string>

#include "point2d.hpp"
#include "affinetransform.hpp"
#include "rectangle.hpp"
#include "trifit.hpp"

class Triangle {
private:
	Triangle* nextSibling;
	Triangle* prevSibling;
	Triangle* parent;
	bool terminal;

	std::size_t id;

	std::vector<Point2D> points;

	std::vector<Triangle*> children;

	TriFit target;

	void assignPrevChildSibling(Triangle* prev, Triangle* triangle);
	void assignNextChildSibling(Triangle* next, Triangle* triangle);
public:
	Triangle(const Point2D& point0, const Point2D& point1,
			const Point2D& point2);
	//Getters and Setters
	void setNextSibling(Triangle* next);
	void setParent(Triangle* parent);
	void setPrevSibling(Triangle* prev);
	void setTarget(TriFit fit);
	Triangle* getNextSibling() const;
	Triangle* getPrevSibling() const;
	Triangle* getParent() const;
	bool isTerminal() const;
	TriFit getTarget() const;
	const std::vector<Point2D>* getPoints() const;
	const std::vector<Triangle*>* getChildren() const;
	std::size_t getId() const;
	void setId(std::size_t id);

	void subdivide(double r01, double r02, double r12);
	AffineTransform getTransformToTarget() const;
	Rectangle getBoundingBox() const;
	double getArea() const;
	bool pointInside(const Point2D& point) const;
	bool pointInsideSameSide(const Point2D& point) const;
	bool pointInsideBarycentric(const Point2D& point) const;
	Point2D calcCenteroid() const;

	std::string str() const;
};

#endif /* TRIANGLE_HPP_ */
