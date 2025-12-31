#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include <string>

using namespace std;

class CStr {
public:
    char* str;

    char* genRandom(size_t len) {
        if (len <= 0) len = 1;
        if (len > 20) len = 20;

        char* p = new char[len + 1];
        for (size_t i = 0; i < len; ++i) {
            p[i] = 'a' + rand() % 26;
        }
        p[len] = '\0';
        return p;
    }

public:
    CStr() {
        size_t len = 1 + rand() % 20;
        str = genRandom(len);
    }

    CStr(const char* s) {
        if (!s) {
            str = new char[1];
            str[0] = '\0';
        }
        else {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        }
    }

    CStr(size_t len) {
        str = genRandom(len);
    }

    CStr(const CStr& other) {
        str = new char[strlen(other.str) + 1];
        strcpy(str, other.str);
    }

    ~CStr() {
        delete[] str;
    }

    CStr& operator=(const CStr& other) {
        if (this == &other) return *this;
        delete[] str;
        str = new char[strlen(other.str) + 1];
        strcpy(str, other.str);
        return *this;
    }

    CStr& operator=(const char* s) {
        delete[] str;
        if (!s) {
            str = new char[1];
            str[0] = '\0';
        }
        else {
            str = new char[strlen(s) + 1];
            strcpy(str, s);
        }
        return *this;
    }

    CStr operator+(const CStr& other) const {
        size_t len1 = strlen(str);
        size_t len2 = strlen(other.str);
        char* buf = new char[len1 + len2 + 1];
        strcpy(buf, str);
        strcpy(buf + len1, other.str);
        CStr res(buf);
        delete[] buf;
        return res;
    }

    CStr& operator+=(const CStr& other) {
        size_t len1 = strlen(str);
        size_t len2 = strlen(other.str);
        char* buf = new char[len1 + len2 + 1];
        strcpy(buf, str);
        strcpy(buf + len1, other.str);
        delete[] str;
        str = buf;
        return *this;
    }

    bool operator>(const CStr& other) const {
        return strcmp(str, other.str) > 0;
    }

    char& operator[](size_t idx) {
        if (idx >= strlen(str)) {
            static char dummy = '\0';
            return dummy;
        }
        return str[idx];
    }

    const char& operator[](size_t idx) const {
        if (idx >= strlen(str)) {
            return '\0';
        }
        return str[idx];
    }

    size_t length() const {
        return strlen(str);
    }

    friend ostream& operator<<(ostream& os, const CStr& s);
    friend istream& operator>>(istream& is, CStr& s);
};

ostream& operator<<(ostream& os, const CStr& s) {
    os << s.str;
    return os;
}

istream& operator>>(istream& is, CStr& s) {
    char* tmp = new char[21];
    is >> tmp;
    delete[] s.str;
    s.str = tmp;
    return is;
}

// ==================== ШАБЛОННЫЙ КЛАСС TVector ====================
template <typename T>
class TVector {
private:
    T* data;
    size_t size;

public:
    // Конструктор с параметром размерности
    TVector(size_t n = 1) : size(n) {
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = T(); // инициализация значением по умолчанию
        }
    }

    // Конструктор копий
    TVector(const TVector& other) : size(other.size) {
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }

    // Деструктор
    ~TVector() {
        delete[] data;
    }

    // Метод изменения длины вектора
    void setLength(size_t newSize) {
        if (newSize == size) return;

        T* newData = new T[newSize];
        size_t minSize = (newSize < size) ? newSize : size;

        // Копируем существующие данные
        for (size_t i = 0; i < minSize; ++i) {
            newData[i] = data[i];
        }

        // Инициализируем новые элементы
        for (size_t i = minSize; i < newSize; ++i) {
            newData[i] = T();
        }

        delete[] data;
        data = newData;
        size = newSize;
    }

    // Оператор присваивания
    TVector& operator=(const TVector& other) {
        if (this == &other) return *this;

        delete[] data;
        size = other.size;
        data = new T[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
        return *this;
    }

    // Оператор += сложения двух векторов
    TVector& operator+=(const TVector& other) {
        if (size != other.size) {
            cerr << "Error: Vectors must have same size for addition!\n";
            return *this;
        }

        for (size_t i = 0; i < size; ++i) {
            data[i] += other.data[i];
        }
        return *this;
    }

    // Оператор индексации []
    T& operator[](size_t index) {
        if (index >= size) {
            static T dummy;
            return dummy;
        }
        return data[index];
    }

    // Константный оператор индексации []
    const T& operator[](size_t index) const {
        if (index >= size) {
            static T dummy;
            return dummy;
        }
        return data[index];
    }

    // Получение размера вектора
    size_t getSize() const {
        return size;
    }

    // Friend-операторы ввода/вывода
    friend ostream& operator<<(ostream& os, const TVector& vec) {
        os << "[";
        for (size_t i = 0; i < vec.size; ++i) {
            os << vec.data[i];
            if (i < vec.size - 1) os << ", ";
        }
        os << "]";
        return os;
    }

    friend istream& operator>>(istream& is, TVector& vec) {
        for (size_t i = 0; i < vec.size; ++i) {
            is >> vec.data[i];
        }
        return is;
    }
};

// ==================== ШАБЛОННЫЙ КЛАСС TMatrix ====================
template <typename T>
class TMatrix {
private:
    TVector<T>* rows;
    size_t rowsCount;
    size_t colsCount;

public:
    // Конструктор с параметрами число строк и столбцов
    TMatrix(size_t r = 1, size_t c = 1) : rowsCount(r), colsCount(c) {
        rows = new TVector<T>[rowsCount];
        for (size_t i = 0; i < rowsCount; ++i) {
            rows[i] = TVector<T>(colsCount);
        }
    }

