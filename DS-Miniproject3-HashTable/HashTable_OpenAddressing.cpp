#include "HashTable_OpenAddressing.hpp"

HashOpenAddressing::HashOpenAddressing(int size) : tableSize(size)
{
    table = new Entry[tableSize];
    for (int i = 0; i < tableSize; ++i)
    {
        table[i].isOccupied = false;
        table[i].isDeleted = false;
    }
}

HashOpenAddressing::~HashOpenAddressing()
{
    delete[] table;
}

int HashOpenAddressing::hash(int key)
{
    return key % tableSize;
}

int HashOpenAddressing::probe(int key, int i)
{
    return (hash(key) + i) % tableSize;
}

void HashOpenAddressing::insert(int key, int elem)
{
    for (int i = 0; i < tableSize; ++i)
    {
        int idx = probe(key, i);
        if (!table[idx].isOccupied || table[idx].isDeleted)
        {
            table[idx].key = key;
            table[idx].value = elem;
            table[idx].isOccupied = true;
            table[idx].isDeleted = false;
            return;
        }
    }
}

int HashOpenAddressing::remove(int key)
{
    for (int i = 0; i < tableSize; ++i)
    {
        int idx = probe(key, i);
        if (table[idx].isOccupied && table[idx].key == key)
        {
            table[idx].isDeleted = true;
            table[idx].isOccupied = false;
            return table[idx].value;
        }
    }
    return -1;
}
