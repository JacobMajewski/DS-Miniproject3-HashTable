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

const int NUM_TESTS = 10;         // 10 operations per dataset
const int NUM_DATASETS = 100;    // 10 datasets per size
const int MASTER_SEED = 19937;
int SEEDS[NUM_DATASETS];
const int TEST_SIZES[] = { 5000, 8000, 10000, 16000, 20000, 40000, 60000, 100000 };
const double FILL_PERCENTAGES[] = { 0.2, 0.5, 0.9 };

struct TestData {
    vector<int> keys;
    vector<int> values;
};

TestData generateTestData(int size, int seed) {
    TestData data;
    unordered_set<int> used_keys;
    mt19937 gen(seed);

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

long long totalInsertTime_Open(int size, const TestData& data, int prefill) {
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
    return total_time;
}

long long totalRemoveTime_Open(int size, const TestData& data, int prefill) {
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
    return total_time;
}

long long totalInsertTime_Chain(int size, const TestData& data, int prefill) {
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
    return total_time;
}

long long totalRemoveTime_Chain(int size, const TestData& data, int prefill) {
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
    return total_time;
}

long long totalInsertTime_AVL(int size, const TestData& data, int prefill) {
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
    return total_time;
}

long long totalRemoveTime_AVL(int size, const TestData& data, int prefill) {
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
    return total_time;
}

int main() {
    mt19937 seed_gen(MASTER_SEED);
    uniform_int_distribution<> seed_dis(0,100000000);
    for (int i = 0; i < NUM_DATASETS; i++)
        SEEDS[i] = seed_dis(seed_gen);
    for (int size : TEST_SIZES) {
        vector<TestData> datasets;
        for (int i = 0; i < NUM_DATASETS; ++i) {
            datasets.push_back(generateTestData(static_cast<int>(size * 1.1), SEEDS[i]));
        }

        for (double fill : FILL_PERCENTAGES) {
            int prefill = static_cast<int>(size * fill);
            if (prefill < 1) prefill = 1;
            if (prefill + 1 > static_cast<int>(size * 1.1)) continue;

            // Accumulate total time across datasets
            long long total_open_insert = 0, total_open_remove = 0;
            long long total_chain_insert = 0, total_chain_remove = 0;
            long long total_avl_insert = 0, total_avl_remove = 0;

            for (int i = 0; i < NUM_DATASETS; ++i) {
                total_open_insert += totalInsertTime_Open(size, datasets[i], prefill);
                total_open_remove += totalRemoveTime_Open(size, datasets[i], prefill);

                total_chain_insert += totalInsertTime_Chain(size, datasets[i], prefill);
                total_chain_remove += totalRemoveTime_Chain(size, datasets[i], prefill);

                total_avl_insert += totalInsertTime_AVL(size, datasets[i], prefill);
                total_avl_remove += totalRemoveTime_AVL(size, datasets[i], prefill);
            }

            // Calculate averages (100 measurements total)
            long long avg_open_insert = total_open_insert / (NUM_DATASETS * NUM_TESTS);
            long long avg_open_remove = total_open_remove / (NUM_DATASETS * NUM_TESTS);

            long long avg_chain_insert = total_chain_insert / (NUM_DATASETS * NUM_TESTS);
            long long avg_chain_remove = total_chain_remove / (NUM_DATASETS * NUM_TESTS);

            long long avg_avl_insert = total_avl_insert / (NUM_DATASETS * NUM_TESTS);
            long long avg_avl_remove = total_avl_remove / (NUM_DATASETS * NUM_TESTS);

            // Output results
            cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;insert;" << avg_open_insert << "\n";
            cout << "OpenAddressing;" << size << ";" << fill * 100 << "%;remove;" << avg_open_remove << "\n";

            cout << "Chaining;" << size << ";" << fill * 100 << "%;insert;" << avg_chain_insert << "\n";
            cout << "Chaining;" << size << ";" << fill * 100 << "%;remove;" << avg_chain_remove << "\n";

            cout << "AVL;" << size << ";" << fill * 100 << "%;insert;" << avg_avl_insert << "\n";
            cout << "AVL;" << size << ";" << fill * 100 << "%;remove;" << avg_avl_remove << "\n";
        }
    }

    return 0;
}