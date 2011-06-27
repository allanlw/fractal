#ifndef _IOUTILS_HPP
#define _IOUTILS_HPP

#include <ostream>
#include <istream>
#include <string>

void serializeDouble(std::ostream& out, double d);
void serializeFraction(std::ostream& out, double d, int min, int max);
void serializeUnsignedShort(std::ostream& out, unsigned short s);
void serializeSignedInt(std::ostream& out, signed int i);
void serializeString(std::ostream& out, std::string str);

double unserializeDouble(std::istream& in);
double unserializeFraction(std::istream& in, int min, int max);
unsigned short unserializeUnsignedShort(std::istream& in);
signed int unserializeSignedInt(std::istream& in);
std::string unserializeString(std::istream& in);

std::string getBasename(std::string filename);

#endif
