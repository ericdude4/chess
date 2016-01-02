#include <fstream>
#include <vector>
#include <iostream>
#include <string.h>
#include "vertex.h"
#include "face.h"
#include "normal.h"

class objParser {
private:
	std::vector<vertex> vertices;
	std::vector<normal> normals;
	std::vector<face> faces;
	std::ifstream obj_file;

	void throwError() {
		std::cout << "Error: you must parse file before retrieving data" << std::endl;
	}

public:
	objParser(){};

	int setFile(char file[]) {
		if (obj_file.is_open())
			obj_file.close();
		obj_file.open(file);
		//make sure the file exists
		if (!obj_file.good()) {
			std::cout << "file error" << std::endl;
			return 1;
		}
	}

	std::vector<vertex> getVertices() {
		if (vertices.size() > 0)
			return vertices;
		else {
			throwError();
		}
	}

	std::vector<normal> getNormals() {
		if (normals.size() > 0)
			return normals;
		else {
			throwError();
		}
	}

	std::vector<face> getFaces() {
		if (faces.size() > 0)
			return faces;
		else {
			throwError();
		}
	}

	void parseDatFile() {
		vertices.clear();
		faces.clear();
		normals.clear(); 
		while ( !obj_file.eof() ) {
			char buffer[256];
			obj_file.getline(buffer, 256);	//assign a line to the buffer

			char* token[9] = {};	//to store individual words for parsing
			token[0] = strtok(buffer, " ");

			if (token[0]) {
				if (token[0][0] == 'v' && token[0][1] != 'n') {		//vertex
					float x, y, z;
					token[1] = strtok(0, " ");
					x = atof (token[1]);
					token[2] = strtok(0, " ");
					y = atof (token[2]);
					token[3] = strtok(0, " ");
					z = atof (token[3]);
					//std::cout << "vertex: " << x << y << z << std::endl;
					vertex v; v.setXYZ(x, y, z);
					vertices.push_back(v);
				}
				if (token[0][0] == 'v' && token[0][1] == 'n') {		//normal
					float i, j, k;
					token[1] = strtok(0, " ");
					i = atof (token[1]);
					token[2] = strtok(0, " ");
					j = atof (token[2]);
					token[3] = strtok(0, " ");
					k = atof (token[3]);
					//std::cout << "normal: " << i << j << k << std::endl;
					normal n; n.setIJK(i, j, k);
					normals.push_back(n);
				}
				if (token[0][0] == 'f') {		//face (the -1 is b/c .obj file uses 1 based indexing)
					int a1, b1, a2, b2, a3, b3;
					token[1] = strtok(0, "/ ");
					a1 = atoi (token[1]) - 1;
					token[2] = strtok(0, "/ ");
					b1 = atoi (token[2]) - 1;

					token[3] = strtok(0, "/ ");
					a2 = atoi (token[3]) - 1;
					token[4] = strtok(0, "/ ");
					b2 = atoi (token[4]) - 1;

					token[5] = strtok(0, "/ ");
					a3 = atoi (token[5]) - 1;
					token[6] = strtok(0, "/ ");
					b3 = atoi (token[6]) - 1;
					//std::cout << "face: " << a1 << b1 << a2 << b2 << a3 << b3 << std::endl;
					face f; f.setAB(a1, b1, a2, b2, a3, b3);
					faces.push_back(f);
				}
			}
		}
	}
};