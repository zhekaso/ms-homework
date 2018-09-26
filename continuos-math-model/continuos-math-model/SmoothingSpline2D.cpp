#include "SmoothingSpline2D.h"

#include <map>
using namespace std;

int find_matrix_elem_idx(Matrix &A, int i, int j)
{
	if (j > i)
	{
		int temp_swap;
		temp_swap = i;
		i = j;
		j = temp_swap;
	}

	for (int k = A.ig[i]; k < A.ig[i + 1]; k++)
	{
		int col = A.jg[k];

		if (j == col)
		{
			return k;
		}
	}
}

void SmoothingSpline2D::SetSpline(Grid & grid, vector<double>& q)
{
	this->grid = grid;
	this->q = q;
}

int SmoothingSpline2D::MakeSpline(vector<Point>& XY, vector<double>& f, vector<double>& w, Grid & grid, vector<double> alpha, vector<double> beta)
{
	map<int, vector<int>> points_in_element;
	Matrix A;
	vector<double> b;

	double Local[16][16];
	double local_b[16];

	this->grid.elems = grid.elems;
	this->grid.points = grid.points;

	for (int i = 0; i < XY.size(); i++)
	{
		for (int j = 0; j < grid.elems.size(); j++)
		{
			if (XY[i].x >= grid.points[grid.elems[j].p[0]].x && XY[i].x <= grid.points[grid.elems[j].p[3]].x)
				if (XY[i].y >= grid.points[grid.elems[j].p[0]].y && XY[i].y <= grid.points[grid.elems[j].p[3]].y)
				{
					points_in_element[j].push_back(i);
					break;
				}
		}
	}

	for (int ix = 0; ix < grid.elems.size(); ix++)
	{
		make_local(Local, local_b, ix, grid, XY, f, points_in_element[ix], w, alpha[ix], beta[ix]);
		add_local_to_global(grid.elems[ix], A, b, Local, local_b);
	}

	//Solver_part

	return 0;
}

double SmoothingSpline2D::GetFuncValue(double x, double y)
{
	double val = 0;
	for (auto elem : grid.elems)
	{
		if(x >= grid.points[elem.p[0]].x && x <= grid.points[elem.p[4]].x)
			if (y >= grid.points[elem.p[0]].y && y <= grid.points[elem.p[4]].y)
			{
				double ksi_x = (x - grid.points[elem.p[0]].x)
					/ (grid.points[elem.p[3]].x - grid.points[elem.p[0]].x);
				double ksi_y = (y - grid.points[elem.p[0]].y)
					/ (grid.points[elem.p[3]].y - grid.points[elem.p[0]].y);

				for (int ix = 0; ix < 16; ix++)
				{
					int x_func_idx = index_converter_for_x(ix);
					int y_func_idx = index_converter_for_x(ix);

					double basis_func_ix_val = basis_1D[x_func_idx](ksi_x) * basis_1D[y_func_idx](ksi_y);

					val += q[4 * elem.p[ix / 4] + ix % 4] * basis_func_ix_val;
				}
				break;
			}
	}

	return val;
}

SmoothingSpline2D::SmoothingSpline2D()
{
	basis_1D[0] = f1;
	basis_1D[1] = f2;
	basis_1D[2] = f3;
	basis_1D[3] = f4;
}


SmoothingSpline2D::~SmoothingSpline2D()
{
}

inline double sqr(double x) { return x * x; }
inline double cube(double x) { return x * x * x; }

double SmoothingSpline2D::f1(double ksi)
{
	return 1 - 3 * sqr(ksi) + 2 * cube(ksi);;
}

double SmoothingSpline2D::f2(double ksi)
{
	return ksi - 2 * sqr(ksi) + cube(ksi);
}

double SmoothingSpline2D::f3(double ksi)
{
	return 3 * sqr(ksi) - 2 * cube(ksi);
}

double SmoothingSpline2D::f4(double ksi)
{
	return -sqr(ksi) + cube(ksi);
}

inline int SmoothingSpline2D::index_converter_for_x(int i)
{
	return 2 * ((i / 4) % 2) + i % 2;
}

inline int SmoothingSpline2D::index_converter_for_y(int i)
{
	return 2 * (i / 8) + (i / 2) % 2;
}

