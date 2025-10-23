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

        //if (s == str) return *this;  

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

class CStrArray {
private:
    CStr** arr;       
    size_t capacity;  
    size_t size_;     

public:

    CStrArray(size_t cap) : capacity(cap), size_(0) {
        arr = new CStr*[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            arr[i] = nullptr;  
        }
    }


    ~CStrArray() {
        delete[] arr;
    }


    CStrArray& operator+=(CStr* p) {
        if (size_ < capacity) {
            arr[size_++] = p;  
        }
        else {
            cerr << "CStrArray full, cannot add more elements\n";
        }
        return *this;
    }


    CStr* operator[](size_t idx) const {
        assert(idx < capacity);  
        return arr[idx];
    }


    void sortByContent() {
        if (size_ < 2) return;

        for (size_t i = 0; i < size_ - 1; ++i) {
            for (size_t j = 0; j < size_ - 1 - i; ++j) {

                if (*(arr[j]) > *(arr[j + 1])) {

                    CStr* tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }
    }


    void sortByLength() {
        if (size_ < 2) return;

        for (size_t i = 0; i < size_ - 1; ++i) {
            for (size_t j = 0; j < size_ - 1 - i; ++j) {
                if (arr[j]->length() > arr[j + 1]->length()) {

                    CStr* tmp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = tmp;
                }
            }
        }
    }


    bool isSortedByContent() const {
        if (size_ < 2) return true;

        for (size_t i = 0; i + 1 < size_; ++i) {
            if (*(arr[i]) > *(arr[i + 1])) {
                return false;
            }
        }
        return true;
    }


    size_t size() const {
        return size_;
    }

    friend ostream& operator<<(ostream& os, const CStrArray& a);
};


ostream& operator<<(ostream& os, const CStrArray& a) {
    for (size_t i = 0; i < a.size_; ++i) {
        os << "[" << i << "]: ";
        if (a.arr[i]) {
            os << *(a.arr[i]) << " (length: " << a.arr[i]->length() << ")";
        }
        else {
            os << "(null)";
        }
        os << "\n";
    }
    return os;
}

int main() {
    srand((unsigned)time(nullptr));

    cout << "=== COMPLETE TEST OF CStr CLASS ===" << endl;

    cout << "\n1. Testing all constructors:" << endl;
    CStr s1;                    
    CStr s2("Hello");           
    CStr s3(5);                 
    CStr s4 = s2;               

    cout << "s1 (default): " << s1 << endl;
    cout << "s2 (from char*): " << s2 << endl;
    cout << "s3 (length 5): " << s3 << endl;
    cout << "s4 (copy of s2): " << s4 << endl;

    cout << "\n2. Testing assignment operators:" << endl;
    CStr s5;
    s5 = s2;                    
    cout << "s5 after s5 = s2: " << s5 << endl;

    s5 = "Assigned String";     
    cout << "s5 after s5 = \"Assigned String\": " << s5 << endl;

    CStr s10 = "fghjk";
   
    const char* a = "fghjk";
    s10 = s10.str;
    cout << "s10 = " << s10 << endl;


    cout << "\n3. Testing concatenation operators:" << endl;
    CStr s6 = s2 + s3;          
    cout << "s6 (s2 + s3): " << s6 << endl;

    s2 += s3;                   
    cout << "s2 after += s3: " << s2 << endl;

    
    cout << "\n4. Testing index operator with modification:" << endl;
    cout << "s2 before modification: " << s2 << endl;
    s2[0] = 'h';                
    s2[1] = 'E';                
    cout << "s2 after s2[0]='h', s2[1]='E': " << s2 << endl;

  
    cout << "\n5. Testing comparison operator:" << endl;
    CStr s7("hello");
    CStr s8("world!");
    cout << "s7: " << s7 << ", s8: " << s8 << endl;
    cout << "s7 > s8: " << (s7 > s8) << endl;
    cout << "s8 > s7: " << (s8 > s7) << endl;


    cout << "\n6. Testing input operator:" << endl;
    CStr s9;
    cout << "Enter a string for s9: ";
    cin >> s9;
    cout << "You entered: " << s9 << endl;


    cout << "\n7. Testing length method:" << endl;
    cout << "Length of s1: " << s1.length() << endl;
    cout << "Length of s2: " << s2.length() << endl;
    cout << "Length of s9: " << s9.length() << endl;

    cout << "\n=== COMPLETE TEST OF CStrArray CLASS ===" << endl;

    CStrArray arr(10);


    CStr* p1 = new CStr("svyatoslav");
    CStr* p2 = new CStr(8);      
    CStr* p3 = new CStr("pashko");
    CStr* p4 = new CStr();       
    CStr* p5 = new CStr("C++");
    CStr* p6 = new CStr(15);     


    arr += p1;
    arr += p2;
    arr += p3;
    arr += p4;
    arr += p5;
    arr += p6;

    cout << "\nInitial array:" << endl;
    cout << arr;


    cout << "\nTesting CStrArray index operator:" << endl;
    cout << "Element at index 2: " << *arr[2] << endl;

    cout << "\nIs array sorted by content? " << (arr.isSortedByContent() ? "Yes" : "No") << endl;


    cout << "\nSorting by content..." << endl;
    arr.sortByContent();
    cout << "After sorting by content:" << endl;
    cout << arr;
    cout << "Is sorted by content? " << (arr.isSortedByContent() ? "Yes" : "No") << endl;


    cout << "\nSorting by length..." << endl;
    arr.sortByLength();
    cout << "After sorting by length:" << endl;
    cout << arr;


    cout << "\nTesting edge cases:" << endl;
    CStrArray smallArr(2);
    cout << "Empty array: " << smallArr;
    cout << "Is empty array sorted? " << (smallArr.isSortedByContent() ? "Yes" : "No") << endl;

    CStr* single = new CStr("single");
    smallArr += single;
    cout << "Single element array: " << smallArr;
    cout << "Is single element sorted? " << (smallArr.isSortedByContent() ? "Yes" : "No") << endl;

    
    cout << "\nCleaning up memory..." << endl;
    delete p1;
    delete p2;
    delete p3;
    delete p4;
    delete p5;
    delete p6;
    delete single;

    return 0;
}