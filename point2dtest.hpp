#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Test.h>
#include <cppunit/Message.h>

#include "mathutils.hpp"
#include "vector2d.hpp"

using namespace std;

class Point2DTest : public CppUnit::TestFixture {
private:
	Point2D *point1;
	Point2D *point2;
	Point2D	*point3;
public:
	void setUp() {
		point1 = new Point2D(0,0);
		point2 = new Point2D(0, 0);
		point3 = new Point2D(2,2);
	}
	void tearDown() {
		delete point1;
		delete point2;
		delete point3;
	}
	void testEquality() {
		CPPUNIT_ASSERT_MESSAGE(point1->str() +"=="+point1->str(), *point1 == *point1);
		CPPUNIT_ASSERT_MESSAGE( point1->str() +"=="+point2->str(), *point1 == *point2);
		CPPUNIT_ASSERT_MESSAGE(point1->str() +"=="+point3->str(), !(*point1 == *point3) );
		CPPUNIT_ASSERT_MESSAGE( point3->str() +"=="+point1->str(), !(*point3 == *point1));
		CPPUNIT_ASSERT_MESSAGE( point2->str() +"=="+point3->str(), !(*point2 == *point3));
		CPPUNIT_ASSERT_MESSAGE( point3->str() +"=="+point2->str(), !(*point3 == *point2));
	}
	void testDistance() {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2)*2,  point1->distance(*point3), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(sqrt(2)*2, point3->distance(*point1), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, point1->distance(*point2), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, point2->distance(*point1), ZERO );
	}
	void testDistanceSquared() {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0,  point1->distanceSquared(*point3), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(8.0, point3->distanceSquared(*point1), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, point1->distanceSquared(*point2), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, point2->distanceSquared(*point1), ZERO );
	}
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Point2DTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<Point2DTest>(
									   "testEquality",
									   &Point2DTest::testEquality ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<Point2DTest>(
									   "testDistance",
									   &Point2DTest::testDistance ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<Point2DTest>(
									   "testDistanceSquared",
									   &Point2DTest::testDistanceSquared ) );
		return suiteOfTests;
	}
};
