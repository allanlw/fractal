#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>
#include <iostream>

#include "vector2dtest.hpp"
#include "point2dtest.hpp"
#include "triangletest.hpp"
#include "affinetransformtest.hpp"

using namespace std;

unsigned char outputLevel = 0;

std::ostream& output = std::cout;

int main(int argc, char* argv[]) {
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( Vector2DTest::suite() );
	runner.addTest( Point2DTest::suite() );
	runner.addTest( TriangleTest::suite());
	runner.addTest( AffineTransformTest::suite());
	runner.run();
	return 0;
}
