#pragma once
#include "HashTable.hpp"
// wyodrêbniono implementacje AVL w celu poprawienia czytelnoœci!
#include "AVL_tree.hpp"
class HashTable_AVL :
    public HashTable
{
private:
    AVLTree* table;
    size_t capacity;
    size_t size;
    const double maxLoadFactor = 2; // according to lecture, value is most often in range [1,3] so lets make it half way
    //table handler
    void reinsert_nodes(Node* node, AVLTree* new_table, size_t new_capacity);
    void resize(int targetCapacity);
    void check_resize();
    size_t hash(int key) const;
public:
    HashTable_AVL(size_t initial_capacity);
    ~HashTable_AVL();
     void insert(int key, int elem) override;
     int remove(int key) override;


};

