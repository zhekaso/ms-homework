#include "mesh.h"

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
