#include "mathutils.hpp"

using namespace std;

double sum(std::list<double>::const_iterator start, std::list<double>::const_iterator end) {
	double result = 0;
	for (; start != end; start++) {
		result += *start;
	}
	return result;
}

double avg(std::list<double>::const_iterator start, std::list<double>::const_iterator end) {
	double s = 0;
	double denom = 0;
	for(; start != end; start++, denom+=1) {
		s += *start;
	}
	return s/denom;
}

double sumSquares(std::list<double>::const_iterator start, std::list<double>::const_iterator end) {
	double result = 0;
	for (; start != end; start++) {
		double val = *start;
		result += val * val;
	}
	return result;
}

double dotProduct(std::list<double>::const_iterator start1, std::list<double>::const_iterator end1,
                  std::list<double>::const_iterator start2, std::list<double>::const_iterator end2) {
	double result = 0;
	for (; start1 != end1 && start2 != end2; start1++, start2++) {
		result += (*start1) * (*start2);
	}
	return result;
}

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
