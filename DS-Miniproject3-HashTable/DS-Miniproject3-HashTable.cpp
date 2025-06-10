#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <unordered_set>
#include <memory>

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


template <typename TableType, typename Func>
long long measureAverageTime(int iterations, int table_size, const TestData& initial_data, int num_elements_to_prefill, int num_elements_to_operate, Func f_op) {
    long long total_time = 0;

    for (int i = 0; i < iterations; ++i) {
        //nowe tablice dla iteracji
        unique_ptr<TableType> table = make_unique<TableType>(table_size);

        //wypelnianie dla danego stanu
        for (int j = 0; j < num_elements_to_prefill; ++j) {
            table->insert(initial_data.keys[j], initial_data.values[j]);
        }

        auto start = high_resolution_clock::now();
        //lambda decyduje jakie lementy insert remove
        f_op(*table, initial_data, num_elements_to_prefill, num_elements_to_operate);
        auto end = high_resolution_clock::now();
        total_time += duration_cast<nanoseconds>(end - start).count();
    }
    return total_time / iterations;
}

int main() {
    for (int size : TEST_SIZES) {
        TestData full_data = generateTestData(static_cast<int>(size * 1.1)); 

        for (double fill_percent : FILL_PERCENTAGES) {
            int num_elements_current_fill = static_cast<int>(size * fill_percent);

            if (num_elements_current_fill == 0 && fill_percent > 0) {
                num_elements_current_fill = 1;
            }
            if (num_elements_current_fill > size) {
                num_elements_current_fill = size;
            }

            int num_elements_to_insert_batch = max(1, static_cast<int>(size * 0.01));
            if (num_elements_current_fill + num_elements_to_insert_batch > size) {
                num_elements_to_insert_batch = size - num_elements_current_fill;
            }
            if (num_elements_to_insert_batch <= 0) { 
                num_elements_to_insert_batch = 0;
            }

            {
                if (num_elements_to_insert_batch > 0) { 
                    auto insertTime = measureAverageTime<HashOpenAddressing>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_to_insert_batch,
                        [&](HashOpenAddressing& table, const TestData& data_source, int prefill_count, int insert_count) {
                            for (int i = 0; i < insert_count; ++i) {
                                table.insert(data_source.keys[prefill_count + i], data_source.values[prefill_count + i]);
                            }
                        });
                    cout << "OpenAddressing;" << size << ";" << (fill_percent * 100) << "%;insert;" << insertTime << "\n";
                }
                auto removeTime = measureAverageTime<HashOpenAddressing>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_current_fill,
                    [&](HashOpenAddressing& table, const TestData& data_source, int prefill_count, int remove_count) {
                        for (int i = 0; i < remove_count; ++i) {
                            table.remove(data_source.keys[i]);
                        }
                    });
                cout << "OpenAddressing;" << size << ";" << (fill_percent * 100) << "%;remove;" << removeTime << "\n";
            }
            {
                if (num_elements_to_insert_batch > 0) {
                    auto insertTime = measureAverageTime<HashChain>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_to_insert_batch,
                        [&](HashChain& table, const TestData& data_source, int prefill_count, int insert_count) {
                            for (int i = 0; i < insert_count; ++i) {
                                table.insert(data_source.keys[prefill_count + i], data_source.values[prefill_count + i]);
                            }
                        });
                    cout << "Chaining;" << size << ";" << (fill_percent * 100) << "%;insert;" << insertTime << "\n";
                }
                auto removeTime = measureAverageTime<HashChain>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_current_fill,
                    [&](HashChain& table, const TestData& data_source, int prefill_count, int remove_count) {
                        for (int i = 0; i < remove_count; ++i) {
                            table.remove(data_source.keys[i]);
                        }
                    });
                cout << "Chaining;" << size << ";" << (fill_percent * 100) << "%;remove;" << removeTime << "\n";
            }

            
            {
                if (num_elements_to_insert_batch > 0) {
                    auto insertTime = measureAverageTime<HashTable_AVL>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_to_insert_batch,
                        [&](HashTable_AVL& table, const TestData& data_source, int prefill_count, int insert_count) {
                            for (int i = 0; i < insert_count; ++i) {
                                table.insert(data_source.keys[prefill_count + i], data_source.values[prefill_count + i]);
                            }
                        });
                    cout << "AVL;" << size << ";" << (fill_percent * 100) << "%;insert;" << insertTime << "\n";
                }
                auto removeTime = measureAverageTime<HashTable_AVL>(NUM_TESTS, size, full_data, num_elements_current_fill, num_elements_current_fill,
                    [&](HashTable_AVL& table, const TestData& data_source, int prefill_count, int remove_count) {
                        for (int i = 0; i < remove_count; ++i) {
                            table.remove(data_source.keys[i]);
                        }
                    });
                cout << "AVL;" << size << ";" << (fill_percent * 100) << "%;remove;" << removeTime << "\n";
            }
        }
    }

    return 0;
}
