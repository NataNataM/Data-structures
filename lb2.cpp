#include <iostream>
#include <complex>
#include <math.h>
#include <mkl.h>
#include <windows.h>
#include <limits.h>
#include <omp.h>
using namespace std;

//создание двумерного динамического массива
complex<float>** create_matrix(int n) {
    complex<float>** a = new complex<float>*[n];
    complex<float>* aa = new complex<float>[n * n];
    for (int i = 0; i < n; i++) {
        a[i] = aa;
        aa += n;
    }
    return a;
}
 
//переумножение матриц 1 способом
void mult1_matrix(complex<float>** A, complex<float>** B, complex<float>** C, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            complex<float> sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

//транспонирование матрицы для 3 способа
void trans_matrix(complex<float>** a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            complex<float> t = a[i][j];
            a[i][j] = a[j][i];
            a[j][i] = t;
        }
    }
}

//переумножение матриц 3 способом
void mult3_matrix(complex<float>** A, complex<float>** B, complex<float>** C, int n) {
#pragma omp parallel for
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            complex<float> sum = 0;
            for (int k = 0; k < n; ++k) {
                sum += A[i][k] * B[j][k];
            }
            C[i][j] = sum;
        }
    }
}

//сравнение матриц
bool sravn_matrix(complex<float>** a, complex<float>** b, int n) {
    float epsilon = 1e-2f;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (fabs(a[i][j].real() - b[i][j].real()) > epsilon || fabs(a[i][j].imag() - b[i][j].imag()) > epsilon) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    setlocale(LC_ALL, "Russian");
    const int N = 2048;
    complex<float>** a = create_matrix(N);
    complex<float>** b = create_matrix(N);
    complex<float>** c1 = create_matrix(N);
    complex<float>** c2 = create_matrix(N);
    complex<float>** c3 = create_matrix(N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            a[i][j] = complex<float>((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
            b[i][j] = complex<float>((float)rand() / RAND_MAX, (float)rand() / RAND_MAX);
        }
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            c1[i][j] = complex<float>(0.0, 0.0);
            c2[i][j] = complex<float>(0.0, 0.0);
            c3[i][j] = complex<float>(0.0, 0.0);
        }

    //1 способ
    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    mult1_matrix(a, b, c2, N);
    QueryPerformanceCounter(&end);
    double t_count = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    double p = 2.0 * pow(N, 3) / t_count * pow(10, -6);
    cout << "1 способ:" << endl;
    cout << "Время, затраченное на умножение матриц: " << t_count << " секунд.\n";
    cout << "Производительность в MFlops: " << p << endl << endl;

    //2 способ
    float alpha[] = { 1.0, 0.0 };
    float beta[] = { 0.0, 0.0 };
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, N, N, alpha, a[0], N, b[0], N, beta, c1[0], N);
    QueryPerformanceCounter(&end);
    t_count = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    p = 2.0 * pow(N, 3) / t_count * pow(10, -6);
    cout << "2 способ:" << endl;
    cout << "Время, затраченное на умножение матриц: " << t_count << " секунд.\n";
    cout << "Производительность в MFlops: " << p << endl << endl;

    //3 способ
    trans_matrix(b, N);
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    mult3_matrix(a, b, c3, N);
    QueryPerformanceCounter(&end);
    t_count = (double)(end.QuadPart - start.QuadPart) / freq.QuadPart;
    p = 2.0 * pow(N, 3) / t_count * pow(10, -6);
    cout << "3 способ:" << endl;
    cout << "Время, затраченное на умножение матриц: " << t_count << " секунд.\n";
    cout << "Производительность в MFlops: " << p << endl << endl;

    //сравнение матриц 
    if (sravn_matrix(c3, c2, N) && sravn_matrix(c1, c2, N) && sravn_matrix(c3, c1, N))
        cout << "Матрицы c1, с2 и с3 равны.\n";
    else
        cout << "Матрицы c1, с2 и с3 не равны!\n";

    cout << "Выполнила: Мангер Наталья Александровна, группа 090304-РПИа-о24\n";
    return 0;
}
