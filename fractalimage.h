#ifndef _FRACTALIMAGE_H
#define _FRACTALIMAGE_H

#include <vector>
#include <istream>
#include <ostream>
#include "gd.h"

#include "triangletree.h"
#include "doubleimage.h"
#include "triangle.h"
#include "metadata.h"

class FractalImage {
public:
	enum ImageType {
		T_GREYSCALE,
		T_COLOR
	};
private:
	ImageType type;
	DoubleImage image;
	std::vector<TriangleTree*> channels;
	MetaData metadata;
public:
	FractalImage(std::istream& in, DoubleImage image);
	FractalImage(DoubleImage image, ImageType type);
	const DoubleImage& getImage() const;
	void setImage(DoubleImage image);
	ImageType getType() const;
	std::vector<Triangle*>::size_type getSize() const;
	const std::vector<TriangleTree*>& getChannels() const;
	MetaData& getMetadata();
	void serialize(std::ostream& out) const;
	void encode(double error);
	void setSubdivisionMethod(TriangleTree::SubdivisionMethod sMethod);
	gdImagePtr decode(bool fixErrors);
	~FractalImage();
};

#endif
