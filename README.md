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
```c
#include <stdio.h>
#include <locale.h>

void evklid(unsigned int a, int b, int *x, int *y, int *d) {
  int q, r, x1=0, x2=1, y1=1, y2=0;
  while (b > 0) {
    q = a / b;
    r = a - q * b;
    *x = x2 - q * x1;
    *y = y2 - q * y1;
    a = b;
    b = r;
    x2 = x1;
    x1 = *x;
    y2 = y1;
    y1 = *y;
  }
  *d = a, *x = x2, *y = y2;
}

int inverse_evklid(unsigned int a, int n) {
  int d, x, y;
  evklid(a, n, &x, &y, &d);
  if (d == 1) return (x%n+n)%n;
  else return 0;
}

int main(void) {
  setlocale(LC_ALL, "rus");
  unsigned int a;
  int n, b;
  printf("Введите значение a и n\n");
  scanf("%d %d", &a, &n);
  b=inverse_evklid(a,n);
  printf("Обратное число по модулю равно %d", b);
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
```
