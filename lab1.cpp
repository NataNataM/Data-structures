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