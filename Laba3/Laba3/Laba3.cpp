#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <map>
#include <queue>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

class SBase {
public:
    virtual ~SBase() {}
    virtual int Get() = 0;
};

class SFile : public SBase {
private:
    ifstream file;

public:
    SFile(const string& filename) {
        file.open(filename);
        if (!file.is_open()) {
            cerr << "Error: Cannot open file " << filename << endl;
        }
    }

    ~SFile() {
        if (file.is_open()) {
            file.close();
        }
    }

    virtual int Get() override {
        if (!file.is_open() || file.eof()) {
            return -1;
        }

        int value;
        file >> value;
        if (file.fail()) {
            return -1;
        }
        return value;
    }
};

class SKbrd : public SBase {
private:
    bool firstCall;

public:
    SKbrd() : firstCall(true) {}

    virtual int Get() override {
        if (firstCall) {
            cout << "Enter non-negative integers (negative to stop):" << endl;
            firstCall = false;
        }

        int value;
        cout << "Enter number: ";
        cin >> value;

        if (cin.fail() || value < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return -1;
        }

        return value;
    }
};


class SQueue : public SBase {
private:
    queue<int> numbers;

public:
    SQueue(size_t length, int maxValue = 100) {
        srand(static_cast<unsigned>(time(nullptr)));
        for (size_t i = 0; i < length; ++i) {
            numbers.push(rand() % (maxValue + 1));
        }
    }

    virtual int Get() override {
        if (numbers.empty()) {
            return -1;
        }

        int value = numbers.front();
        numbers.pop();
        return value;
    }

    size_t size() const {
        return numbers.size();
    }
};

class Freq {
protected:
    map<int, int> frequency;  
    long long sum;            
    int count;                

public:
    Freq() : sum(0), count(0) {}

    virtual void Calc(SBase& provider) {
        frequency.clear();
        sum = 0;
        count = 0;

        while (true) {
            int value = provider.Get();
            if (value < 0) break;

            frequency[value]++;
            sum += value;
            count++;
        }
    }

    virtual ~Freq() {}

    friend ostream& operator<<(ostream& os, const Freq& freq) {
        os << "Total numbers: " << freq.count << endl;
        os << "Sum: " << freq.sum << endl;
        os << "Frequency distribution:" << endl;

        for (const auto& pair : freq.frequency) {
            os << pair.first << ": " << pair.second << " piece" << endl;
        }

        return os;
    }

    long long getSum() const { return sum; }
    int getCount() const { return count; }
    const map<int, int>& getFrequency() const { return frequency; }
};

class Diap : public Freq {
private:
    int minValue;
    int maxValue;

public:
    Diap() : minValue(-1), maxValue(-1) {}

    virtual void Calc(SBase& provider) override {
        frequency.clear();
        sum = 0;
        count = 0;
        minValue = -1;
        maxValue = -1;

        while (true) {
            int value = provider.Get();
            if (value < 0) break;

            if (minValue == -1 || value < minValue) {
                minValue = value;
            }
            if (maxValue == -1 || value > maxValue) {
                maxValue = value;
            }

            frequency[value]++;
            sum += value;
            count++;
        }
    }

    friend ostream& operator<<(ostream& os, const Diap& diap) {
        os << "Total numbers: " << diap.count << endl;
        os << "Sum: " << diap.sum << endl;

        if (diap.minValue != -1) {
            os << "Minimum: " << diap.minValue << endl;
            os << "Maximum: " << diap.maxValue << endl;
            os << "Range: " << (diap.maxValue - diap.minValue) << endl;
        }
        else {
            os << "No numbers processed" << endl;
        }

        os << "Frequency distribution:" << endl;
        for (const auto& pair : diap.frequency) {
            os << pair.first << ": " << pair.second << " times" << endl;
        }

        return os;
    }

    int getMin() const { return minValue; }
    int getMax() const { return maxValue; }
};



void createTestFiles() {
    ofstream file1("test_data1.txt");
    if (file1.is_open()) {
        file1 << "10 20 30 40 50 10 20 30 10 20 10" << endl;
        file1.close();
        cout << "Test file 'test_data1.txt' created successfully!" << endl;
    }

    ofstream file2("test_data2.txt");
    if (file2.is_open()) {
        file2 << "5 5 5 5 5 5 5 5 5 5" << endl;
        file2.close();
        cout << "Test file 'test_data2.txt' created successfully!" << endl;
    }

    ofstream file3("test_data3.txt");
    if (file3.is_open()) {
        file3 << "1 2 3 4 5 6 7 8 9 10" << endl;
        file3.close();
        cout << "Test file 'test_data3.txt' created successfully!" << endl;
    }

    ofstream file4("test_data4.txt");
    if (file4.is_open()) {
        file4 << "15 8 22 8 15 37 22 8 15 8 45 37 15" << endl;
        file4.close();
        cout << "Test file 'test_data4.txt' created successfully!" << endl;
    }

    ofstream file5("test_data5.txt");
    if (file5.is_open()) {
        file5 << "" << endl;
        file5.close();
        cout << "Test file 'test_data5.txt' (empty) created successfully!" << endl;
    }
}

