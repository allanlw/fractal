#include "ioutils.hpp"

#include <cmath>

using namespace std;

void serializeDouble(ostream& out, double d) {
	out.write((char*)(&d), 8);
}

void serializeUnsignedShort(ostream& out, unsigned short s) {
	unsigned char bytes[2] = {(unsigned char)((s & 0xFF00) >> 8),
	                          (unsigned char)((s & 0x00FF)) };
	out.write((char*)bytes, 2);
}

void serializeSignedInt(ostream& out, signed int i) {
	unsigned char bytes[4] = {(unsigned char)((i & 0xFF000000) >> 24),
	                          (unsigned char)((i & 0x00FF0000) >> 16),
	                          (unsigned char)((i & 0x0000FF00) >> 8),
	                          (unsigned char)((i & 0x000000FF)) };
	out.write((char*)bytes, 4);
}

void serializeString(ostream& out, string str) {
	unsigned short len = (str.size()<=0xFFFF)?str.size():0xFFFF;
	serializeUnsignedShort(out, len);
	out.write(str.c_str(), len);
}

void serializeFraction(ostream& out, double d, int min, int max) {
	if (d < min) {
		d = min;
	} else if (d > max) {
		d = max;
	}

	double val = (d - min) / (max - min);

	signed int fraction = round((double)(0x7FFFFFFF) * val);
	if (fraction < 0) {
		fraction = 0x7FFFFFFF;
	}
	serializeSignedInt(out, fraction);
}

double unserializeFraction(istream& in, int min, int max) {
	int fraction = unserializeSignedInt(in);

	double val = ((double)fraction)/((signed int)0x7FFFFFFF);

	int range = max - min;

	double result = (range * val) + min;

	if (result > max) {
		return max;
	} else if (result < min) {
		return min;
	} else {
		return result;
	}
}

unsigned short unserializeUnsignedShort(istream& in) {
	unsigned char bytes[2];
	in.read((char*)bytes, 2);
	unsigned short result = ((unsigned short)bytes[0] << 8) | (bytes[1]);
	return result;
}

double unserializeDouble(istream& in) {
	union {
		double d;
		unsigned char bytes[8];
	};
	in.read((char*)bytes, 8);
	return d;
}

signed int unserializeSignedInt(istream& in) {
	unsigned char bytes[4];
	in.read((char*)bytes, 4);
	signed int result = ((unsigned int)bytes[0] << 24) |
	                    ((unsigned int)bytes[1] << 16) |
	                    ((unsigned int)bytes[2] << 8) |
	                    ((unsigned int)bytes[3]);
	return result;
}

string unserializeString(istream& in) {
	size_t len = unserializeUnsignedShort(in);
	char result[len];
	in.read(result, len);
	return string(result, len);
}

string getBasename(string filename) {
	size_t loc = filename.rfind('/');
	if (loc == string::npos) {
		return filename;
	} else {
		return filename.substr(loc);
	}
}
