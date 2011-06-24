#ifndef TRIANGLETREE_HPP_
#define TRIANGLETREE_HPP_

#include "triangle.hpp"

#include <deque>
#include <iterator>
#include <cstddef>
#include <ostream>
#include <istream>
#include "gd.h"

#include "constant.hpp"
#include "triangle.hpp"
#include "doubleimage.hpp"
#include "imageutils.hpp"

class TriangleTree {
private:
	Channel channel;
	DoubleImage& image;
	std::deque<Triangle*> unassigned;
	std::vector<Triangle*> allTriangles;
	unsigned short lastId;

	void subivide(Triangle* t);
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
	Triangle* assignOne(double cutoff);
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
	void renderTo(gdImagePtr image, bool fixErrors);
	const DoubleImage& getImage() const;
};

#endif

