#include "mesh.h"

void main()
{
	MESH disk;
	disk.input();
	disk.calculateTheCoordinatesOfTheVertexes();
	disk.buildingRectangularGrid();
	disk.output();
}