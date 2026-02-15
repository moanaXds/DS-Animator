// File: BST.h
// Description: Binary Search Tree data structure declaration.
// This file defines the Node structure and the BST class with all operations.
// The BST stores integers and supports insert, delete, search, and traversal.

#ifndef BST_H
#define BST_H

#include <vector>
#include <functional>

// ============================================================================
// NODE STRUCTURE
// ============================================================================
// Each node in the BST contains:
// - value: the integer stored in this node
// - left/right: pointers to child nodes (nullptr if no child)
// - id: unique identifier for animation purposes
// - x, y: visual position on screen (managed by Visualizer)
// ============================================================================
struct Node {
    int value;
    Node* left;
    Node* right;
    
    // Visual properties (used by Visualizer for drawing/animation)
    int id;             // Unique node ID for tracking in animations
    float x, y;         // Current visual position
    float targetX, targetY;  // Target position for smooth movement
    
    // Constructor
    Node(int val, int nodeId) 
        : value(val), left(nullptr), right(nullptr), 
          id(nodeId), x(0), y(0), targetX(0), targetY(0) {}
};

// ============================================================================
// BST CLASS
// ============================================================================
// The Binary Search Tree class manages all tree operations.
// It maintains a root pointer and provides methods for:
// - Insertion: Add a new value (no duplicates allowed)
// - Deletion: Remove a value using standard BST deletion algorithm
// - Searching: Find a value and return the path taken
// - Traversal: Get all nodes in various orders
// ============================================================================
class BST {
private:
    Node* root;         // Pointer to the root node
    int nextNodeId;     // Counter for assigning unique IDs to nodes

    // ========================================================================
    // PRIVATE HELPER FUNCTIONS
    // ========================================================================
    
    // Recursively insert a value into the subtree rooted at 'node'
    // Returns the (possibly new) root of the subtree
    // Sets 'success' to false if the value already exists
    Node* insertHelper(Node* node, int value, bool& success, std::vector<Node*>& path);
    
    // Recursively delete a value from the subtree rooted at 'node'
    // Returns the (possibly new) root of the subtree
    // Sets 'success' to false if the value doesn't exist
    Node* deleteHelper(Node* node, int value, bool& success, 
                       std::vector<Node*>& path, Node*& deletedNode, Node*& successor);
    
    // Find the node with minimum value in a subtree (leftmost node)
    // Used during deletion when node has two children
    Node* findMin(Node* node);
    
    // Find the node with minimum value and track the path
    Node* findMinWithPath(Node* node, std::vector<Node*>& path);
    
    // Recursively search for a value
    Node* searchHelper(Node* node, int value, std::vector<Node*>& path);
    
    // Recursively delete all nodes in the subtree
    void clearHelper(Node* node);
    
    // Collect all nodes in the tree (for iteration/drawing)
    void collectNodes(Node* node, std::vector<Node*>& nodes);

public:
    // ========================================================================
    // CONSTRUCTOR & DESTRUCTOR
    // ========================================================================
    BST();
    ~BST();

    // ========================================================================
    // PUBLIC INTERFACE
    // ========================================================================
    
    // Insert a value into the BST
    // Returns true if insertion was successful, false if value already exists
    // 'path' will contain the nodes visited during insertion (for animation)
    bool insert(int value, std::vector<Node*>& path);
    
    // Delete a value from the BST
    // Returns true if deletion was successful, false if value not found
    // 'path' contains nodes visited, 'deletedNode' is the removed node,
    // 'successor' is the inorder successor (if applicable)
    bool remove(int value, std::vector<Node*>& path, 
                Node*& deletedNode, Node*& successor);
    
    // Search for a value in the BST
    // Returns pointer to the node if found, nullptr otherwise
    // 'path' contains all nodes visited during the search
    Node* search(int value, std::vector<Node*>& path);
    
    // Check if a value exists in the tree
    bool contains(int value);
    
    // Remove all nodes from the tree
    void clear();
    
    // Check if tree is empty
    bool isEmpty() const;
    
    // Get the root node (for visualization)
    Node* getRoot() const;
    
    // Get all nodes in the tree
    std::vector<Node*> getAllNodes();
    
    // Get height of the tree (for layout calculations)
    int getHeight() const;
    int getHeightHelper(Node* node) const;
    
    // In-order traversal: returns values in sorted order
    std::vector<int> inorderTraversal();
    void inorderHelper(Node* node, std::vector<int>& result);
};

#endif // BST_H

