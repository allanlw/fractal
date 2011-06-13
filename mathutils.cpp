#include "mathutils.hpp"

using namespace std;

void serializeDouble(ostream& out, double d) {
	out.write((char*)(&d), 8);
}

void serializeUnsignedShort(ostream& out, unsigned short s) {
	out.write((char*)(&s), 2);
}

unsigned short unserializeUnsignedShort(istream& in) {
	union {
		char bytes[2];
		unsigned short s;
	};
	in.read(bytes, 2);
	return s;
}

double unserializeDouble(istream& in) {
	union {
	double d;
	char bytes[8];
	};
	in.read(bytes, 8);
	return d;
}
