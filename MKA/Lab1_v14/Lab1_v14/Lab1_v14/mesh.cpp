#include "mesh.h"

void MESH::input() {
	FILE* file = fopen("input.txt", "r");
	fscanf(file, "r = %lf R = %lf\n", &r, &R);
	int sideN, sideM;
	fscanf(file, "n = %d kn = %lf ns = %i\n", &n, &kn, &sideN);
	fscanf(file, "m = %d km = %lf ms = %i\n", &m, &km, &sideM);
	ns = sideN;
	ms = sideM;
}
void MESH::calculateTheCoordinatesOfTheVertexes() {
	int id = 1;
	Point A;
	double yStep = (R - r) / m; 
	double nAngle = 90 / n;
	for (int j = 0; j <= m; j++) {
		double cR = r + j * yStep;
		for (int i = 0; i <= n; i++,id++) {
			
			double aop = i * nAngle* M_PI/ 180.0;
			double sn = sin(aop);
			double cs = cos(aop);

			A.id = id;
			A.x = cs * cR;
			A.y = sn * cR;

			vertexes.push_back(A);
		}
	}
	nPoints = vertexes.size();
}
void MESH::buildingRectangularGrid() {
	Ractangle tmp;
	int id = 1;
	for (int j = 0; j < m; j++,id++) {
		for (int i = 0; i < n; i++,id++) {
			tmp.vertex[0] = id;
			tmp.vertex[1] = id + 1;
			tmp.vertex[2] = n + 1 + id;
			tmp.vertex[3] = (n+1) + id + 1;
			rctglGrig.push_back(tmp);
		}
	}
	nElems = 2 * rctglGrig.size();
	trglGrid.resize(nElems);
}

void MESH::convertRectangleToTriangle() {
	Triangle tmp;
	for (int i = 0,j=0; i < rctglGrig.size(); i = i++,j=j+2) {
		tmp.vertex[0] = rctglGrig[i].vertex[0];
		tmp.vertex[1] = rctglGrig[i].vertex[1];
		tmp.vertex[2] = rctglGrig[i].vertex[2];
		trglGrid[j]=tmp;
		tmp.vertex[0] = rctglGrig[i].vertex[2];
		tmp.vertex[1] = rctglGrig[i].vertex[3];
		tmp.vertex[2] = rctglGrig[i].vertex[1];
		trglGrid[j+1]=tmp;
	}
}

void MESH::output() {
	convertRectangleToTriangle();
	writeMaterialInfo();
	writeElemInfo();
	writeCooordInfo();
	writeKuzlovInfo();
}
void MESH::writeElemInfo() {

	//vector<int> tmp; tmp.resize(n_el*14);
	size_t t;
	int *tmp = new int[nElems * 7];
	for (int i = 0, k = 0; k < nElems/2; k++, i = i + 14) {
		for (int j = 0; j < 4; j++)
			tmp[i + j] = rctglGrig[k].vertex[j];
		for (int j = 4; j < 8; j++)
			tmp[i + j] = rctglGrig[k].vertex[j] + nElems;
	}

	//прежде следует вычленить информацию
	FILE *fout;
	fopen_s(&fout, "nver.dat", "wb");
	t = fwrite(tmp, sizeof(int), nElems * 7, fout);
	fclose(fout);
}
void MESH::writeMaterialInfo() {

	int *tmp = new int[nElems/2];
	size_t t;
	for (int i = 0; i < nElems/2; i++)	tmp[i] = 1;
	FILE *fout;
	fopen_s(&fout, "nvkat.dat", "wb");
	t = fwrite(tmp, sizeof(int), nElems/2, fout);
	fclose(fout);

}
void MESH::writeCooordInfo() {

	double *tmp = new double[6 * nPoints];
	size_t t;
	int i,k;
	for (i = 0, k = 0; k < nPoints; k++, i = i + 3) {
		tmp[i] = vertexes[k].x;
		tmp[i + 1] = vertexes[k].y;
		tmp[i + 2] = 0;
	}
	for ( k = 0; k < nPoints; k++, i = i + 3) {
		tmp[i] = vertexes[k].x;
		tmp[i + 1] = vertexes[k].y;
		tmp[i + 2] = 1;
	}
	FILE *fout;
	fopen_s(&fout, "xyz.dat", "wb");
	t = fwrite(tmp, sizeof(double), 6 * nPoints, fout);
	fclose(fout);

}
void MESH::writeKuzlovInfo() {

	FILE *b = fopen("inftry.dat", "w");
	fprintf(b, " ISLAU=       0 INDKU1=       0 INDFPO=       0\nKUZLOV=%8d   KPAR=%8d    KT1=       0   KTR2=       0   KTR3=       0\nKISRS1=       0 KISRS2=       0 KISRS3=       0   KBRS=       0\n   KT7=       0   KT10=       0   KTR4=       0  KTSIM=       0\n   KT6=       0\n",2*nPoints, nElems/2);
	fclose(b);

}