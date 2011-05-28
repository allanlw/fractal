#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/Test.h>
#include <cppunit/Message.h>

#include "mathutils.hpp"
#include "vector2d.hpp"

using namespace std;

class Vector2DTest : public CppUnit::TestFixture {
private:
	Vector2D *vector1;
	Vector2D *vector2;
	Vector2D *vector3;
	Vector2D *vector4;
public:
	void setUp() {
		Point2D point1(0,0);
		Point2D point2(10,0);
		vector1 = new Vector2D(&point1, &point2);
		vector2 = new Vector2D(10, 0);
		vector3 = new Vector2D(10, PI);
		vector4 = new Vector2D(10, PI/2);
	}
	void tearDown() {
		delete vector1;
		delete vector2;
		delete vector3;
		delete vector4;
	}
	void testEquality() {
		CPPUNIT_ASSERT_MESSAGE(vector1->str() +"=="+vector1->str(), *vector1 == *vector1);
		CPPUNIT_ASSERT_MESSAGE( vector1->str() +"=="+vector2->str(), *vector1 == *vector2);
		CPPUNIT_ASSERT_MESSAGE(vector1->str() +"=="+vector3->str(), !(*vector1 == *vector3) );
		CPPUNIT_ASSERT_MESSAGE( vector3->str() +"=="+vector1->str(), !(*vector3 == *vector1));
		CPPUNIT_ASSERT_MESSAGE( vector2->str() +"=="+vector3->str(), !(*vector2 == *vector3));
		CPPUNIT_ASSERT_MESSAGE( vector3->str() +"=="+vector2->str(), !(*vector3 == *vector2));
	}
	void testInverse() {
		Vector2D* v1op = vector1->getOpposite();
		Vector2D* v2op = vector2->getOpposite();
		Vector2D* v3op = vector3->getOpposite();
		CPPUNIT_ASSERT_MESSAGE(v1op->str() +"=="+vector3->str(), *v1op == *vector3 );
		CPPUNIT_ASSERT_MESSAGE(v3op->str() +"=="+vector1->str(), *v3op == *vector1 );
		CPPUNIT_ASSERT_MESSAGE(v2op->str() +"=="+vector3->str(), *v2op == *vector3 );
		CPPUNIT_ASSERT_MESSAGE(v3op->str() +"=="+vector2->str(), *v3op == *vector2 );
		delete v1op;
		delete v2op;
		delete v3op;
	}
	void testAngleBetween() {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(PI/2,  vector1->angleBetween(vector4), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(PI/2, vector4->angleBetween(vector1), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, vector1->angleBetween(vector2), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, vector2->angleBetween(vector1), ZERO );
	}
	void testCrossProduct() {
		CPPUNIT_ASSERT_DOUBLES_EQUAL(100, vector1->crossProduct(vector4), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(100, vector4->crossProduct(vector1), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, vector1->crossProduct(vector2), ZERO );
		CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0, vector2->crossProduct(vector1), ZERO );
	}
	static CppUnit::Test *suite() {
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Vector2DTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<Vector2DTest>( 
									   "testInverse", 
									   &Vector2DTest::testInverse ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<Vector2DTest>(
									   "testEquality",
									   &Vector2DTest::testEquality ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<Vector2DTest>(
									   "testAngleBetween",
									   &Vector2DTest::testAngleBetween ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<Vector2DTest>(
									   "testCrossProduct",
									   &Vector2DTest::testCrossProduct ) );
		return suiteOfTests;
	}
};
