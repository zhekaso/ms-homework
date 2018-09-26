#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
	int id;
	double x, y;
};

struct Triangle {
	int vertex[3];
};

class MESH {
public:
	void input();
protected:
	int nPoints, nElems; //кол-во вершин и элементов
	
	vector <Point> vertexes;
	vector <Triangle> trglGrid;


};
