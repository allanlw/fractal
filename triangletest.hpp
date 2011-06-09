#include <string>
#include <vector>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Test.h>
#include <cppunit/Message.h>

#include "mathutils.hpp"
#include "point2d.hpp"
#include "triangle.hpp"

using namespace std;

class TriangleTest : public CppUnit::TestFixture {
private:
	Point2D *point1;
	Point2D *point2;
	Point2D	*point3;
	Point2D *point4;
	Point2D *point5;
	Point2D *point6;
	
	Triangle *tri1;
	Triangle *tri2;
	Triangle *tri3;
	Triangle *tri4;
public:
	void setUp() {
		point1 = new Point2D(0,0);
		point2 = new Point2D(2,0);
		point3 = new Point2D(0,2);
		point4 = new Point2D(100, 100);
		point5 = new Point2D(-PI, PI);
		point6 = new Point2D(-PI, -PI);
		
		tri1 = new Triangle(*point1, *point2, *point3);
		tri2 = new Triangle(*point1, *point4, *point5);
		tri3 = new Triangle(*point2, *point3, *point4);
		tri4 = new Triangle(*point2, *point3, *point6);
	}
	void tearDown() {
		delete point1;
		delete point2;
		delete point3;
		delete point4;
		delete point5;
		delete point6;

		delete tri1;
		delete tri2;
		delete tri3;
		delete tri4;
	}
	void testArea() {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(2.0, tri1->getArea(), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(314.159265358979, tri2->getArea(), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(198, tri3->getArea(), ZERO );
	}
	void testRectangleArea() {
		Rectangle rect1 = tri1->getBoundingBox();
		Rectangle rect2 = tri2->getBoundingBox();
		Rectangle rect3 = tri3->getBoundingBox();
		CPPUNIT_ASSERT(tri1->getArea() < rect1.getArea());
		CPPUNIT_ASSERT(tri2->getArea() < rect2.getArea());
		CPPUNIT_ASSERT(tri3->getArea() < rect3.getArea());
	}
	void testBoundingBox () {
		Rectangle rect1 = tri1->getBoundingBox();
		Rectangle rect2 = tri2->getBoundingBox();
		Rectangle rect3 = tri3->getBoundingBox();
		vector<Point2D> points1 = *(tri1->getPoints());
		vector<Point2D> points2 = *(tri2->getPoints());
		vector<Point2D> points3 = *(tri3->getPoints());
		CPPUNIT_ASSERT_MESSAGE(points1[0].str() + " in " + rect1.str(),rect1.pointInside(points1[0]));
		CPPUNIT_ASSERT_MESSAGE(points1[1].str() + " in " + rect1.str(),rect1.pointInside(points1[1]));
		CPPUNIT_ASSERT_MESSAGE(points1[2].str() + " in " + rect1.str(),rect1.pointInside(points1[2]));
		CPPUNIT_ASSERT_MESSAGE(points2[0].str() + " in " + rect2.str(),rect2.pointInside(points2[0]));
		CPPUNIT_ASSERT_MESSAGE(points2[1].str() + " in " + rect2.str(),rect2.pointInside(points2[1]));
		CPPUNIT_ASSERT_MESSAGE(points2[2].str() + " in " + rect2.str(),rect2.pointInside(points2[2]));
		CPPUNIT_ASSERT_MESSAGE(points3[0].str() + " in " + rect3.str(),rect3.pointInside(points3[0]));
		CPPUNIT_ASSERT_MESSAGE(points3[1].str() + " in " + rect3.str(),rect3.pointInside(points3[1]));
		CPPUNIT_ASSERT_MESSAGE(points3[2].str() + " in " + rect3.str(),rect3.pointInside(points3[2]));
	}
	void testPointInside() {
		Point2D p1(.5, .5);
		Point2D p3(0, 0);
		CPPUNIT_ASSERT(tri1->pointInside(p1));
		CPPUNIT_ASSERT(tri4->pointInside(p3));
		CPPUNIT_ASSERT(tri4->pointInside(p1));
	}
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "TriangleTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<TriangleTest>(
									   "testArea",
									   &TriangleTest::testArea ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<TriangleTest>(
									   "testRectangleArea",
									   &TriangleTest::testRectangleArea ));
		suiteOfTests->addTest( new CppUnit::TestCaller<TriangleTest>(
									   "testBoundingBox",
									   &TriangleTest::testBoundingBox ));
		suiteOfTests->addTest(new CppUnit::TestCaller<TriangleTest>("testPointInside", &TriangleTest::testPointInside));
		return suiteOfTests;
	}
};
