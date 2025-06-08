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

struct TestData {
    vector<int> keys;
    vector<int> values;
};


TestData generateTestData(int size) {
    TestData data;
    unordered_set<int> used;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, size * 10);

    while ((int)data.keys.size() < size) {
        int key = dis(gen);
        if (used.insert(key).second) {
            data.keys.push_back(key);
            data.values.push_back(dis(gen));
        }
    }
    return data;
}

template <typename Func>
long long measureAverageTime(Func f, int iterations) {
    long long total = 0;
    for (int i = 0; i < iterations; ++i) {
        auto start = high_resolution_clock::now();
        f();
        auto end = high_resolution_clock::now();
        total += duration_cast<nanoseconds>(end - start).count();
    }
    return total / iterations;
}

int main() {
    for (int size : TEST_SIZES) {
        TestData data = generateTestData(size);

        {
            HashOpenAddressing table;
            auto insertTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.insert(data.keys[i], data.values[i]);
                }, NUM_TESTS);

            cout << "OpenAddressing;" << size << ";insert;" << insertTime << "\n";

            auto removeTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.remove(data.keys[i]);
                }, NUM_TESTS);

            cout << "OpenAddressing;" << size << ";remove;" << removeTime << "\n";
        }

        // ==== CHAINING ====
        {
            HashChain table;
            auto insertTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.insert(data.keys[i], data.values[i]);
                }, NUM_TESTS);

            cout << "Chaining;" << size << ";insert;" << insertTime << "\n";

            auto removeTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.remove(data.keys[i]);
                }, NUM_TESTS);

            cout << "Chaining;" << size << ";remove;" << removeTime << "\n";
        }

        {
            HashTable_AVL table(size);
            auto insertTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.insert(data.keys[i], data.values[i]);
                }, NUM_TESTS);

            cout << "AVL;" << size << ";insert;" << insertTime << "\n";

            auto removeTime = measureAverageTime([&]() {
                for (int i = 0; i < size; ++i)
                    table.remove(data.keys[i]);
                }, NUM_TESTS);

            cout << "AVL;" << size << ";remove;" << removeTime << "\n";
        }
    }

    return 0;
}
