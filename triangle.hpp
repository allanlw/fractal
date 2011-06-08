#ifndef TRIANGLE_HPP_
#define TRIANGLE_HPP_

class AffineTransform;

#include <vector>
#include <cstddef>

#include "point2d.hpp"
#include "rectangle.hpp"
#include "trifit.hpp"

class Triangle {
public:
	enum PointMap {
		P012, P021, P102, P120, P201, P210
	};
	static const unsigned char NUM_MAPS = 6;
	static PointMap pointMapFromInt(unsigned char pMap);
private:
	Triangle* nextSibling;
	Triangle* prevSibling;
	Triangle* parent;
	bool terminal;

	std::size_t id;

	std::vector<const Point2D*> points;

	std::vector<Triangle*> children;

	Triangle* target;
	TriFit fit;
	PointMap pointMap;

	void assignPrevChildSibling(Triangle* prev, Triangle* triangle);
	void assignNextChildSibling(Triangle* next, Triangle* triangle);
public:
	Triangle(const Point2D* point0, const Point2D* point1,
			const Point2D* point2);
	//Getters and Setters
	void setNextSibling(Triangle* next);
	void setParent(Triangle* parent);
	void setPrevSibling(Triangle* prev);
	void setTarget(Triangle* target, TriFit fit,
			PointMap pointMap);
	Triangle* getNextSibling() const;
	Triangle* getPrevSibling() const;
	Triangle* getParent() const;
	bool isTerminal() const;
	Triangle* getTarget() const;
	TriFit getFit() const;
	const std::vector<const Point2D*>* getPoints() const;
	PointMap getPointMap() const;
	const std::vector<Triangle*>* getChildren() const;
	std::size_t getId() const;
	void setId(std::size_t id);

	void subdivide(double r01, double r02, double r12);
	AffineTransform getTransformToTarget() const;
	Rectangle getBoundingBox() const;
	double getArea() const;
	bool pointInside(const Point2D& point) const;

	static unsigned char getPoint0(PointMap pointMap);
	static unsigned char getPoint1(PointMap pointMap);
	static unsigned char getPoint2(PointMap pointMap);

};

#include "affinetransform.hpp"

#endif /* TRIANGLE_HPP_ */
