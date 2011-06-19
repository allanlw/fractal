#include "triangletree.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include "gd.h"
#include <stdexcept>

#include "mathutils.hpp"
#include "imageutils.hpp"
#include "output.hpp"

using namespace std;

TriangleTree::TriangleTree(DoubleImage image) : image(image), lastId(0) {
	std::vector<Point2D> corners = image.getCorners();
	Triangle* head = new Triangle(corners[0], corners[1], corners[2]);
	head->setNextSibling(new Triangle(corners[0], corners[3], corners[2]));
	unassigned.push_back(head);
	unassigned.push_back(head->getNextSibling());
	allTriangles.push_back(head);
	allTriangles.push_back(head->getNextSibling());
}

TriangleTree::TriangleTree(DoubleImage image, istream& in) : image(image), lastId(0) {
	this->unserialize(in);
}


TriangleTree::~TriangleTree() {
	for (std::vector<Triangle*>::iterator it = allTriangles.begin(); it != allTriangles.end(); it++) {
		delete *it;
	}
}

TriangleTree::TriangleTree(const TriangleTree& tree) : image(tree.image), lastId(0) {
	std::stringstream serial(ios_base::out|ios_base::in|ios_base::binary);

	tree.serialize(serial);

	unserialize(serial);
}

Triangle* TriangleTree::getHead() const {
	return allTriangles.front();
}

const std::deque<Triangle*>& TriangleTree::getUnassigned() const {
	return unassigned;
}

const std::vector<Triangle*>& TriangleTree::getAllTriangles() const {
	return allTriangles;
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
#if USE_HEURISTICS == 1
	if (predictError(next->getArea()) > cutoff*PREDICT_ACCURACY) {
		subdivide(next);
		return next;
	}
#endif
	list<Triangle*> above(0);
	insert_iterator<list<Triangle*> > it(above, above.begin());
	getAllAbove(next, it);
	if (above.empty()) {
		subdivide(next);
		return next;
	} else {
		TriFit best = image.getBestMatch(next, above.begin(), above.end());
		if (outputDebug()) {
			output << " - Best Error: " << best.error << endl;
		}
		if (best.error < cutoff && best.error >= 0) {
			next->setTarget(best);
		} else {
			subdivide(next);
		}
	}
	return next;
}

void TriangleTree::subdivide(Triangle* t) {
	if (image.getEdges() == NULL) {
		image.generateEdges();
	}
	const vector<Point2D>& points = t->getPoints();
	double r01 = image.getBestDivide(points[0], points[1]);
	double r02 = image.getBestDivide(points[0], points[2]);
	double r12 = image.getBestDivide(points[1], points[2]);
	if (outputDebug()) {
		output << " - Subdivide Ratios: " << r01 << ", " << r02 << ", " << r12 << endl;
	}

	t->subdivide(r01, r02, r12);

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

void TriangleTree::eval() {

	gdImagePtr newImage = gdImageCreateTrueColor(gdImageSX(image.getImage()), gdImageSY(image.getImage()));

	gdImageAlphaBlending(newImage, 0);
	gdImageSaveAlpha(newImage, 1);

	gdImageFill(newImage, 0, 0, gdTrueColorAlpha(255, 0, 255, gdAlphaOpaque));

	for (vector<Triangle*>::const_iterator it = allTriangles.begin(); it != allTriangles.end(); it++) {
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

TriangleTree TriangleTree::loadFractal(const char* in, int width, int height) {

	if (outputStd()) {
		output << "loading fractal " << in << "..." << endl;
	}

	gdImagePtr noise = blankCanvas(width, height);
	DoubleImage img(noise);
	gdFree(noise);

	ifstream inStream(in, ios_base::in | ios_base::binary);
	TriangleTree tree(img, inStream);
	inStream.close();

	if (outputStd()) {
		output << "fractal loaded. (" << tree.getAllTriangles().size() << " triangles)" << endl;
	}

	return tree;
}
