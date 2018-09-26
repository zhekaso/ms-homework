#include "mesh.h"

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
