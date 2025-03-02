#include <iostream>
#include <complex>
#include <math.h>
#include <mkl.h>
#include <windows.h>
using namespace std;

//создание двумерного динамического массива
complex<float>** create_matrix(int n){
    complex<float>** a = new complex<float>*[n];
    complex<float>* aa = new complex<float>[n * n];
    for (int i = 0; i < n; i++) {
        a[i] = aa;
        aa += n;
    }
    return a;
}

//переумножение матриц 1 способом
void mult1_matrix(complex<float>** a, complex<float>** b, complex<float>** с, int n){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            с[i][j] = complex<float>(0.0, 0.0);
            for (int k = 0; k < n; ++k) {
                с[i][j] += a[i][k] * b[j][k];
            }
        }
    }
}

//транспонирование матрицы для 3 способа
void trans_matrix(complex<float>** a, int n) {
    complex<float>** _a = create_matrix(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            _a[i][j] = a[j][i];
        }
    }
}

//переумножение матриц 3 способом
void mult3_matrix(complex<float>** a, complex<float>** b, complex<float>** с, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
                с[i][j] += a[i][j] * b[i][j];
        }
    }
}

int main(){
    setlocale(LC_ALL, "Russian");
    const int N = 2048;
    complex<float>** a = create_matrix(N);
    complex<float>** b = create_matrix(N);
    complex<float>** c1 = create_matrix(N);
    complex<float>** c2 = create_matrix(N);
    complex<float>** c3 = create_matrix(N);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j) {
            a[i][j] = complex<float>((double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
            b[i][j] = complex<float>((double)rand() / RAND_MAX, (double)rand() / RAND_MAX);
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
    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasTrans, N, N, N, alpha, a[0], N, b[0], N, beta, c1[0], N);
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
    
    cout << "Выполнила: Мангер Наталья Александровна, группа 090304-РПИа-о24\n";
    return 0;
}