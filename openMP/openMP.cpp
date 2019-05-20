// openMP.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>

using namespace std;

#define matr_print(A, m, n) \
{ \
  int i, j; \
  for(i = 0; i < m; i++) { \
  for(j = 0; j < n; j++)   { \
      printf("%.1lf\t", A[j+i*n]); \
    } \
    putchar('\n'); \
  } \
}

#define matrT_print(A, m, n) \
{ \
  int i, j; \
  for(j = 0; j < n; j++)   { \
  for(i = 0; i < m; i++) { \
      printf("%.1lf\t", A[j+i*n]); \
    } \
    putchar('\n'); \
  } \
}

int n, m;
double* A, * B, * C; /* матрица, вектор, результаты */

void load()
{
	cout << "Read size.txt" << endl;
	fstream fsize("D:/Projects/C++/OpenMP/openMP/openMP/Debug/size.txt");
	fsize >> n >> m;
	fsize.close();

	A = new double(n*m);
	B = new double(n);
	C = new double(n);

	cout << "Read A.txt" << endl;
	fstream fa("D:/Projects/C++/OpenMP/openMP/openMP/Debug/A.txt");
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			fa >> A[j * n + i];
		}
	}

	fa.close();

	cout << "Read B.txt" << endl;
	fstream fb("D:/Projects/C++/OpenMP/openMP/openMP/Debug/B.txt");

	for (int i = 0; i < n; i++) {
		fb >> B[i];
	}

	fb.close();
}

void print_all()
{
	printf("B:\n");
	matr_print(A, n, m);

	printf("B:\n");
	matr_print(B, n, 1);

	printf("C:\n");
	matr_print(C, n, 1);
}

void direct_pass()
{
	double x1, x2, x;
	for (int i = 1; i < n; i++) {
		for (int j = i; j < n; j++) {
			x1 = A[j * n + (i - 1)];
			x2 = A[(i - 1) * n + (i - 1)];
			x = -1.0 * x1 / x2;
#pragma omp parallel
			{

#pragma omp for
				for (int k = i; k < m; k++) {
					A[j * n + k] += A[(i - 1) * n + k] * x;
				}
			}

			for (int k = 0; k < i; k++) {
				A[j * n + k] = 0.0;
			}
			B[j] += B[i - 1] * x;
		}
	}
}

void reduct()
{
	double x;
	for (int i = 0; i < n; i++) {
		x = A[i * n + i];
		A[i * n + i] = 1.0;
#pragma omp parallel
		{
#pragma omp for
			for (int j = i + 1; j < m; j++) {
				A[i * n + j] /= x;
			}
		}
		B[i] /= x;
	}
}

void reverse_pass()
{
	for (int i = n - 1; i >= 0; i--) {
		C[i] = 0.0;
		for (int j = i; j < n - 1; j++) {
			C[i] += -1.0 * C[j + 1] * A[i * n + (j + 1)];
		}
		C[i] += B[i];
	}
}

void calc()
{
	direct_pass();
	reduct();
	reverse_pass();
}


int main()
{
	load();
	calc();
	print_all();
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
