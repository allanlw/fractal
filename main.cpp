#include <cstdio>
#include <iostream>
#include <fstream>
#include <sstream>
#include "gd.h"

#include "doubleimage.hpp"
#include "triangletree.hpp"
#include "constant.hpp"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc > 1) {
		FILE* lennaHandle = fopen(argv[1], "r");
		if (lennaHandle == NULL) {
			cout << "Could not load lenna.png, dying." << endl;
			return 1;
		}
		gdImagePtr lenna = gdImageCreateFromPng(lennaHandle);
		fclose(lennaHandle);

		DoubleImage image(lenna);
		TriangleTree tree(image);

		for (int i = 0; true; i++) {
			Triangle* cur = tree.assignOne();
			if (cur == NULL) break;
			cout << tree.getLastId();
			cout << " - "<< (cur->isTerminal()?"Terminal":"Not Terminal") << endl;
			if (cur->isTerminal()) {
				cout << " Error: " << cur->getTarget().error << endl;
			}
		}

		ofstream out("fractal.bin", ios_base::out | ios_base::trunc | ios_base::binary);
		cout << "Dumping Image..." << endl;
		tree.serialize(out);
		cout << "Image dumped." << endl;
		out.close();
	} else {
		cout << "loading fractal..." << endl; 
		ifstream in("fractal.bin", ios_base::in | ios_base::binary);

		FILE* noiseHandle = fopen("noise.png", "r");
		if (noiseHandle == NULL) {
			cout << "Could not load noise.png, dying." << endl;
			return 1;
		}
		gdImagePtr noise = gdImageCreateFromPng(noiseHandle);
		fclose (noiseHandle);

		DoubleImage img(noise);
		gdFree(noise);
		TriangleTree tree(img, in);

		cout << "fractal loaded. (" << tree.getAllTriangles()->size() << " triangles)" << endl;

		for (size_t i = 0; i < NUM_ITERATIONS; i++) {
			cout << "evaulating iteration #" << i << endl;
			tree.eval();
			ostringstream s;
			s << "e" << i << ".png";
			string fname = s.str();
			cout << "saving to " << fname << endl;
			FILE* oimg = fopen(fname.c_str(), "w");
			if (oimg == NULL) {
				cout << "Could not load " << fname << ", dying." << endl;
				return 1;
			}
			gdImagePng(tree.getImage().getImage(), oimg);
			fclose(oimg);
			cout << "iteration #" << i<<" done." << endl;
		}
	}
	return 0;
}
