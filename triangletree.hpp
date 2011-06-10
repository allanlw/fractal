#ifndef TRIANGLETREE_HPP_
#define TRIANGLETREE_HPP_

#include "triangle.hpp"

#include <list>
#include <iterator>
#include <cstddef>
#include <ostream>
#include <istream>

#include "constant.hpp"
#include "triangle.hpp"
#include "doubleimage.hpp"

class TriangleTree {
private:
	Triangle* head;
	DoubleImage image;
	std::list<Triangle*> unassigned;
	unsigned short lastId;

	void subivide(Triangle* t);
public:
	explicit TriangleTree(DoubleImage& image);
	TriangleTree(DoubleImage&, std::istream& in);
	Triangle* getHead() const;
	const std::list<Triangle*>* getUnassigned() const;
	Triangle* assignOne();
	void subdivide(Triangle* t);
	static void getAllAbove(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllBelow(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllNextSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	static void getAllPrevSiblings(Triangle* t, std::insert_iterator<std::list<Triangle*> >& it);
	unsigned short getLastId();
	void serialize(std::ostream& out) const;
	static void serializeTree(std::ostream& out, const Triangle* t);
	static void serializeChildren(std::ostream& out, const Triangle* t);
	void eval();
};

#endif
