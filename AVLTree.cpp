// File: AVLTree.cpp
// Description: AVL Tree implementation with self-balancing rotations

#include "AVLTree.h"
#include <algorithm>
#include <sstream>

AVLTree::AVLTree() : root(nullptr), nextNodeId(0) {}

AVLTree::~AVLTree() {
    clear();
}

int AVLTree::getHeight(AVLNode* node) {
    return node ? node->height : 0;
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
}

// Right rotation (for Left-Left case)
//       y                x
//      / \             /   \
//     x   T3   -->    T1    y
//    / \                   / \
//   T1  T2               T2  T3
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    updateHeight(y);
    updateHeight(x);
    
    return x;  // New root
}

// Left rotation (for Right-Right case)
//     x                  y
//    / \               /   \
//   T1  y     -->     x    T3
//      / \           / \
//     T2  T3       T1  T2
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    updateHeight(x);
    updateHeight(y);
    
    return y;  // New root
}

bool AVLTree::insert(int value, std::vector<AVLNode*>& path, RotationType& rotation) {
    bool success = true;
    rotation = RotationType::NONE;
    root = insertHelper(root, value, success, path, rotation);
    return success;
}

AVLNode* AVLTree::insertHelper(AVLNode* node, int value, bool& success,
                                std::vector<AVLNode*>& path, RotationType& rotation) {
    // Standard BST insert
    if (node == nullptr) {
        AVLNode* newNode = new AVLNode(value, nextNodeId++);
        path.push_back(newNode);
        return newNode;
    }
    
    path.push_back(node);
    
    if (value < node->value) {
        node->left = insertHelper(node->left, value, success, path, rotation);
    } else if (value > node->value) {
        node->right = insertHelper(node->right, value, success, path, rotation);
    } else {
        // Duplicate value
        success = false;
        return node;
    }
    
    // Update height
    updateHeight(node);
    
    // Get balance factor
    int balance = getBalance(node);
    
    // Left Left Case
    if (balance > 1 && value < node->left->value) {
        rotation = RotationType::RIGHT;
        return rotateRight(node);
    }
    
    // Right Right Case
    if (balance < -1 && value > node->right->value) {
        rotation = RotationType::LEFT;
        return rotateLeft(node);
    }
    
    // Left Right Case
    if (balance > 1 && value > node->left->value) {
        rotation = RotationType::LEFT_RIGHT;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right Left Case
    if (balance < -1 && value < node->right->value) {
        rotation = RotationType::RIGHT_LEFT;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

bool AVLTree::remove(int value, std::vector<AVLNode*>& path, AVLNode*& deletedNode,
                     RotationType& rotation) {
    bool success = true;
    deletedNode = nullptr;
    rotation = RotationType::NONE;
    root = deleteHelper(root, value, success, path, deletedNode, rotation);
    return success;
}

AVLNode* AVLTree::deleteHelper(AVLNode* node, int value, bool& success,
                                std::vector<AVLNode*>& path, AVLNode*& deletedNode,
                                RotationType& rotation) {
    if (node == nullptr) {
        success = false;
        return nullptr;
    }
    
    path.push_back(node);
    
    if (value < node->value) {
        node->left = deleteHelper(node->left, value, success, path, deletedNode, rotation);
    } else if (value > node->value) {
        node->right = deleteHelper(node->right, value, success, path, deletedNode, rotation);
    } else {
        // Found node to delete
        deletedNode = node;
        
        // Node with one child or no child
        if (node->left == nullptr || node->right == nullptr) {
            AVLNode* temp = node->left ? node->left : node->right;
            
            if (temp == nullptr) {
                // No child
                return nullptr;
            } else {
                // One child - copy contents
                return temp;
            }
        } else {
            // Node with two children - get inorder successor
            AVLNode* temp = findMin(node->right);
            node->value = temp->value;
            node->right = deleteHelper(node->right, temp->value, success, path, deletedNode, rotation);
        }
    }
    
    if (node == nullptr) return nullptr;
    
    // Update height
    updateHeight(node);
    
    // Get balance factor
    int balance = getBalance(node);
    
    // Left Left Case
    if (balance > 1 && getBalance(node->left) >= 0) {
        rotation = RotationType::RIGHT;
        return rotateRight(node);
    }
    
    // Left Right Case
    if (balance > 1 && getBalance(node->left) < 0) {
        rotation = RotationType::LEFT_RIGHT;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    // Right Right Case
    if (balance < -1 && getBalance(node->right) <= 0) {
        rotation = RotationType::LEFT;
        return rotateLeft(node);
    }
    
    // Right Left Case
    if (balance < -1 && getBalance(node->right) > 0) {
        rotation = RotationType::RIGHT_LEFT;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

AVLNode* AVLTree::findMin(AVLNode* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

AVLNode* AVLTree::search(int value, std::vector<AVLNode*>& path) {
    return searchHelper(root, value, path);
}

AVLNode* AVLTree::searchHelper(AVLNode* node, int value, std::vector<AVLNode*>& path) {
    if (node == nullptr) return nullptr;
    
    path.push_back(node);
    
    if (value < node->value) {
        return searchHelper(node->left, value, path);
    } else if (value > node->value) {
        return searchHelper(node->right, value, path);
    }
    return node;
}

bool AVLTree::contains(int value) {
    std::vector<AVLNode*> path;
    return search(value, path) != nullptr;
}

void AVLTree::clear() {
    clearHelper(root);
    root = nullptr;
}

void AVLTree::clearHelper(AVLNode* node) {
    if (node) {
        clearHelper(node->left);
        clearHelper(node->right);
        delete node;
    }
}

bool AVLTree::isEmpty() const {
    return root == nullptr;
}

AVLNode* AVLTree::getRoot() const {
    return root;
}

std::vector<AVLNode*> AVLTree::getAllNodes() {
    std::vector<AVLNode*> nodes;
    collectNodes(root, nodes);
    return nodes;
}

void AVLTree::collectNodes(AVLNode* node, std::vector<AVLNode*>& nodes) {
    if (node) {
        nodes.push_back(node);
        collectNodes(node->left, nodes);
        collectNodes(node->right, nodes);
    }
}

int AVLTree::getTreeHeight() const {
    return root ? root->height : 0;
}

std::vector<int> AVLTree::inorderTraversal() {
    std::vector<int> result;
    inorderHelper(root, result);
    return result;
}

void AVLTree::inorderHelper(AVLNode* node, std::vector<int>& result) {
    if (node) {
        inorderHelper(node->left, result);
        result.push_back(node->value);
        inorderHelper(node->right, result);
    }
}

std::string AVLTree::getRotationName(RotationType type) {
    switch (type) {
        case RotationType::LEFT: return "Left Rotation";
        case RotationType::RIGHT: return "Right Rotation";
        case RotationType::LEFT_RIGHT: return "Left-Right Rotation";
        case RotationType::RIGHT_LEFT: return "Right-Left Rotation";
        default: return "";
    }
}