void SmoothingSpline2D::make_local(double Local[16][16], double local_b[16], int elem_idx, Grid & grid, vector<Point>& XY, vector<double>& f, vector<int>& points_idx, vector<double>& w, double alpha, double beta)
{
	double LocalS[16][16];
	double LocalA[16][16];
	double LocalB[16][16];

	make_localS(elem_idx, grid, LocalS, XY, points_idx, w);
	make_localA(elem_idx, grid, LocalA, alpha);
	make_localB(elem_idx, grid, LocalB, beta);

	make_local_b(elem_idx, grid, local_b, XY, f, points_idx, w);

	for (int i = 0; i < 16; i++)
		for (int j = 0; j < 16; j++)
			Local[i][j] = LocalS[i][j] + LocalA[i][j] + LocalB[i][j];
}


void SmoothingSpline2D::make_localS(int elem_idx, Grid & grid, double LocalS[16][16], vector<Point>& XY, vector<int>& points_idx, vector<double> &w)
{
	for (int i = 0; i < 16; i++)
	{
		int x_func_idx_i = index_converter_for_x(i);
		int y_func_idx_i = index_converter_for_y(i);

		for (int j = 0; j < 16; j++)
		{
			int x_func_idx_j = index_converter_for_x(j);
			int y_func_idx_j = index_converter_for_y(j);

			LocalS[i][j] = 0;
			for (int k : points_idx)
			{
				double ksi_x = (XY[k].x - grid.points[grid.elems[elem_idx].p[0]].x)
					/ (grid.points[grid.elems[elem_idx].p[3]].x - grid.points[grid.elems[elem_idx].p[0]].x);
				double ksi_y = (XY[k].y - grid.points[grid.elems[elem_idx].p[0]].y)
					/ (grid.points[grid.elems[elem_idx].p[3]].y - grid.points[grid.elems[elem_idx].p[0]].y);

				double basis_func_val_i = basis_1D[x_func_idx_i](ksi_x) * basis_1D[y_func_idx_i](ksi_y);
				double basis_func_val_j = basis_1D[x_func_idx_j](ksi_x) * basis_1D[y_func_idx_j](ksi_y);

				LocalS[i][j] += w[k] * basis_func_val_i * basis_func_val_j;
			}
		}
	}
}

void SmoothingSpline2D::make_localA(int elem_idx, Grid & grid, double LocalA[16][16], double a)
{
	double G_1D[4][4];
	double M_1D[4][4];

	double h = grid.points[grid.elems[elem_idx].p[3]].x - grid.points[grid.elems[elem_idx].p[0]].x;

#define SET_SYM_MATRIX_ELEM(M, i, j, val) M[i][j] = M[j][i] = val
	G_1D[0][0] = 36;
	G_1D[1][1] = 4 * sqr(h);
	G_1D[2][2] = 36;
	G_1D[3][3] = 4 * sqr(h);
	SET_SYM_MATRIX_ELEM(G_1D, 1, 0, 3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 2, 0, -36);
	SET_SYM_MATRIX_ELEM(G_1D, 2, 1, -3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 3, 0, 3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 3, 1, -sqr(h));
	SET_SYM_MATRIX_ELEM(G_1D, 3, 2, -3 * h);

	M_1D[0][0] = 156;
	M_1D[1][1] = 4 * sqr(h);
	M_1D[2][2] = 156;
	M_1D[3][3] = 4 * sqr(h);
	SET_SYM_MATRIX_ELEM(M_1D, 1, 0, 22 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 2, 0, 54);
	SET_SYM_MATRIX_ELEM(M_1D, 2, 1, 13 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 3, 0, -13 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 3, 1, -3 * sqr(h));
	SET_SYM_MATRIX_ELEM(M_1D, 3, 2, -22 * h);
#undef SET_SYM_MATRIX_ELEM

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			G_1D[i][j] *= 1.0 / (30 * h);
			M_1D[i][j] *= h / 420.0;
		}

	for (int i = 0; i < 16; i++)
	{
		int x_idx_i = index_converter_for_x(i);
		int y_idx_i = index_converter_for_y(i);
		for (int j = 0; j < 16; j++)
		{
			int x_idx_j = index_converter_for_x(j);
			int y_idx_j = index_converter_for_y(j);

			LocalA[i][j] = a * (G_1D[x_idx_i][x_idx_j] * M_1D[y_idx_i][y_idx_j] + M_1D[x_idx_i][x_idx_j] * G_1D[y_idx_i][y_idx_j]);
		}
	}

}

