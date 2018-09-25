#pragma once
#include <vector>
#include <functional>

using namespace std;

struct Point
{
	double x;
	double y;
};

struct Elem
{
	int p[4];
};

struct Grid {
	vector<Point> points;
	vector<Elem> elems;
};

struct Matrix
{
	vector<double> gg;
	vector<double> di;
	vector<int> ig;
	vector<int> jg;
};

class SmoothingSpline2D
{
private:
	vector<double> q;
	Grid grid;
	function<double(double)> basis_1D[4];
	static double f1(double ksi);
	static double f2(double ksi);
	static double f3(double ksi);
	static double f4(double ksi);
	inline int index_converter_for_x(int i);
	inline int index_converter_for_y(int i);
	void make_local(double Local[16][16], double local_b[16], int elem_idx, Grid &grid, vector<Point> &XY, vector<double> &f, vector<int> &points_idx, vector<double> &w, double alpha, double beta);
	void make_localS(int elem_idx, Grid &grid, double LocalS[16][16], vector<Point> &XY, vector<int> &points_idx, vector<double> &w);
	void make_localA(int elem_idx, Grid &grid, double LocalA[16][16], double a);
	void make_localB(int elem_idx, Grid &grid, double LocalB[16][16], double b);
	void make_local_b(int elem_idx, Grid &grid, double local_b[16], vector<Point> &XY, vector<double> f, vector<int> &points_idx, vector<double> &w);
	void add_local_to_global(Elem elem, Matrix &A, vector<double> &b, double Local[16][16], double local_b[16]);
public:
	void SetSpline(Grid &grid, vector<double> &q);
	int MakeSpline(vector<Point> &XY, vector<double> &f, vector<double> &w, Grid &grid, vector<double> alpha, vector<double> beta);
	double GetFuncValue(double x, double y);
	SmoothingSpline2D();
	~SmoothingSpline2D();
};

