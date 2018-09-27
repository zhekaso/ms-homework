#include "mesh.h"
Point retMiddlePoint(Point A, Point B, int id) {
	Point res;
	res.id = id;
	res.x = (A.x + B.x) / 2.;
	res.y = (A.y + B.y) / 2.;
	return res;
}

Point MESH::findPoint(Point A, Point B){
	for (int i = 0; i < basic_functions.size(); i++)
		if (abs(basic_functions[i].OfMax.x - ((A.x + B.x) / 2.)) < 1e-7 &&
			abs(basic_functions[i].OfMax.y - ((A.y + B.y) / 2.)) < 1e-7)
			return basic_functions[i].OfMax;
	
}

Point MESH::returnPointFromId(int id){
	for (int i = 0; i < nPoints; i++)
		if (vertexes[i].id == id) return vertexes[i];
}

bool MESH::findEdgeInEdges(Edge pretender) {
	for (int i = 0; i < edges.size(); i++) {
		if (pretender.A.id == edges[i].A.id && pretender.B.id == edges[i].B.id ||
			pretender.A.id == edges[i].B.id && pretender.B.id == edges[i].A.id)
			return true;
	}
	return false;
}

void MESH::input() {
	FILE* file = fopen("coordinates.txt", "r");
	fscanf(file, "%d\n", &nPoints);
	vertexes.resize(nPoints);
	for (int i = 0; i < nPoints; i++) 
		fscanf(file, "%d %lf %lf\n", &vertexes[i].id, &vertexes[i].x, &vertexes[i].y);
	
	file = fopen("elems.txt", "r");
	fscanf(file, "%d\n", &nElems);
	trglGrid.resize(nElems);
	for (int i = 0; i < nElems; i++) 
		fscanf(file, "%d %d %d\n", &trglGrid[i].vertex[0], &trglGrid[i].vertex[1], &trglGrid[i].vertex[2]);
	
	
}

vector<int> MESH::returnTheBasicFuncOfFE(int fe_id) {
	vector <int> func;
	vector<Point> nodes;
	bool flag;

	for (int j = 0; j < 3; j++) {
		flag = false;
		for (int i = 0; i < vertexes.size && !flag; i++) {
			if (vertexes[i].id == trglGrid[fe_id - 1].vertex[j]) {
				nodes.push_back(vertexes[i]);
				flag = true;
			}
		}
	}
	nodes.push_back(findPoint(nodes[0], nodes[1]));
	nodes.push_back(findPoint(nodes[0], nodes[2]));
	nodes.push_back(findPoint(nodes[1], nodes[2]));

	for (int i = 0; i < nodes.size(); i++) {
		flag = false;
		for (int j = 0; j < basic_functions.size() && !flag; j++) {
			if (nodes[i].id == basic_functions[j].OfMax.id)
			{
				func.push_back(basic_functions[j].id);
				flag = true;

			}
		}
	}


	return func;
}

vector<int>  MESH::returnNumberFEIncludingEdge(int edge_id) {
	vector<int> FE;
	for (int i = 0; i < trglGrid.size(); i++) {
		if (trglGrid[i].vertex[0] == edges[edge_id].A.id ||
			trglGrid[i].vertex[1] == edges[edge_id].A.id ||
			trglGrid[i].vertex[2] == edges[edge_id].A.id)
			if (trglGrid[i].vertex[0] == edges[edge_id].B.id ||
				trglGrid[i].vertex[1] == edges[edge_id].B.id ||
				trglGrid[i].vertex[2] == edges[edge_id].B.id)
				FE.push_back(i + 1);
	}
	return FE;
}

bool  MESH::returnTheNumberOfEdge(int a_id, int b_id, int *edge_id) {
	for (int i = 0; i < edges.size(); i++)
		if (edges[i].A.id == a_id && edges[i].B.id == b_id ||
			edges[i].A.id == b_id && edges[i].B.id == a_id) {
			*edge_id = edges[i].edge_id;
			return true;
		}
	return false;
}

bool  MESH::returnPointsFormingAnEdge(int *a_id, int *b_id, int edge_id) {
	if (edge_id <= edges.size()) {
			for(int i=0;i<edges.size();i++)
				if (edges[i].edge_id == edge_id) {
					*a_id = edges[i].A.id;
					*b_id = edges[i].B.id;
					return true;
				}
	}
	else return false;
}

void MESH::buildAListOfEdges() {
	Edge tmp;
	int id = 1;
	for (int i = 0; i < nElems; i++) {
		tmp.A = returnPointFromId(trglGrid[i].vertex[0]);
		tmp.B = returnPointFromId(trglGrid[i].vertex[1]);
		if (!findEdgeInEdges(tmp)) {
			tmp.edge_id = id;
			edges.push_back(tmp);
			id++;
		}

		tmp.A = returnPointFromId(trglGrid[i].vertex[0]);
		tmp.B = returnPointFromId(trglGrid[i].vertex[2]);
		if (!findEdgeInEdges(tmp)) {
			tmp.edge_id = id;
			edges.push_back(tmp);
			id++;
		}

		tmp.A = returnPointFromId(trglGrid[i].vertex[1]);
		tmp.B = returnPointFromId(trglGrid[i].vertex[2]);
		if (!findEdgeInEdges(tmp)) {
			tmp.edge_id = id;
			edges.push_back(tmp);
			id++;
		}
	}
}

void  MESH::numerateBasicFunc() {
	BasicFunc ver;
	int cur_id = 1;
	int new_points_id = nPoints;
	for (int i = 0; i < edges.size(); i++)
	{
		//Первая точка на отрезке
		bool verWasNumberedA = false;
		for (int j = 0; !verWasNumberedA && j < basic_functions.size(); j++)
			if (edges[i].A.id == basic_functions[j].OfMax.id)
				verWasNumberedA = true;
		if (!verWasNumberedA) 
		{ 
			ver.OfMax = edges[i].A;
			ver.id = cur_id;
			cur_id++;
			basic_functions.push_back(ver);
		}

		//вторая точка на отрезке
		bool verWasNumberedB = false;
		for (int j = 0; !verWasNumberedB && j < basic_functions.size(); j++)
			if (edges[i].B.id == basic_functions[j].OfMax.id)
				verWasNumberedB = true;
		if (!verWasNumberedB)
		{
			ver.OfMax = edges[i].B;
			ver.id = cur_id;
			cur_id++;
			basic_functions.push_back(ver);
		}

		//срединная точка отрезка
		if (!verWasNumberedA || !verWasNumberedB) {
			ver.OfMax = retMiddlePoint(edges[i].A, edges[i].B, new_points_id);
			ver.id = cur_id;
			basic_functions.push_back(ver);
			cur_id++;
			new_points_id++;
		}
	}
}
