#pragma once
#include "HashTable.hpp"

class HashChain : public HashTable
{
private:
    static const int TABLE_SIZE = 10;

    struct Node
    {
        int key;
        int value;
        Node* next;
    };

    Node* table[TABLE_SIZE];

    int hash(int key);

public:
    HashChain();
    ~HashChain();

    void insert(int key, int elem) override;
    int remove(int key) override;
};
