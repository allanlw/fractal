/*
 * Copyright (c) 2011 Allan Wirth <allanlw@gmail.com>
 *
 * This file is part of Fractal Image Compressor.
 *
 * Fractal Image Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Image Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Image Compressor.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef _TRIANGLE_H
#define _TRIANGLE_H

class Triangle;

#include <vector>
#include <cstddef>
#include <string>
#include <ostream>
#include <istream>

#include "point2d.h"
#include "affinetransform.h"
#include "rectangle.h"
#include "trifit.h"

class Triangle {
private:
	struct Dependencies {
		unsigned short parent;
		unsigned short nextSibling;
		unsigned short prevSibling;
		unsigned short target;
		std::vector<unsigned short> children;
	};
	Triangle* nextSibling;
	Triangle* prevSibling;
	Triangle* parent;

	Dependencies* unresolvedDependencies;

	unsigned short id;

	std::vector<Point2D> points;

	std::vector<Triangle*> children;

	TriFit target;

	void assignPrevChildSibling(Triangle* prev, Triangle* triangle);
	void assignNextChildSibling(Triangle* next, Triangle* triangle);
public:
	Triangle(const Point2D& point0, const Point2D& point1,
			const Point2D& point2);
	Triangle(std::istream& in);
	~Triangle();
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
	const std::vector<Point2D>& getPoints() const;
	const std::vector<Triangle*>& getChildren() const;
	unsigned short getId() const;
	void setId(unsigned short id);

	void subdivide(double r01, double r02, double r12);
	void subdivideBarycentric();
	AffineTransform getTransformToTarget() const;
	Rectangle getBoundingBox() const;
	double getArea() const;
	bool pointInside(const Point2D& point) const;
	bool pointInsideSameSide(const Point2D& point) const;
	bool pointInsideBarycentric(const Point2D& point) const;
	Point2D calcCenteroid() const;

	std::string str() const;
	void serialize(std::ostream& out) const;
	void serializeID(std::ostream& out) const;
	void resolveDependencies(const std::vector<Triangle*>& tris);
};

#endif
