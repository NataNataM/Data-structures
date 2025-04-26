#include <iostream>
#include <cmath>
#include <vector>
#include <locale>
#include <windows.h>
#define MAX_K 6
using namespace std;

// Структура для 1 задачи (статический массив)
struct Massiv {
    int last1 = 0, last2 = 0, next1 = 0, next2 = 0, new_num = 0;
};

// Структура для 2 задачи (связный список)
struct Node {
    Node* last1 = nullptr;
    Node* last2 = nullptr;
    Node* next1 = nullptr;
    Node* next2 = nullptr;
    int new_num = 0;
    int x = 0, y = 0;
};

// Структура для 3 задачи (вектор)
struct Vector {
    int last1 = 0, last2 = 0, next1 = 0, next2 = 0, new_num = 0;
};

int k, n, n1, cn, half;
Massiv m1[(1 << MAX_K) + 2][(1 << MAX_K) + 2]; 
vector<vector<Node>> m2;
vector<vector<Vector>> m3;

void OneConcat(int a, int b, int c, int d) {
    int i1 = a, i2 = b;
    while (m1[i1][i2].next1 != n + 1 && m1[i1][i2].next2 != n + 1) {
        i1 = m1[i1][i2].next1;
        i2 = m1[i1][i2].next2;
    }
    int j1 = c, j2 = d;
    while (m1[j1][j2].next1 != n + 1 && m1[j1][j2].next2 != n + 1) {
        j1 = m1[j1][j2].next1;
        j2 = m1[j1][j2].next2;
    }
    while (j1 != 0) {
        int nj2 = m1[j1][j2].last2;
        int nj1 = m1[j1][j2].last1;
        m1[i1][i2].next1 = j1;
        m1[i1][i2].next2 = j2;
        m1[j1][j2].last1 = i1;
        m1[j1][j2].last2 = i2;
        i1 = j1;
        i2 = j2;
        j1 = nj1;
        j2 = nj2;
    }
    m1[i1][i2].next1 = n + 1;
    m1[i1][i2].next2 = n + 1;
}

void One() {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            m1[i][j] = { 0, 0, n + 1, n + 1, 0 };
        }
    }
    cn = n;
    while (cn > 1) {
        half = cn / 2;
        // сгиб по вертикали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= cn; j++) {
                OneConcat(j, i, j, cn - i + 1);
            }
        }
        // сгиб по горизонтали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= half; j++) {
                OneConcat(i, j, cn - i + 1, j);
            }
        }
        cn = half;
    }
    int j1 = 1, j2 = 1;
    for (int i = 1; i <= n1; i++) {
        m1[j1][j2].new_num = i;
        int nj1 = m1[j1][j2].next1;
        int nj2 = m1[j1][j2].next2;
        j1 = nj1;
        j2 = nj2;
    }
}

void TwoConcat(Node* a, Node* b) {
    Node* i1 = a;
    while (i1->next1 != nullptr && i1->next2 != nullptr) {
        i1 = i1->next1;
    }
    Node* j1 = b;
    while (j1->next1 != nullptr && j1->next2 != nullptr) {
        j1 = j1->next1;
    }
    while (j1 != nullptr) {
        Node* nj1 = j1->last1;
        Node* nj2 = j1->last2;

        i1->next1 = j1;
        i1->next2 = j1;
        j1->last1 = i1;
        j1->last2 = i1;

        i1 = j1;
        j1 = nj1;
    }
    i1->next1 = nullptr;
    i1->next2 = nullptr;
}

void Two() {
    m2.resize(n + 1, vector<Node>(n + 1));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            m2[i][j] = { nullptr, nullptr, nullptr, nullptr, 0, i, j };
        }
    }
    cn = n;
    while (cn > 1) {
        int half = cn / 2;
        // Сгиб по вертикали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= cn; j++) {
                TwoConcat(&m2[j][i], &m2[j][cn - i + 1]);
            }
        }
        // Сгиб по горизонтали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= half; j++) {
                TwoConcat(&m2[i][j], &m2[cn - i + 1][j]);
            }
        }
        cn = half;
    }
    Node* current = &m2[1][1];
    for (int i = 1; i <= n1; i++) {
        current->new_num = i;
        current = current->next1;
    }
}

