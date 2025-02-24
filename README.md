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
