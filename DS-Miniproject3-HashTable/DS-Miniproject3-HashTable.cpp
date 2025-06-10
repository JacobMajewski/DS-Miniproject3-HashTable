#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <unordered_set>

#include "HashTable_OpenAddressing.hpp"
#include "HashTable_Chain.hpp"
#include "HashTable_AVL.hpp"

using namespace std;
using namespace std::chrono;

const int NUM_TESTS = 50;
const vector<int> TEST_SIZES = { 5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000 };
const vector<double> FILL_PERCENTAGES = { 0.2, 0.5, 0.9 };

struct TestData {
    vector<int> keys;
    vector<int> values;
};

TestData generateTestData(int size) {
    TestData data;
    unordered_set<int> used_keys;
    random_device rd;
    mt19937 gen(rd());

    uniform_int_distribution<> key_dis(1, size * 20);
    uniform_int_distribution<> val_dis(1, size * 10);

    while ((int)data.keys.size() < size) {
        int key = key_dis(gen);
        if (used_keys.insert(key).second) {
            data.keys.push_back(key);
            data.values.push_back(val_dis(gen));
        }
    }
    return data;
}

long long averageInsertTime_Open(int size, const TestData& data, int prefill, int to_insert) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashOpenAddressing table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < to_insert; ++i) {
            table.insert(data.keys[prefill + i], data.values[prefill + i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}

long long averageRemoveTime_Open(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashOpenAddressing table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < prefill; ++i) {
            table.remove(data.keys[i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}


long long averageInsertTime_Chain(int size, const TestData& data, int prefill, int to_insert) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashChain table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < to_insert; ++i) {
            table.insert(data.keys[prefill + i], data.values[prefill + i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}

long long averageRemoveTime_Chain(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashChain table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < prefill; ++i) {
            table.remove(data.keys[i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}


long long averageInsertTime_AVL(int size, const TestData& data, int prefill, int to_insert) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashTable_AVL table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < to_insert; ++i) {
            table.insert(data.keys[prefill + i], data.values[prefill + i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}

long long averageRemoveTime_AVL(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashTable_AVL table(size);
        for (int i = 0; i < prefill; ++i) {
            table.insert(data.keys[i], data.values[i]);
        }

        auto start = high_resolution_clock::now();
        for (int i = 0; i < prefill; ++i) {
            table.remove(data.keys[i]);
        }
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / NUM_TESTS;
}

int main() {
    for (int size : TEST_SIZES) {
        TestData data = generateTestData(static_cast<int>(size * 1.1));

        for (double fill : FILL_PERCENTAGES) {
            int prefill = static_cast<int>(size * fill);
            if (prefill < 1 && fill > 0.0) prefill = 1;
            if (prefill > size) prefill = size;

            int to_insert = max(1, static_cast<int>(size * 0.01));
            if (prefill + to_insert > size) to_insert = size - prefill;
            if (to_insert <= 0) to_insert = 0;


            if (to_insert > 0)
                cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_Open(size, data, prefill, to_insert) << "\n";
            cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_Open(size, data, prefill) << "\n";

            if (to_insert > 0)
                cout << "Chaining;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_Chain(size, data, prefill, to_insert) << "\n";
            cout << "Chaining;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_Chain(size, data, prefill) << "\n";

            if (to_insert > 0)
                cout << "AVL;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_AVL(size, data, prefill, to_insert) << "\n";
            cout << "AVL;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_AVL(size, data, prefill) << "\n";
        }
    }

    return 0;
}
