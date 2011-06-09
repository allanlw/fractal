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

class AffineTransformTest : public CppUnit::TestFixture {
private:
	Point2D point1;
	Point2D point2;
	Point2D	point3;
	Point2D point4;
	Point2D point5;
	Point2D point6;

	Triangle tri1;
	Triangle tri3;
	Triangle tri4;

	AffineTransform a1;
	AffineTransform a2;
	AffineTransform a3;
	AffineTransform a4;
public:
	void setUp() {
		point1 = Point2D(0,0);
		point2 = Point2D(2,0);
		point3 = Point2D(0,2);
		point6 = Point2D(-PI, -PI);

		tri1 = Triangle(point1, point2, point3);
		tri3 = Triangle(Point2D(2,2), Point2D(4,2), Point2D(2,4));
		tri4 = Triangle(point2, point3, point6);

		a1 = AffineTransform(tri1, tri4, TriFit::P012);
		a2 = AffineTransform(tri4, tri1, TriFit::P012);
		a3 = AffineTransform(tri1, tri3, TriFit::P012);
		a4 = AffineTransform(tri3, tri1, TriFit::P012);
	}
	void tearDown() {
	}
	void testTransform() {
		Point2D p1(.5, .5);
		Point2D p3(0, 0);
		CPPUNIT_ASSERT(tri4.pointInside(a1.transform(p1)));
		CPPUNIT_ASSERT(tri1.pointInside(a2.transform(p3)));
		CPPUNIT_ASSERT(tri1.pointInside(a2.transform(p1)));
		CPPUNIT_ASSERT(tri3.pointInside(a3.transform(p1)));
		CPPUNIT_ASSERT(tri1.pointInside(a4.transform(Point2D(2.5,2.5))));
	}
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "AffineTransformTest" );
		suiteOfTests->addTest(new CppUnit::TestCaller<TriangleTest>("testPointInside", &TriangleTest::testPointInside));
		return suiteOfTests;
	}
};
