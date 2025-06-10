#include "HashTable_Chain.hpp"

HashChain::HashChain(int size) : tableSize(size)
{
    table = new Node * [tableSize];
    for (int i = 0; i < tableSize; ++i)
        table[i] = nullptr;
}

HashChain::~HashChain()
{
    for (int i = 0; i < tableSize; ++i)
    {
        Node* curr = table[i];
        while (curr)
        {
            Node* tmp = curr;
            curr = curr->next;
            delete tmp;
        }
    }
    delete[] table;
}

int HashChain::hash(int key)
{
    return key % tableSize;
}

void HashChain::insert(int key, int elem)
{
    int idx = hash(key);
    Node* newNode = new Node{ key, elem, table[idx] };
    table[idx] = newNode;
}

int HashChain::remove(int key)
{
    int idx = hash(key);
    Node* curr = table[idx];
    Node* prev = nullptr;

    while (curr)
    {
        if (curr->key == key)
        {
            int val = curr->value;
            if (prev)
                prev->next = curr->next;
            else
                table[idx] = curr->next;
            delete curr;
            return val;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}
