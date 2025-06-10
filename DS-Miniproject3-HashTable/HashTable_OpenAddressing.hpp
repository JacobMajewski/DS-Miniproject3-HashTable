#pragma once
#include "HashTable.hpp"

class HashOpenAddressing : public HashTable
{
private:
    struct Entry
    {
        int key;
        int value;
        bool isOccupied;
        bool isDeleted;
    };

    int tableSize;
    Entry* table;

    int hash(int key);
    int probe(int key, int i);

public:
    HashOpenAddressing(int size = 10);
    ~HashOpenAddressing();

    void insert(int key, int elem) override;
    int remove(int key) override;
};
