/*
 * Copyright (c) 2011 Allan Wirth <allanlw@gmail.com>
 *
 * This file is part of Fractal Image Compressor.
 *
 * Fractal Image Compressor is free software: you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Fractal Image Compressor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Fractal Image Compressor.
 * If not, see <http://www.gnu.org/licenses/>.
 */
#include "fractalimage.h"

#include <stdexcept>

#include "output.h"
#include "imageutils.h"
#include "ioutils.h"

using namespace std;

FractalImage::FractalImage(istream& in, DoubleImage image) : image(image) {
	if (outputVerbose()) {
		output << "Loading fractal..." << endl;
	}

	char magic[8];
	in.read(magic, 7);
	magic[7] = '\0';

	if (!(string("FRACTAL") == magic)) {
		throw logic_error("NOT VALID FRACTAL FILE");
	}

	metadata = MetaData(in);

	char type = in.get();

	if (type == 0) {
		this->type = T_GREYSCALE;
		channels.push_back(new TriangleTree(this->image, in, C_GREY));
	} else {
		this->type = T_COLOR;
		channels.push_back(new TriangleTree(this->image, in, C_RED));
		channels.push_back(new TriangleTree(this->image, in, C_GREEN));
		channels.push_back(new TriangleTree(this->image, in, C_BLUE));
	}
	if (outputVerbose()) {
		output << "Done loading fractal (" << getSize() << " triangles)." << endl;
	}
}

FractalImage::FractalImage(DoubleImage image, ImageType type) : type(type), image(image) {
	metadata.setWidth(image.getWidth());
	metadata.setHeight(image.getHeight());
	switch(type) {
	default:
	case T_GREYSCALE:
		channels.push_back(new TriangleTree(this->image, C_GREY));
		break;
	case T_COLOR:
		channels.push_back(new TriangleTree(this->image, C_RED));
		channels.push_back(new TriangleTree(this->image, C_GREEN));
		channels.push_back(new TriangleTree(this->image, C_BLUE));
		break;
	}
}

MetaData& FractalImage::getMetadata() {
	return metadata;
}

FractalImage::ImageType FractalImage::getType() const {
	return type;
}

const DoubleImage& FractalImage::getImage() const {
	return image;
}

void FractalImage::serialize(ostream& out) const {
	out << "FRACTAL";

	metadata.serialize(out);

	switch(type) {
	default:
	case T_GREYSCALE:
		out.put(0);
		break;
	case T_COLOR:
		out.put(1);
		break;
	}
	for (vector<TriangleTree*>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		(*it)->serialize(out);
	}
}

gdImagePtr FractalImage::decode(bool fixErrors) {
	gdImagePtr newImage = gdImageCreateTrueColor(image.getWidth(), image.getHeight());

	gdImageAlphaBlending(newImage, 0);
	gdImageSaveAlpha(newImage, 1);

	gdImageFill(newImage, 0, 0, ERROR_COLOR);
	for (std::vector<TriangleTree*>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		(*it)->renderTo(newImage, fixErrors);
	}
	return newImage;
}

void FractalImage::encode(double error) {
	Triangle* cur;
	for (vector<TriangleTree*>::size_type i = 0; i < channels.size(); i++) {
		if (outputVerbose()) {
			output << "Assigning channel " << channelToString(channels[i]->getChannel()) << endl;
		}
		while((cur = channels[i]->assignOne(error)) != NULL) {
			if (outputVerbose()) {
				output << "Triangle #" << cur->getId() << " (Area: " << cur->getArea() * image.getWidth() * image.getHeight() << "px)";
				output << " assigned - "<< (cur->isTerminal()?"Terminal":"Not Terminal") << endl;
				output << channels[i]->getUnassigned().size() << " unassigned / ";
				output << channels[i]->getAllTriangles().size() << " total. (";
				output << (((double)channels[i]->getUnassigned().size())/((double)channels[i]->getAllTriangles().size())*100) << "%)" << endl;
			}
		}
	}

}

void FractalImage::setImage(DoubleImage image) {
	this->image = image;
}

vector<Triangle*>::size_type FractalImage::getSize() const {
	vector<Triangle*>::size_type total = 0;
	for (vector<TriangleTree*>::const_iterator it = channels.begin(); it != channels.end(); it++) {
		total += (*it)->getAllTriangles().size();
	}
	return total;
}

FractalImage::~FractalImage() {
	for (vector<TriangleTree*>::iterator it = channels.begin(); it != channels.end(); it++) {
		delete *it;
	}
}

const vector<TriangleTree*>& FractalImage::getChannels() const {
	return channels;
}

void FractalImage::setSubdivisionMethod(TriangleTree::SubdivisionMethod sMethod) {
	for (vector<TriangleTree*>::iterator it = channels.begin(); it != channels.end(); it++) {
		(*it)->setSubdivisionMethod(sMethod);
	}
}
