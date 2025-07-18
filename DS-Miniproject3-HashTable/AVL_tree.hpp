#pragma once
#include <memory>
struct Node {
    int key;        
    int value;      
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    int height;     

    Node(int k, int v) : key(k), value(v), left(nullptr), right(nullptr), height(1) {}
};
class AVLTree
{
private :
    
    
    // helpers
    int height(const Node* node) const;
    void update_height(Node& node);
    int balance_factor(const Node& node) const;

    std::unique_ptr<Node> rotate_right(std::unique_ptr<Node> y);
    std::unique_ptr<Node> rotate_left(std::unique_ptr<Node> x);

    std::unique_ptr<Node> balance(std::unique_ptr<Node> node);
    std::unique_ptr<Node> insert(std::unique_ptr<Node> node, int key, int value);
    std::unique_ptr<Node> remove(std::unique_ptr<Node> node, int key, bool& found);
    bool contains(const Node* node, int key) const;
    std::unique_ptr<Node>* find_node(std::unique_ptr<Node>* node, int key);
public:
    std::unique_ptr<Node> root;

    // Interfejs publiczny
     void insert(int key, int value);
     bool remove(int key);
    bool contains(int key) const;
    bool empty() const;

};

