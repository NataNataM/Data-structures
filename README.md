# Data-structures
## Выполнила: Мангер Наталья, группа 090304-РПИа-о24
### Лабораторная работа №1
#### Задание:
> Пусть даны числа a и n. Обратным элементом к числу a в кольце вычетов по модулю n называется такое число b, что ab=1 (mod n), то есть ab дает остаток 1 при делении на n.
#### Входные данные:
> Даны числа a и n.
#### Выходные данные:
> Введите значение обратного элемента к числу a в кольце вычетов по модулю n. Если обратного элемента не существует, выведите число 0.
#### Примеры:
> **Входные данные:** 2 3
> 
> **Выходные данные:** 2
```cpp
#include <iostream>
using namespace std;

//алгоритм Евклида
int GCD(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int gcd = GCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return gcd;
}

//обратный алгоритм Евклида
int Inverse_GCD(int a, int n) {
    int x, y;
    int gcd = GCD(a, n, x, y);
    if (gcd != 1) {
        return 0;
    }
    int result = (x % n + n) % n;
    return result;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int a, n;
    cout << "Введите числа a и n: ";
    cin >> a >> n;
    int inverse = Inverse_GCD(a, n);
    if (inverse == 0) {
        cout << "Обратный элемент не существует." << endl;
    }
    else {
        cout << "Обратный элемент: " << inverse << endl;
    }
    cout << "Выполнила: Мангер Наталья Александровна, группа 090304-РПИа-о24\n";
    return 0;
}
```
### Лабораторная работа №2
#### Задание:
> Перемножить 2 квадратные матрицы размера 2048x2048 с элементами типа double complex (комплексное число двойной точности). Исходные матрицы генерируются в программе (случайным образом либо по определенной формуле) либо считываются из заранее подготовленного файла.
> 
> Оценить сложность алгоритма по формуле c = 2 n3, где n - размерность матрицы.
> 
> Оценить производительность в MFlops, p = c/t*10-6, где t - время в секундах работы алгоритма.
> 
> Выполнить 3 варианта перемножения и их анализ и сравнение:
> 
> 1-й вариант перемножения - по формуле из линейной алгебры.
> 
> 2-й вариант перемножения - результат работы функции cblas_zgemm из библиотеки BLAS (рекомендуемая реализация из Intel MKL)
> 
> 3-й вариант перемножения - оптимизированный алгоритм по вашему выбору, написанный вами, производительность должна быть не ниже 30% от 2-го варианта
> 
```cpp
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
```
