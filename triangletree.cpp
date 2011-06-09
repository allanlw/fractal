#include "triangletree.hpp"

#include <iostream>

using namespace std;

TriangleTree::TriangleTree(DoubleImage image) : head(NULL), image(image), lastId(0) {
	std::vector<Point2D> corners = image.getCorners();
	head = new Triangle(corners[0], corners[1], corners[2]);
	head->setNextSibling(new Triangle(corners[0], corners[3], corners[2]));
	unassigned.push_back(head);
	unassigned.push_back(head->getNextSibling());
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
	if (next->getArea() > MIN_SEARCH_AREA) {
		subdivide(next);
	} else {
		list<Triangle*> above(0);
		insert_iterator<list<Triangle*> > it(above, above.begin());
		getAllAbove(next, it);
		cout << above.size() << endl;
		list<Triangle*>::const_iterator begin = above.begin();
		list<Triangle*>::const_iterator end = above.end();
		TriFit best = image.getBestMatch(next, begin, end);
		if (best.error < ERROR_CUTOFF && best.error >= 0) {
			next->setTarget(best);
		} else {
			subdivide(next);
		}
	}
	return next;
}

void TriangleTree::subdivide(Triangle* t) {
	t->subdivide(.5, .5, .5);
	for(vector<Triangle*>::const_iterator it = t->getChildren()->begin(); it != t->getChildren()->end(); it++) {
		unassigned.push_back(*it);
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


std::size_t TriangleTree::getLastId() {
	return lastId;
}
