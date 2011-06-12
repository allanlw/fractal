#include "triangletree.hpp"

#include <iostream>
#include "gd.h"
#include <stdexcept>

#include "mathutils.hpp"
#include "imageutils.hpp"

using namespace std;

TriangleTree::TriangleTree(DoubleImage image) : head(NULL), image(image), lastId(0) {
	std::vector<Point2D> corners = image.getCorners();
	head = new Triangle(corners[0], corners[1], corners[2]);
	head->setNextSibling(new Triangle(corners[0], corners[3], corners[2]));
	unassigned.push_back(head);
	unassigned.push_back(head->getNextSibling());
	allTriangles.push_back(head);
	allTriangles.push_back(head->getNextSibling());
}

TriangleTree::TriangleTree(DoubleImage image, istream& in) : head(NULL), image(image), lastId(0) {
	char magic[5];
	in.read(magic, 4);
	magic[4] = '\0';
	if (!(string("TREE") == magic)) {
		throw logic_error("NOT VALID FILE");
	}
	unsigned short numIds = unserializeUnsignedShort(in);
	cout << numIds << endl;
	vector<Triangle*> tris(numIds, NULL);

	for (std::vector<Triangle*>::size_type i = 0; i < numIds; i++) {
		Triangle* temp = new Triangle(in);
		tris[temp->getId()] = temp;
	}
	head = tris[0];
	for (std::vector<Triangle*>::size_type i = 0; i < numIds; i++) {
		tris[i]->resolveDependencies(tris);
	}
	allTriangles.insert(allTriangles.end(), tris.begin(), tris.end());
}

Triangle* TriangleTree::getHead() const {
	return head;
}
const std::list<Triangle*>* TriangleTree::getUnassigned() const {
	return &unassigned;
}

Triangle* TriangleTree::assignOne() {
	if (unassigned.size() == 0) {
		return NULL;
	}
	Triangle* next = unassigned.front();
	if (next == NULL) {
		return NULL;
	}
	unassigned.pop_front();
	next->setId(lastId++);

	list<Triangle*> above(0);
	insert_iterator<list<Triangle*> > it(above, above.begin());
	getAllAbove(next, it);
	TriFit best = image.getBestMatch(next, above.begin(), above.end());
	if (best.error < ERROR_CUTOFF && best.error >= 0) {
		next->setTarget(best);
	} else {
		subdivide(next);
	}
	return next;
}

void TriangleTree::subdivide(Triangle* t) {
	vector<Point2D> points = *(t->getPoints());
	double r01 = image.getBestDivide(points[0], points[1]);
	double r02 = image.getBestDivide(points[0], points[2]);
	double r12 = image.getBestDivide(points[1], points[2]);
	cout << r01 << " " << r02 << " " << r12 << endl;

	t->subdivide(r01, r02, r12);

	for(vector<Triangle*>::const_iterator it = t->getChildren()->begin(); it != t->getChildren()->end(); it++) {
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
	const vector<Triangle*>* children = t->getChildren();
	if (children != NULL && children->size() > 0) {
		*it = (*children)[0];
		getAllSiblings((*children)[0], it);
		getAllBelow((*children)[0], it);
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
	if (t->getChildren()->size() != 0) {
		const vector<Triangle*> children = *(t->getChildren());
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
	serializeTree(out, head);
}

void TriangleTree::eval() {

	gdImagePtr newImage = gdImageCreateTrueColor(gdImageSX(image.getImage()), gdImageSY(image.getImage()));

	gdImageAlphaBlending(newImage, 0);
	gdImageSaveAlpha(newImage, 1);

	gdImageFill(newImage, 0, 0, gdTrueColorAlpha(255, 0, 255, gdAlphaOpaque));

	for (list<Triangle*>::const_iterator it = allTriangles.begin(); it != allTriangles.end(); it++) {
		if (!(*it)->isTerminal()) {
			continue;
		}
		image.mapPoints(*it, (*it)->getTarget(), newImage);
	}
	clearAlpha(newImage);
	image.setImage(newImage);
}

const DoubleImage& TriangleTree::getImage() const {
	return image;
}
