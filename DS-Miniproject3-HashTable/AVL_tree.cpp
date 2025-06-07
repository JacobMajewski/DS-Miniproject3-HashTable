#include "AVL_tree.hpp"
#include <algorithm>
// Metody prywatne
int AVLTree::height(const Node* node) const {
    return node ? node->height : 0;
}

void AVLTree::update_height(Node& node) {
    node.height = std::max(height(node.left.get()), height(node.right.get())) + 1;
}

int AVLTree::balance_factor(const Node& node) const {
    return height(node.left.get()) - height(node.right.get());
}

std::unique_ptr<Node> AVLTree::rotate_right(std::unique_ptr<Node> y) {
    std::unique_ptr<Node> x = std::move(y->left);
    y->left = std::move(x->right);
    x->right = std::move(y);

    update_height(*x->right);
    update_height(*x);

    return x;
}

std::unique_ptr<Node> AVLTree::rotate_left(std::unique_ptr<Node> x) {
    std::unique_ptr<Node> y = std::move(x->right);
    x->right = std::move(y->left);
    y->left = std::move(x);

    update_height(*y->left);
    update_height(*y);

    return y;
}

std::unique_ptr<Node> AVLTree::balance(std::unique_ptr<Node> node) {
    update_height(*node);
    int bf = balance_factor(*node);

    // Lewa nierównowaga
    if (bf > 1) {
        if (balance_factor(*node->left) < 0) {
            node->left = rotate_left(std::move(node->left));
        }
        return rotate_right(std::move(node));
    }

    // Prawa nierównowaga
    if (bf < -1) {
        if (balance_factor(*node->right) > 0) {
            node->right = rotate_right(std::move(node->right));
        }
        return rotate_left(std::move(node));
    }

    return node;
}

std::unique_ptr<Node> AVLTree::insert(std::unique_ptr<Node> node, int key, int value) {
    if (!node) {
        return std::make_unique<Node>(key, value);
    }

    if (key < node->key) {
        node->left = insert(std::move(node->left), key, value);
    }
    else if (key > node->key) {
        node->right = insert(std::move(node->right), key, value);
    }
    else {
        // Aktualizacja wartoœci dla istniej¹cego klucza
        node->value = value;
        return node;
    }

    return balance(std::move(node));
}

std::unique_ptr<Node> AVLTree::remove(std::unique_ptr<Node> node, int key, bool& found) {
    if (!node) return nullptr;

    if (key < node->key) {
        node->left = remove(std::move(node->left), key, found);
    }
    else if (key > node->key) {
        node->right = remove(std::move(node->right), key, found);
    }
    else {
        found = true;
        if (!node->left || !node->right) {
            return node->left ? std::move(node->left) : std::move(node->right);
        }

        // ZnajdŸ nastêpnik (minimalny w prawym poddrzewie)
        Node* successor = node->right.get();
        while (successor->left) successor = successor->left.get();

        // Skopiuj dane nastêpnika
        node->key = successor->key;
        node->value = successor->value;

        // Usuñ nastêpnika
        bool dummy;
        node->right = remove(std::move(node->right), successor->key, dummy);
    }

    return balance(std::move(node));
}

bool AVLTree::contains(const Node* node, int key) const {
    if (!node) return false;
    if (key == node->key) return true;
    if (key < node->key) return contains(node->left.get(), key);
    return contains(node->right.get(), key);
}

std::unique_ptr<Node>* AVLTree::find_node(std::unique_ptr<Node>* node_ptr, int key) {
    if (!node_ptr || !*node_ptr) {
        return nullptr;
    }

    Node* current_node = node_ptr->get();

    if (key == current_node->key) {
        return node_ptr;
    }
    else if (key < current_node->key) {
        return find_node(&(current_node->left), key);
    }
    else {
        return find_node(&(current_node->right), key);
    }
}
// Metody publiczne
void AVLTree::insert(int key, int value) {
    root = insert(std::move(root), key, value);
}

bool AVLTree::remove(int key) {
    bool found = false;
    root = remove(std::move(root), key, found);
    return found;
}

bool AVLTree::contains(int key) const {
    return contains(root.get(), key);
}

bool AVLTree::empty() const {
    return root == nullptr;
}