#ifndef TRIANGLETREE_HPP_
#define TRIANGLETREE_HPP_

#include "triangle.hpp"

#include <list>
#include <iterator>
#include <cstddef>

#include "constant.hpp"
#include "triangle.hpp"
#include "doubleimage.hpp"

class TriangleTree {
private:
	Triangle* head;
	DoubleImage image;
	std::list<Triangle*> unassigned;
	std::size_t lastId;

	void subivide(Triangle* t);
public:
	TriangleTree(DoubleImage image);
	Triangle* getHead() const;
	const std::list<Triangle*>* getUnassigned() const;
	Triangle* assignOne();
	void subdivide(Triangle* t);
	void getAllAbove(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	void getAllSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	void getAllNextSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	void getAllPrevSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	std::size_t getLastId();
};

#endif