// ==================== ФУНКЦИИ ДЛЯ ДЕМОНСТРАЦИИ ВИРТУАЛЬНЫХ ВЫЗОВОВ ====================

// Невиртуальный вызов (статическое связывание)
void nonVirtualCall(Freq& processor, SBase& provider) {
    cout << "\n--- Non-virtual call ---" << endl;
    processor.Calc(provider);
    cout << processor;
}

// Виртуальный вызов через указатель на базовый класс
void virtualCall(Freq* processor, SBase& provider) {
    cout << "\n--- Virtual call ---" << endl;
    processor->Calc(provider);
    cout << *processor;
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    cout << "=== LAB 3: INHERITANCE AND VIRTUAL FUNCTIONS ===" << endl;

    createTestFiles();


    cout << "\n*** TESTING DIFFERENT DATA PROVIDERS ***" << endl;

    cout << "\n--- File Provider Test ---" << endl;
    SFile fileProvider("test_data1.txt");
    Freq freq1;
    freq1.Calc(fileProvider);
    cout << freq1;

    cout << "\n--- Queue Provider Test ---" << endl;
    SQueue queueProvider(15, 10);
    Diap diap1;
    diap1.Calc(queueProvider);
    cout << diap1;

    cout << "\n--- Keyboard Provider Test ---" << endl;
    SKbrd keyboardProvider;
    Freq freq2;
    cout << "Please enter some non-negative integers (negative to stop):" << endl;
    Freq* freq = &freq2;
    freq->Calc(keyboardProvider);

    cout << freq2;


    cout << "\n*** DEMONSTRATING VIRTUAL VS NON-VIRTUAL CALLS ***" << endl;

    SFile fileProvider2("test_data4.txt");
    SQueue queueProvider2(8, 20);

    Freq freqProcessor;
    cout << "\n--- Testing with Freq base class ---" << endl;

    nonVirtualCall(freqProcessor, fileProvider2);

    virtualCall(&freqProcessor, queueProvider2);

    Diap diapProcessor;
    cout << "\n--- Testing with Diap derived class ---" << endl;

    SFile fileProvider3("test_data4.txt");
    nonVirtualCall(diapProcessor, fileProvider3);

    SQueue queueProvider3(8, 15);
    virtualCall(&diapProcessor, queueProvider3);


    cout << "\n*** DEMONSTRATING POLYMORPHISM ***" << endl;

    Freq* processors[] = { new Freq(), new Diap() };
    SBase* providers[] = {
        new SFile("test_data1.txt"),
        new SFile("test_data3.txt"),
        new SQueue(10, 30)
    };

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            cout << "\n--- Combination " << i << "-" << j << " ---" << endl;
            cout << "Processor: " << (i == 0 ? "Freq" : "Diap") << endl;
            cout << "Provider: " << (j == 0 ? "File1" : (j == 1 ? "File3" : "Queue")) << endl;

            processors[i]->Calc(*providers[j]);
            cout << *processors[i];

            delete providers[j];
            if (j == 0) providers[j] = new SFile("test_data1.txt");
            else if (j == 1) providers[j] = new SFile("test_data3.txt");
            else providers[j] = new SQueue(10, 30);
        }
    }


    cout << "\n*** TESTING EDGE CASES ***" << endl;

    cout << "\n--- Empty File Test ---" << endl;
    SFile emptyFileProvider("test_data5.txt");
    Freq emptyFreq;
    emptyFreq.Calc(emptyFileProvider);
    cout << emptyFreq;

    cout << "\n--- Single Element Queue Test ---" << endl;
    SQueue singleQueueProvider(1, 5);
    Diap singleDiap;
    singleDiap.Calc(singleQueueProvider);
    cout << singleDiap;

    for (int i = 0; i < 2; ++i) {
        delete processors[i];
    }
    for (int j = 0; j < 3; ++j) {
        delete providers[j];
    }

    cout << "\n=== PROGRAM COMPLETED SUCCESSFULLY ===" << endl;
    return 0;
}