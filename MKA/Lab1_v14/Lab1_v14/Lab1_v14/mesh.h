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

struct Ractangle {
	int vertex[4];
};

struct Triangle {
	int vertex[3];
};

class MESH {
public:
	void input();
	void calculateTheCoordinatesOfTheVertexes();
	void buildingRectangularGrid();
	void output();
protected:
	// Параметры, определяющие сетку
	int n, m; // кол-во разбиений по Y и X
	double r, R; // внешний и внутренний радиус
	double kn, km; // коэффициенты разрядки
	bool ns, ms; // 1 - разрядка снизу 0 - разрядка сверху

	int nPoints, nElems; //кол-во вершин и элементов
	
	vector <Point> vertexes;
	vector <Ractangle> rctglGrig;
	vector <Triangle> trglGrid;
	void convertRectangleToTriangle();
	//Вывод в файлы
	void writeKuzlovInfo();
	void writeCooordInfo();
	void writeMaterialInfo();
	void writeElemInfo();

};
