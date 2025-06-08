#pragma once
#include "HashTable.hpp"

class HashOpenAddressing : public HashTable
{
private:
    static const int TABLE_SIZE = 10;

    struct Entry
    {
        int key;
        int value;
        bool isOccupied;
        bool isDeleted;
    };

    Entry table[TABLE_SIZE];

    int hash(int key);
    int probe(int key, int i);

public:
    HashOpenAddressing();
    void insert(int key, int elem) override;
    int remove(int key) override;
};