void SmoothingSpline2D::make_localB(int elem_idx, Grid & grid, double LocalB[16][16], double b)
{
	double SecondDerivative_1D[4][4];
	double G_1D[4][4];
	double M_1D[4][4];

	double h = grid.points[grid.elems[elem_idx].p[3]].x - grid.points[grid.elems[elem_idx].p[0]].x;

#define SET_SYM_MATRIX_ELEM(M, i, j, val) M[i][j] = M[j][i] = val
	G_1D[0][0] = 36;
	G_1D[1][1] = 4 * sqr(h);
	G_1D[2][2] = 36;
	G_1D[3][3] = 4 * sqr(h);
	SET_SYM_MATRIX_ELEM(G_1D, 1, 0, 3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 2, 0, -36);
	SET_SYM_MATRIX_ELEM(G_1D, 2, 1, -3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 3, 0, 3 * h);
	SET_SYM_MATRIX_ELEM(G_1D, 3, 1, -sqr(h));
	SET_SYM_MATRIX_ELEM(G_1D, 3, 2, -3 * h);

	M_1D[0][0] = 156;
	M_1D[1][1] = 4 * sqr(h);
	M_1D[2][2] = 156;
	M_1D[3][3] = 4 * sqr(h);
	SET_SYM_MATRIX_ELEM(M_1D, 1, 0, 22 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 2, 0, 54);
	SET_SYM_MATRIX_ELEM(M_1D, 2, 1, 13 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 3, 0, -13 * h);
	SET_SYM_MATRIX_ELEM(M_1D, 3, 1, -3 * sqr(h));
	SET_SYM_MATRIX_ELEM(M_1D, 3, 2, -22 * h);

	SecondDerivative_1D[0][0] = 12;
	SecondDerivative_1D[1][1] = 4 * sqr(h);
	SecondDerivative_1D[2][2] = 12;
	SecondDerivative_1D[3][3] = 4 * sqr(h);
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 1, 0, 6 * h);
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 2, 0, -12);
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 2, 1, -6 * h);
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 3, 0, 6 * h);
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 3, 1, 2 * sqr(h));
	SET_SYM_MATRIX_ELEM(SecondDerivative_1D, 3, 2, -6 * h);
#undef SET_SYM_MATRIX_ELEM

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			G_1D[i][j] *= 1.0 / (30 * h);
			M_1D[i][j] *= h / 420.0;
			SecondDerivative_1D[i][j] *= 1.0 / cube(h);
		}

	for (int i = 0; i < 16; i++)
	{
		int x_idx_i = index_converter_for_x(i);
		int y_idx_i = index_converter_for_y(i);
		for (int j = 0; j < 16; j++)
		{
			int x_idx_j = index_converter_for_x(j);
			int y_idx_j = index_converter_for_y(j);

			LocalB[i][j] = b * (SecondDerivative_1D[x_idx_i][x_idx_j] * M_1D[y_idx_i][y_idx_j] + M_1D[x_idx_i][x_idx_j] * SecondDerivative_1D[y_idx_i][y_idx_j]);
			LocalB[i][j] += b * G_1D[x_idx_i][x_idx_j] * G_1D[y_idx_i][y_idx_j];
		}
	}
}

void SmoothingSpline2D::make_local_b(int elem_idx, Grid &grid, double local_b[16], vector<Point> &XY, vector<double> f, vector<int> &points_idx, vector<double> &w)
{
	for (int i = 0; i < 16; i++)
	{
		int x_func_idx = index_converter_for_x(i);
		int y_func_idx = index_converter_for_y(i);
		local_b[i] = 0;
		for (int j : points_idx)
		{
			double ksi_x = (XY[j].x - grid.points[grid.elems[elem_idx].p[0]].x) 
			                                    / (grid.points[grid.elems[elem_idx].p[3]].x - grid.points[grid.elems[elem_idx].p[0]].x);
			double ksi_y = (XY[j].y - grid.points[grid.elems[elem_idx].p[0]].y)
			                                    / (grid.points[grid.elems[elem_idx].p[3]].y - grid.points[grid.elems[elem_idx].p[0]].y);
			double basis_func_val = basis_1D[x_func_idx](ksi_x) * basis_1D[y_func_idx](ksi_y);

			local_b[i] += w[j] * basis_func_val * f[j];
		}
	}
}

void SmoothingSpline2D::add_local_to_global(Elem elem, Matrix & A, vector<double>& b, double Local[16][16], double local_b[16])
{
	for (int i = 0; i < 16; i++)
	{
		int i_global = 4 * elem.p[i / 4] + i % 4;

		for (int j = 0; j < i; j++)
		{
			int j_global = 4 * elem.p[j / 4] + j % 4;
			int matrix_elem_idx = find_matrix_elem_idx(A, i_global, j_global);

			A.gg[matrix_elem_idx] += Local[i][j];
		}

		A.di[i_global] += Local[i][i];
		b[i_global] += local_b[i];
	}
}
