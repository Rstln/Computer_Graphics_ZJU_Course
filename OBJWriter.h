#ifndef OBJWRITER_H
#define OBJWRITER_H

#include <vector>
#include <iostream>
#include <glm/glm.hpp>

class Point {
public:
	float x, y, z;
};

class UV {
public:
	float u, v;
};

class Normal {
public:
	float x, y, z;
};

class PointData {
public:
	int pointindex, uvindex, normalindex;
};

void ExportCube();

#endif // !OBJWRITER_H
