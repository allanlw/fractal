#include <cppunit/TestRunner.h>
#include <cppunit/ui/text/TestRunner.h>

#include "vector2dtest.hpp"
#include "point2dtest.hpp"
#include "triangletest.hpp"

int main(int argc, char* argv[]) {
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( Vector2DTest::suite() );
	runner.addTest( Point2DTest::suite() );
	runner.addTest( TriangleTest::suite());
	runner.run();
	return 0;
}