    // Конструктор копий
    TMatrix(const TMatrix& other) : rowsCount(other.rowsCount), colsCount(other.colsCount) {
        rows = new TVector<T>[rowsCount];
        for (size_t i = 0; i < rowsCount; ++i) {
            rows[i] = other.rows[i];
        }
    }

    // Деструктор
    ~TMatrix() {
        delete[] rows;
    }

    // Оператор присваивания
    TMatrix& operator=(const TMatrix& other) {
        if (this == &other) return *this;

        if (rowsCount != other.rowsCount || colsCount != other.colsCount) {
            cerr << "Error: Matrices must have same dimensions for assignment!\n";
            return *this;
        }

        for (size_t i = 0; i < rowsCount; ++i) {
            rows[i] = other.rows[i];
        }
        return *this;
    }

    // Оператор += сложения двух матриц
    TMatrix& operator+=(const TMatrix& other) {
        if (rowsCount != other.rowsCount || colsCount != other.colsCount) {
            cerr << "Error: Matrices must have same dimensions for addition!\n";
            return *this;
        }

        for (size_t i = 0; i < rowsCount; ++i) {
            rows[i] += other.rows[i];
        }
        return *this;
    }

    // Оператор индексации []
    TVector<T>& operator[](size_t index) {
        if (index >= rowsCount) {
            static TVector<T> dummy;
            return dummy;
        }
        return rows[index];
    }

    // Константный оператор индексации []
    const TVector<T>& operator[](size_t index) const {
        if (index >= rowsCount) {
            static TVector<T> dummy;
            return dummy;
        }
        return rows[index];
    }

    // Получение размеров матрицы
    size_t getRows() const { return rowsCount; }
    size_t getCols() const { return colsCount; }

    // Friend-операторы ввода/вывода
    friend ostream& operator<<(ostream& os, const TMatrix& mat) {
        for (size_t i = 0; i < mat.rowsCount; ++i) {
            os << mat.rows[i];
            if (i < mat.rowsCount - 1) os << "\n";
        }
        return os;
    }

    friend istream& operator>>(istream& is, TMatrix& mat) {
        for (size_t i = 0; i < mat.rowsCount; ++i) {
            is >> mat.rows[i];
        }
        return is;
    }
};

// ==================== ТЕСТИРОВАНИЕ ====================

int main() {
    srand((unsigned)time(nullptr));

    cout << "=== TESTING TVector WITH DOUBLE ===" << endl;

    // Создание векторов double
    TVector<double> v1(3);
    TVector<double> v2(3);

    // Инициализация значений
    v1[0] = 1.5; v1[1] = 2.7; v1[2] = 3.9;
    v2[0] = 0.5; v2[1] = 1.3; v2[2] = 2.1;

    cout << "v1 = " << v1 << endl;
    cout << "v2 = " << v2 << endl;

    // Тестирование оператора +=
    v1 += v2;
    cout << "After v1 += v2: " << v1 << endl;

    // Тестирование изменения размера
    v1.setLength(5);
    cout << "After setLength(5): " << v1 << endl;

    // Тестирование копирования
    TVector<double> v3 = v1;
    cout << "v3 (copy of v1) = " << v3 << endl;

    cout << "\n=== TESTING TVector WITH STRING ===" << endl;

    TVector<string> vs1(2);
    TVector<string> vs2(2);

    vs1[0] = "Hello"; vs1[1] = "World";
    vs2[0] = " C++"; vs2[1] = " Programming";

    cout << "vs1 = " << vs1 << endl;
    cout << "vs2 = " << vs2 << endl;

    vs1 += vs2;
    cout << "After vs1 += vs2: " << vs1 << endl;

    cout << "\n=== TESTING TMatrix WITH INT ===" << endl;

    TMatrix<int> m1(2, 3);
    TMatrix<int> m2(2, 3);

    // Инициализация матриц
    for (size_t i = 0; i < m1.getRows(); ++i) {
        for (size_t j = 0; j < m1.getCols(); ++j) {
            m1[i][j] = i * m1.getCols() + j + 1;
            m2[i][j] = (i * m1.getCols() + j + 1) * 10;
        }
    }

    cout << "Matrix m1:\n" << m1 << endl;
    cout << "Matrix m2:\n" << m2 << endl;

    // Тестирование сложения матриц
    m1 += m2;
    cout << "After m1 += m2:\n" << m1 << endl;

    // Тестирование оператора индексации
    cout << "m1[1][2] = " << m1[1][2] << endl;

    cout << "\n=== TESTING TMatrix WITH CStr ===" << endl;

    TMatrix<CStr> ms(2, 2);

    // Инициализация строковой матрицы
    ms[0][0] = CStr("Hello");
    ms[0][1] = CStr("World");
    ms[1][0] = CStr("C++");
    ms[1][1] = CStr("Lab");

    cout << "String Matrix:\n" << ms << endl;

    // Тестирование ввода/вывода
    cout << "\n=== TESTING INPUT/OUTPUT ===" << endl;

    TVector<double> vInput(2);
    cout << "Enter 2 double values for vector: ";
    cin >> vInput;
    cout << "You entered: " << vInput << endl;

    TMatrix<int> mInput(2, 2);
    cout << "Enter 4 int values for 2x2 matrix: ";
    cin >> mInput;
    cout << "You entered:\n" << mInput << endl;

    return 0;
}