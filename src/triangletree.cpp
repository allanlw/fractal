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
#include "triangletree.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include "gd.h"
#include <stdexcept>

#include "mathutils.h"
#include "ioutils.h"
#include "imageutils.h"
#include "output.h"

using namespace std;

TriangleTree::TriangleTree(DoubleImage& image, Channel channel) : channel(channel), image(image), lastId(0), sMethod(DEFAULT_SUBDIVISION_METHOD) {
	std::vector<Point2D> corners = image.getCorners();
	Triangle* head = new Triangle(corners[0], corners[1], corners[2]);
	head->setNextSibling(new Triangle(corners[0], corners[3], corners[2]));
	unassigned.push_back(head);
	unassigned.push_back(head->getNextSibling());
	allTriangles.push_back(head);
	allTriangles.push_back(head->getNextSibling());
}

TriangleTree::TriangleTree(DoubleImage& image, istream& in, Channel channel) : channel(channel), image(image), lastId(0), sMethod(DEFAULT_SUBDIVISION_METHOD) {
	this->unserialize(in);
}

TriangleTree::~TriangleTree() {
	for (std::vector<Triangle*>::iterator it = allTriangles.begin(); it != allTriangles.end(); it++) {
		delete *it;
	}
}

TriangleTree::TriangleTree(const TriangleTree& tree) : image(tree.image), lastId(0), sMethod(tree.sMethod) {
	std::stringstream serial(ios_base::out|ios_base::in|ios_base::binary);

	tree.serialize(serial);

	unserialize(serial);
}

Triangle* TriangleTree::getHead() const {
	return allTriangles.front();
}

Channel TriangleTree::getChannel() const {
	return channel;
}

const std::deque<Triangle*>& TriangleTree::getUnassigned() const {
	return unassigned;
}

const std::vector<Triangle*>& TriangleTree::getAllTriangles() const {
	return allTriangles;
}

TriangleTree::SubdivisionMethod TriangleTree::getSubdivisionMethod() const {
	return sMethod;
}

void TriangleTree::setSubdivisionMethod(TriangleTree::SubdivisionMethod sMethod) {
	this->sMethod = sMethod;
}

Triangle* TriangleTree::assignOne(double cutoff) {
	if (unassigned.empty()) {
		return NULL;
	}
	Triangle* next = unassigned.front();
	if (next == NULL) {
		return NULL;
	}
	unassigned.pop_front();
	next->setId(lastId++);

	if (outputDebug()) {
		output << "Assigning Triangle #" << next->getId() << "..." << endl;
	}
	list<Triangle*> above(0);
	insert_iterator<list<Triangle*> > it(above, above.begin());
	getAllAbove(next, it);
	if (above.empty()) {
		subdivide(next);
		return next;
	} else {
		TriFit best = image.getBestMatch(next, above.begin(), above.end(), channel);
		if (outputDebug()) {
			output << " - Best Error: " << best.error << endl;
			output << " - # points inside: " << image.getPointsInside(next).size() << endl;
		}
		if (allTriangles.size() == MAX_NUM_TRIANGLES ||
			((best.error < cutoff*cutoff || image.getPointsInside(next).size() < MAX_SUBDIVIDE_SIZE || next->getArea() < MAX_SUBDIVIDE_AREA) && best.error >= 0)) {
			next->setTarget(best);
		} else {
			subdivide(next);
		}
	}
	return next;
}

void TriangleTree::subdivide(Triangle* t) {
	switch(sMethod) {
	case M_QUAD: {
		if (!image.hasEdges()) {
			image.generateEdges();
		}
		const vector<Point2D>& points = t->getPoints();
		double r01 = image.getBestDivide(points[0], points[1], channel);
		double r02 = image.getBestDivide(points[0], points[2], channel);
		double r12 = image.getBestDivide(points[1], points[2], channel);
		if (outputDebug()) {
			output << " - Subdivide Ratios: " << r01 << ", " << r02 << ", " << r12 << endl;
		}

		t->subdivide(r01, r02, r12);
		break;
	}
	case M_CENTEROID:
		t->subdivideBarycentric();
		break;
	}
	for(vector<Triangle*>::const_iterator it = t->getChildren().begin(); it != t->getChildren().end(); it++) {
		unassigned.push_back(*it);
		allTriangles.push_back(*it);
	}
}

void TriangleTree::getAllAbove(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	Triangle* parent = t->getParent();
	if (parent != NULL) {
		*it = parent;
		getAllSiblings(parent, it);
		getAllAbove(parent, it);
	}
}

void TriangleTree::getAllBelow(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	const vector<Triangle*>& children = t->getChildren();
	if (!children.empty()) {
		*it = children[0];
		getAllSiblings(children[0], it);
		getAllBelow(children[0], it);
	}
}

void TriangleTree::getAllSiblings(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	getAllNextSiblings(t, it);
	getAllPrevSiblings(t, it);
}

void TriangleTree::getAllNextSiblings(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	Triangle* next = t->getNextSibling();
	if (next != NULL) {
		*it = next;
		getAllNextSiblings(next, it);
	}
}

void TriangleTree::getAllPrevSiblings(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	Triangle* prev = t->getPrevSibling();
	if (prev != NULL) {
		*it = prev;
		getAllPrevSiblings(prev, it);
	}
}

unsigned short TriangleTree::getLastId() {
	return lastId;
}

void TriangleTree::serializeChildren(ostream& out, const Triangle* t) {
	t->serialize(out);
	if (!t->isTerminal()) {
		const vector<Triangle*>& children = t->getChildren();
		for (vector<Triangle*>::const_iterator it=children.begin(); it != children.end(); it++) {
			serializeChildren(out, *it);
		}
	}
}

void TriangleTree::serializeTree(ostream& out, const Triangle* t) {
	serializeChildren(out, t);
	if (t->getNextSibling() != NULL) {
		serializeTree(out, t->getNextSibling());
	}
}

void TriangleTree::serialize(ostream& out) const {
	out << "TREE";
	serializeUnsignedShort(out, lastId);
	serializeTree(out, allTriangles.front());
}

void TriangleTree::unserialize(istream& in) {
	char magic[5];
	in.read(magic, 4);
	magic[4] = '\0';
	if (!(string("TREE") == magic)) {
		throw logic_error("NOT VALID FILE");
	}
	unsigned short numIds = unserializeUnsignedShort(in);
	allTriangles.resize(numIds, NULL);

	for (std::vector<Triangle*>::size_type i = 0; i < numIds; i++) {
		Triangle* temp = new Triangle(in);
		allTriangles[temp->getId()] = temp;
	}

	for (std::vector<Triangle*>::size_type i = 0; i < numIds; i++) {
		allTriangles[i]->resolveDependencies(allTriangles);
	}
}

void TriangleTree::renderTo(gdImagePtr image, bool fixErrors) {
	if (outputVerbose()) {
		output << "Rendering channel " << channelToString(channel) << "..." << endl;
	}
	for (vector<Triangle*>::const_iterator it = allTriangles.begin(); it != allTriangles.end(); it++) {
		if (!(*it)->isTerminal()) {
			continue;
		}
		this->image.mapPoints(*it, (*it)->getTarget(), image, channel);
	}

	if (fixErrors) {
		interpolateErrors(image, channel);
	}

	clearAlpha(image);
}

const DoubleImage& TriangleTree::getImage() const {
	return image;
}
