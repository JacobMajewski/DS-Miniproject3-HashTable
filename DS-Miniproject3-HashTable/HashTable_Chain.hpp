#pragma once
#include "HashTable.hpp"

class HashTable_Cuckoo : public HashTable {
private:
    static const int TABLE_SIZE = 101;
    int* table1_keys;
    int* table1_vals;
    int* table2_keys;
    int* table2_vals;
    bool* occupied1;
    bool* occupied2;

    int hash1(int key) const;
    int hash2(int key) const;

    static const int MAX_KICKS = 20;

public:
    HashTable_Cuckoo();
    ~HashTable_Cuckoo();

    void insert(int key, int elem) override;
    int remove(int key) override;
};
