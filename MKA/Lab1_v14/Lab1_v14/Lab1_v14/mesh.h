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
	// ���������, ������������ �����
	int n, m; // ���-�� ��������� �� Y � X
	double r, R; // ������� � ���������� ������
	double kn, km; // ������������ ��������
	bool ns, ms; // 1 - �������� ����� 0 - �������� ������

	int nPoints, nElems; //���-�� ������ � ���������
	
	vector <Point> vertexes;
	vector <Ractangle> rctglGrig;
	vector <Triangle> trglGrid;
	void convertRectangleToTriangle();
	//����� � �����
	void writeKuzlovInfo();
	void writeCooordInfo();
	void writeMaterialInfo();
	void writeElemInfo();

};
