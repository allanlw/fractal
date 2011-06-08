#include "triangletree.hpp"

using namespace std;

TriangleTree::TriangleTree(DoubleImage image) : head(NULL), image(image), lastId(0) {
	std::vector<Point2D> corners = image.getCorners();
	head = new Triangle(&(corners[0]), &(corners[1]), &(corners[2]));
	head->setNextSibling(new Triangle(&(corners[0]), &(corners[3]), &(corners[2])));
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
	lastId++;
	next->setId(this->lastId);
	if (next->getArea() > MIN_SEARCH_AREA) {
		subdivide(next);
		return next;
	}
	list<Triangle*> above;
	insert_iterator<list<Triangle*> > it(above, above.end());
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
	t->subdivide(.5, .5, .5);
	for(vector<Triangle*>::const_iterator it = t->getChildren()->begin(); it != t->getChildren()->end(); it++) {
		unassigned.push_front(*it);
	}
}

void TriangleTree::getAllAbove(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	if (t != NULL) {
		Triangle* parent = t->getParent();
		if (parent != NULL) {
			getAllSiblings(parent, it);
			getAllAbove(parent, it);
		}
	}
}

void TriangleTree::getAllSiblings(Triangle* t, insert_iterator<list<Triangle*> >& it) {
	if (t->getNextSibling() != NULL) {
		getAllNextSiblings(t, it);
	}
	if (t->getPrevSibling() != NULL) {
		getAllPrevSiblings(t, it);
	}
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
