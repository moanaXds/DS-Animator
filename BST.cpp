// File: BST.cpp
// Description: Binary Search Tree implementation.
// Contains all the logic for BST operations: insert, delete, search.
// Each operation tracks the path taken for animation purposes.

#include "BST.h"

// ============================================================================
// CONSTRUCTOR & DESTRUCTOR
// ============================================================================

BST::BST() : root(nullptr), nextNodeId(0) {
    // Start with an empty tree
}

BST::~BST() {
    // Clean up all dynamically allocated nodes
    clear();
}

// ============================================================================
// INSERT OPERATION
// ============================================================================
// BST Insert Rule:
// - If value < current node: go left
// - If value > current node: go right
// - If value == current node: duplicate (not allowed)
// ============================================================================

bool BST::insert(int value, std::vector<Node*>& path) {
    bool success = true;
    root = insertHelper(root, value, success, path);
    return success;
}

Node* BST::insertHelper(Node* node, int value, bool& success, std::vector<Node*>& path) {
    // Base case: found an empty spot, create new node here
    if (node == nullptr) {
        Node* newNode = new Node(value, nextNodeId++);
        path.push_back(newNode);  // New node is also part of the path
        return newNode;
    }
    
    // Add current node to the path (we're visiting it)
    path.push_back(node);
    
    if (value < node->value) {
        // Value is smaller: go to left subtree
        node->left = insertHelper(node->left, value, success, path);
    } 
    else if (value > node->value) {
        // Value is larger: go to right subtree
        node->right = insertHelper(node->right, value, success, path);
    } 
    else {
        // Value already exists: this is a duplicate
        success = false;
    }
    
    return node;
}

// ============================================================================
// DELETE OPERATION
// ============================================================================
// BST Delete has three cases:
// 1. Node has no children (leaf): simply remove it
// 2. Node has one child: replace node with its child
// 3. Node has two children: replace with inorder successor (smallest in right subtree)
// ============================================================================

bool BST::remove(int value, std::vector<Node*>& path, 
                 Node*& deletedNode, Node*& successor) {
    bool success = true;
    deletedNode = nullptr;
    successor = nullptr;
    root = deleteHelper(root, value, success, path, deletedNode, successor);
    return success;
}

Node* BST::deleteHelper(Node* node, int value, bool& success, 
                        std::vector<Node*>& path, Node*& deletedNode, Node*& successor) {
    // Base case: value not found in tree
    if (node == nullptr) {
        success = false;
        return nullptr;
    }
    
    // Add current node to path (we're visiting it during search)
    path.push_back(node);
    
    if (value < node->value) {
        // Value is smaller: search in left subtree
        node->left = deleteHelper(node->left, value, success, path, deletedNode, successor);
    } 
    else if (value > node->value) {
        // Value is larger: search in right subtree
        node->right = deleteHelper(node->right, value, success, path, deletedNode, successor);
    } 
    else {
        // FOUND THE NODE TO DELETE!
        deletedNode = node;
        
        // Case 1: No left child (includes leaf nodes)
        if (node->left == nullptr) {
            Node* rightChild = node->right;
            // Don't delete the node yet - let the animation handle it
            return rightChild;
        }
        
        // Case 2: No right child
        if (node->right == nullptr) {
            Node* leftChild = node->left;
            return leftChild;
        }
        
        // Case 3: Node has two children
        // Find the inorder successor (smallest value in right subtree)
        std::vector<Node*> successorPath;
        successor = findMinWithPath(node->right, successorPath);
        
        // Add successor path to the main path for animation
        for (Node* n : successorPath) {
            path.push_back(n);
        }
        
        // Copy the successor's value to this node
        node->value = successor->value;
        
        // Delete the successor from the right subtree
        bool tempSuccess = true;
        Node* tempDeleted = nullptr;
        Node* tempSuccessor = nullptr;
        std::vector<Node*> tempPath;
        node->right = deleteHelper(node->right, successor->value, tempSuccess, 
                                   tempPath, tempDeleted, tempSuccessor);
    }
    
    return node;
}

// Find the minimum value node in a subtree (leftmost node)
Node* BST::findMin(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Find minimum and track the path (for animation)
Node* BST::findMinWithPath(Node* node, std::vector<Node*>& path) {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) {
        path.push_back(node);
        node = node->left;
    }
    path.push_back(node);
    return node;
}

// ============================================================================
// SEARCH OPERATION
// ============================================================================
// Search follows the same logic as insert:
// - If value < current: go left
// - If value > current: go right
// - If value == current: found it!
// ============================================================================

Node* BST::search(int value, std::vector<Node*>& path) {
    return searchHelper(root, value, path);
}

Node* BST::searchHelper(Node* node, int value, std::vector<Node*>& path) {
    // Base case: reached end without finding
    if (node == nullptr) {
        return nullptr;
    }
    
    // Add this node to the path (we're visiting it)
    path.push_back(node);
    
    if (value < node->value) {
        // Value is smaller: search left
        return searchHelper(node->left, value, path);
    } 
    else if (value > node->value) {
        // Value is larger: search right
        return searchHelper(node->right, value, path);
    } 
    else {
        // Found it!
        return node;
    }
}

// ============================================================================
// UTILITY FUNCTIONS
// ============================================================================

bool BST::contains(int value) {
    std::vector<Node*> path;
    return search(value, path) != nullptr;
}

void BST::clear() {
    clearHelper(root);
    root = nullptr;
}

void BST::clearHelper(Node* node) {
    if (node == nullptr) return;
    
    // Recursively delete children first (post-order)
    clearHelper(node->left);
    clearHelper(node->right);
    
    // Then delete this node
    delete node;
}

bool BST::isEmpty() const {
    return root == nullptr;
}

Node* BST::getRoot() const {
    return root;
}

std::vector<Node*> BST::getAllNodes() {
    std::vector<Node*> nodes;
    collectNodes(root, nodes);
    return nodes;
}

void BST::collectNodes(Node* node, std::vector<Node*>& nodes) {
    if (node == nullptr) return;
    nodes.push_back(node);
    collectNodes(node->left, nodes);
    collectNodes(node->right, nodes);
}

int BST::getHeight() const {
    return getHeightHelper(root);
}

int BST::getHeightHelper(Node* node) const {
    if (node == nullptr) return 0;
    int leftHeight = getHeightHelper(node->left);
    int rightHeight = getHeightHelper(node->right);
    return 1 + std::max(leftHeight, rightHeight);
}

// ============================================================================
// IN-ORDER TRAVERSAL
// ============================================================================
// In-order traversal visits: Left -> Current -> Right
// For a BST, this gives values in sorted order!
// ============================================================================

std::vector<int> BST::inorderTraversal() {
    std::vector<int> result;
    inorderHelper(root, result);
    return result;
}

void BST::inorderHelper(Node* node, std::vector<int>& result) {
    if (node == nullptr) return;
    
    inorderHelper(node->left, result);   // Visit left subtree
    result.push_back(node->value);        // Visit current node
    inorderHelper(node->right, result);  // Visit right subtree
}

