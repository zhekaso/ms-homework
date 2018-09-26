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

struct Edge {
	int edge_id;
	Point A, B;
};

class MESH {
public:
	// ���������� �������� �����
	void input();
	
	vector<int> returnTheBasicFuncOfFE(int fe_id);	//�� ������ �� ���������� ������ �������� � ���� �������� �-���
	
	vector<int> returnNumberFEIncludingEdge(int edge_id);	//�� ������ ����� ���������� ������ ��, � ������� ������ ������ �����
	
	bool returnTheNumberOfEdge(int a_id,int b_id,int *edge_id);	//�� ���� ������� ������ ������������ ����� �����
	
	bool returnPointsFormingAnEdge(int *a_id, int *b_id, int edge_id);	//�� ���� ������� ������ ������������ ����� �����
protected:
	int nPoints, nElems; //���-�� ������ � ���������
	
	vector <Point> vertexes;
	vector <Triangle> trglGrid;
	vector <Edge> edges;

	void buildAListOfEdges();
	Point returnPointFromId(int id);
	bool findEdgeInEdges(Edge A);


};
