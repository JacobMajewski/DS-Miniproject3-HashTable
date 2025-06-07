#include "HashTable_AVL.hpp"

HashTable_AVL::HashTable_AVL(size_t initial_capacity)
    : capacity(initial_capacity), size(0)
{
    table = new AVLTree[capacity];
}

HashTable_AVL::~HashTable_AVL()
{
    delete[] table;
}
void HashTable_AVL::reinsert_nodes(Node* node, AVLTree* new_table, size_t new_capacity) {
    if (!node) return;

    // Rekurencyjne przegl¹danie lewego poddrzewa
    reinsert_nodes(node->left.get(), new_table, new_capacity);

    // Wstawienie bie¿¹cego wêz³a do nowej tablicy
    size_t new_index = hash(node->key) % new_capacity;
    new_table[new_index].insert(node->key, node->value);

    // Rekurencyjne przegl¹danie prawego poddrzewa
    reinsert_nodes(node->right.get(), new_table, new_capacity);
}

void HashTable_AVL::resize(int targetCapacity)
{
    AVLTree* new_table = new AVLTree[targetCapacity];

    // Przenoszenie elementów
    for (size_t i = 0; i < capacity; i++) {
        reinsert_nodes(table[i].root.get(), new_table, targetCapacity);
    }

    // Aktualizacja struktury
    delete[] table;
    table = new_table;
    capacity = targetCapacity;
}

void HashTable_AVL::check_resize()
{
    double currentLF = static_cast<double>(size) / capacity;

    if (currentLF >= maxLoadFactor) { // 2
        resize(capacity * 2);
    }
    else if (currentLF <= maxLoadFactor / 4 && capacity > 16) { // lecture, 2/4 = 0.5
        resize(capacity / 2);
    }
}

size_t HashTable_AVL::hash(int key) const
{
    /*
    Simples hash alg from lecutre, simple and with AVL tree should work fine enought
    h(x) = x mod m
    */
    return (static_cast<size_t>(std::abs(key))) % capacity;
}

void HashTable_AVL::insert(int key, int elem)
{
    size_t index = hash(key);
    table[index].insert(key, elem);
    size++;
    check_resize();
}

int HashTable_AVL::remove(int key)
{
    size_t index = hash(key);
    table[index].remove(key);
    size--;
    check_resize();
    return 0;
}
