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
#ifndef _TRIANGLETREE_H
#define _TRIANGLETREE_H

#include <deque>
#include <iterator>
#include <cstddef>
#include <ostream>
#include <istream>
#include "gd.h"

#include "constant.h"
#include "triangle.h"
#include "doubleimage.h"
#include "imageutils.h"

class TriangleTree {
public:
	enum SubdivisionMethod {
		M_QUAD,
		M_CENTEROID
	};
private:
	Channel channel;
	DoubleImage& image;
	std::deque<Triangle*> unassigned;
	std::vector<Triangle*> allTriangles;
	unsigned short lastId;

	SubdivisionMethod sMethod;

	void subdivide(Triangle* t);
	void unserialize(std::istream& in);
public:
	TriangleTree(DoubleImage& image, Channel channel);
	TriangleTree(const TriangleTree& tree);
	TriangleTree(DoubleImage&, std::istream& in, Channel channel);
	~TriangleTree();
	Triangle* getHead() const;
	Channel getChannel() const;
	const std::deque<Triangle*>& getUnassigned() const;
	const std::vector<Triangle*>& getAllTriangles() const;
	SubdivisionMethod getSubdivisionMethod() const;
	void setSubdivisionMethod(SubdivisionMethod sMethod);

	Triangle* assignOne(double cutoff);
	static void getAllAbove(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllBelow(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllNextSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllPrevSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	unsigned short getLastId();
	void serialize(std::ostream& out) const;
	static void serializeTree(std::ostream& out, const Triangle* t);
	static void serializeChildren(std::ostream& out, const Triangle* t);
	void renderTo(gdImagePtr image, bool fixErrors);
	const DoubleImage& getImage() const;
};

#endif
