#pragma once
#include "HashTable.hpp"

class HashChain : public HashTable
{
private:
    struct Node
    {
        int key;
        int value;
        Node* next;
    };

    int tableSize;
    Node** table;

    int hash(int key);

public:
    HashChain(int size = 10);
    ~HashChain();

    void insert(int key, int elem) override;
    int remove(int key) override;
};