void ThreeConcat(int a, int b, int c, int d) {
    int i1 = a, i2 = b;
    while (m3[i1][i2].next1 != n + 1 && m3[i1][i2].next2 != n + 1) {
        i1 = m3[i1][i2].next1;
        i2 = m3[i1][i2].next2;
    }
    int j1 = c, j2 = d;
    while (m3[j1][j2].next1 != n + 1 && m3[j1][j2].next2 != n + 1) {
        j1 = m3[j1][j2].next1;
        j2 = m3[j1][j2].next2;
    }
    while (j1 != 0) {
        int nj2 = m3[j1][j2].last2;
        int nj1 = m3[j1][j2].last1;
        m3[i1][i2].next1 = j1;
        m3[i1][i2].next2 = j2;
        m3[j1][j2].last1 = i1;
        m3[j1][j2].last2 = i2;
        i1 = j1;
        i2 = j2;
        j1 = nj1;
        j2 = nj2;
    }
    m3[i1][i2].next1 = n + 1;
    m3[i1][i2].next2 = n + 1;
}

void Three() {
    m3.resize(n + 2, vector<Vector>(n + 2));
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            m3[i][j] = { 0, 0, n + 1, n + 1, 0 };
        }
    }
    cn = n;
    while (cn > 1) {
        int half = cn / 2;
        // Сгиб по вертикали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= cn; j++) {
                ThreeConcat(j, i, j, cn - i + 1);
            }
        }
        // Сгиб по горизонтали
        for (int i = 1; i <= half; i++) {
            for (int j = 1; j <= half; j++) {
                ThreeConcat(i, j, cn - i + 1, j);
            }
        }
        cn = half;
    }
    int j1 = 1, j2 = 1;
    for (int i = 1; i <= n1; i++) {
        m3[j1][j2].new_num = i;
        int nj1 = m3[j1][j2].next1;
        int nj2 = m3[j1][j2].next2;
        j1 = nj1;
        j2 = nj2;
    }
}

template<typename T>
void Print(T m) {
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cout << m[i][j].new_num << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    setlocale(LC_ALL, "ru");
    cout << "Введите k (1 <= k <= 6): ";
    cin >> k;
    n = pow(2, k);
    n1 = n * n;

    LARGE_INTEGER freq, start, end;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    One();
    QueryPerformanceCounter(&end);
    double t = ((double)(end.QuadPart - start.QuadPart) / freq.QuadPart)*1000;
    double p = 2.0 * pow(n, 3) / t * pow(10, -6);
    cout << "\nРеализация с помощью массива:" << endl;
    Print(m1);
    cout << "Время: " << t << " секунд\n";
    cout << "Производительность: " << p << " MFlops" << endl << endl;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    Two();
    QueryPerformanceCounter(&end);
    t = ((double)(end.QuadPart - start.QuadPart) / freq.QuadPart) * 1000;
    p = 2.0 * pow(n, 3) / t * pow(10, -6);
    cout << "\nРеализация с помощью связного списка:" << endl;
    Print(m2);
    cout << "Время: " << t << " секунд\n";
    cout << "Производительность: " << p << " MFlops" << endl << endl;

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    Three();
    QueryPerformanceCounter(&end);
    t = ((double)(end.QuadPart - start.QuadPart) / freq.QuadPart) * 1000;
    p = 2.0 * pow(n, 3) / t * pow(10, -6);
    cout << "\nРеализация с помощью вектора:" << endl;
    Print(m3);
    cout << "Время: " << t << " секунд\n";
    cout << "Производительность: " << p << " MFlops" << endl << endl;

    cout<<"Выполнила: Мангер Наталья Александровна, группа 090304-РПИа-о24\n";
    return 0;
}