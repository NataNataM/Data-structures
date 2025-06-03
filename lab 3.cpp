#include <iostream>
#include <vector>
#include <locale>
#include <windows.h>
#define MAX_K 6
using namespace std;

// Структура для 1 задачи (статический массив)
struct Massiv {
    int last1 = 0, last2 = 0, next1 = 0, next2 = 0, new_num = 0;
};

class Node {
public:
    int new_num;
    Node* next;
    Node* prev;

    Node(int new_num){
        this->new_num = new_num;
        this->next = nullptr;
        this->prev = nullptr;
    }

    void insertAtBeginning(Node*& head, int new_num) {
        Node* newNode = new Node(new_num);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }

    void insertAtEnd(Node*& head, int new_num) {
        Node* newNode = new Node(new_num);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }

    void insertAtPosition(Node*& head, int new_num, int position) {
        if (position < 1) {
            cout << "Номер позиции должен быть равен или больше 1" << endl;
            return;
        }
        if (position == 1) {
            insertAtBeginning(head, new_num);
            return;
        }
        Node* newNode = new Node(new_num);
        Node* temp = head;
        for (int i = 1; temp != nullptr && i < position - 1; i++) {
            temp = temp->next;
        }
        if (temp == nullptr) {
            cout << "Номер позиции превышает количество элементов в списке" << endl;
            return;
        }
        newNode->next = temp->next;
        newNode->prev = temp;
        if (temp->next != nullptr) {
            temp->next->prev = newNode;
        }
        temp->next = newNode;
    }

    void deleteAtBeginning(Node*& head) {
        if (head == nullptr) {
            cout << "Список пуст" << endl;
            return;
        }
        Node* temp = head;
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
        delete temp;
    }

    void deleteAtEnd(Node*& head) {
        if (head == nullptr) {
            cout << "Список пуст" << endl;
            return;
        }
        Node* temp = head;
        if (temp->next == nullptr) {
            head = nullptr;
            delete temp;
            return;
        }
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->prev->next = nullptr;
        delete temp;
    }

    void deleteAtPosition(Node*& head, int position) {
        if (head == nullptr) {
            cout << "Список пуст" << endl;
            return;
        }
        if (position == 1) {
            deleteAtBeginning(head);
            return;
        }
        Node* temp = head;
        for (int i = 1; temp != nullptr && i < position; i++) {
            temp = temp->next;
        }
        if (temp == nullptr) {
            cout << "Номер позиции превышает количество элементов в списке" << endl;
            return;
        }
        if (temp->next != nullptr) {
            temp->next->prev = temp->prev;
        }
        if (temp->prev != nullptr) {
            temp->prev->next = temp->next;
        }
        delete temp;
    }
}; 

// Структура для 2 задачи (связный список)
struct List {
    Node* new_num;
    int row, col;
    int nextRow, nextCol;
    int prevRow, prevCol;
};

// Структура для 3 задачи (вектор)
struct Vector {
    int last1 = 0, last2 = 0, next1 = 0, next2 = 0, new_num = 0;
};

int k, n, n1, cn, half;
Massiv m1[(1 << MAX_K) + 2][(1 << MAX_K) + 2];
List m[(1 << MAX_K) + 1][(1 << MAX_K) + 1];
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

void TwoConcat(int a1, int a2, int b1, int b2) {
    int i1 = a1, i2 = a2;
    while (m[i1][i2].nextRow != n + 1 && m[i1][i2].nextCol != n + 1) {
        int ni1 = m[i1][i2].nextRow;
        int ni2 = m[i1][i2].nextCol;
        i1 = ni1;
        i2 = ni2;
    }
    int j1 = b1, j2 = b2;
    while (m[j1][j2].nextRow != n + 1 && m[j1][j2].nextCol != n + 1) {
        int nj1 = m[j1][j2].nextRow;
        int nj2 = m[j1][j2].nextCol;
        j1 = nj1;
        j2 = nj2;
    }
    while (j1 != 0) {
        int nj2 = m[j1][j2].prevCol;
        int nj1 = m[j1][j2].prevRow;

        m[i1][i2].nextRow = j1;
        m[i1][i2].nextCol = j2;
        m[j1][j2].prevRow = i1;
        m[j1][j2].prevCol = i2;

        i1 = j1;
        i2 = j2;
        j1 = nj1;
        j2 = nj2;
    }
    m[i1][i2].nextRow = n + 1;
    m[i1][i2].nextCol = n + 1;
}

void Two() {
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j) {
            m[i][j].row = i;
            m[i][j].col = j;
            m[i][j].new_num = new Node((i - 1) * n + j);
            m[i][j].nextRow = n + 1;
            m[i][j].nextCol = n + 1;
            m[i][j].prevRow = 0;
            m[i][j].prevCol = 0;
        }
    int cn = n;
    while (cn > 1) {
        int half = cn / 2;
        // Сгибание по вертикали
        for (int i = 1; i <= half; ++i)
            for (int j = 1; j <= cn; ++j)
                TwoConcat(j, i, j, cn - i + 1);
        // Сгибание по горизонтали
        for (int i = 1; i <= half; ++i)
            for (int j = 1; j <= half; ++j)
                TwoConcat(i, j, cn - i + 1, j);

        cn /= 2;
    }
    int i = 1, j = 1;
    for (int num = 1; num <= n * n; ++num) {
        m[i][j].new_num->new_num = num;
        int ni = m[i][j].nextRow;
        int nj = m[i][j].nextCol;
        i = ni;
        j = nj;
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

void PrintList() {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cout << m[i][j].new_num->new_num << "\t";
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
    double t = ((double)(end.QuadPart - start.QuadPart) / freq.QuadPart) * 1000;
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
    PrintList();
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

    cout << "Выполнила: Мангер Наталья Александровна, группа 090304-РПИа-о24\n";
    return 0;
}
