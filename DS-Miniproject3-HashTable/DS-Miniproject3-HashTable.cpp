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

long long averageInsertTime_Open(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashOpenAddressing table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.insert(data.keys[prefill], data.values[prefill]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS * to_insert);
}

long long averageRemoveTime_Open(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashOpenAddressing table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.remove(data.keys[prefill - 1]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS * prefill);
}

long long averageInsertTime_Chain(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashChain table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.insert(data.keys[prefill], data.values[prefill]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS* to_insert);
}

long long averageRemoveTime_Chain(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashChain table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.remove(data.keys[prefill - 1]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS * prefill);
}

long long averageInsertTime_AVL(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashTable_AVL table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.insert(data.keys[prefill], data.values[prefill]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS* to_insert);
}

long long averageRemoveTime_AVL(int size, const TestData& data, int prefill) {
    long long total_time = 0;
    for (int t = 0; t < NUM_TESTS; ++t) {
        HashTable_AVL table(size);
        for (int i = 0; i < prefill; ++i)
            table.insert(data.keys[i], data.values[i]);

        auto start = high_resolution_clock::now();
        table.remove(data.keys[prefill - 1]);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / (NUM_TESTS * prefill);
}

int main() {
    for (int size : TEST_SIZES) {
        TestData data = generateTestData(static_cast<int>(size * 1.1)); // większy zapas danych

        for (double fill : FILL_PERCENTAGES) {
            int prefill = static_cast<int>(size * fill);
            if (prefill < 1) prefill = 1;
            if (prefill + 1 >= (int)data.keys.size()) continue;

            cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_Open(size, data, prefill) << "\n";
            cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_Open(size, data, prefill) << "\n";

            cout << "Chaining;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_Chain(size, data, prefill) << "\n";
            cout << "Chaining;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_Chain(size, data, prefill) << "\n";

            cout << "AVL;" << size << ";" << fill * 100 << "%;insert;" << averageInsertTime_AVL(size, data, prefill) << "\n";
            cout << "AVL;" << size << ";" << fill * 100 << "%;remove;" << averageRemoveTime_AVL(size, data, prefill) << "\n";
        }
    }

    return 0;
}
