// File: AVLTree.h
// Description: AVL Tree - Self-balancing Binary Search Tree
// AVL trees maintain balance by ensuring the height difference between
// left and right subtrees is at most 1. Rotations restore balance.

#ifndef AVLTREE_H
#define AVLTREE_H

#include <vector>
#include <string>

// ============================================================================
// AVL NODE STRUCTURE
// ============================================================================
struct AVLNode {
    int value;
    AVLNode* left;
    AVLNode* right;
    int height;         // Height of subtree rooted at this node
    
    // Visual properties
    int id;
    float x, y;
    float targetX, targetY;
    
    AVLNode(int val, int nodeId) 
        : value(val), left(nullptr), right(nullptr), height(1),
          id(nodeId), x(0), y(0), targetX(0), targetY(0) {}
};

// Rotation type for animation
enum class RotationType {
    NONE,
    LEFT,           // Single left rotation
    RIGHT,          // Single right rotation
    LEFT_RIGHT,     // Left-Right double rotation
    RIGHT_LEFT      // Right-Left double rotation
};

// ============================================================================
// AVL TREE CLASS
// ============================================================================
class AVLTree {
private:
    AVLNode* root;
    int nextNodeId;
    
    // Get height of a node (0 if null)
    int getHeight(AVLNode* node);
    
    // Get balance factor (left height - right height)
    int getBalance(AVLNode* node);
    
    // Update height of a node
    void updateHeight(AVLNode* node);
    
    // Rotation operations
    AVLNode* rotateRight(AVLNode* y);
    AVLNode* rotateLeft(AVLNode* x);
    
    // Recursive insert with balancing
    AVLNode* insertHelper(AVLNode* node, int value, bool& success, 
                          std::vector<AVLNode*>& path, RotationType& rotation);
    
    // Recursive delete with balancing
    AVLNode* deleteHelper(AVLNode* node, int value, bool& success,
                          std::vector<AVLNode*>& path, AVLNode*& deletedNode,
                          RotationType& rotation);
    
    // Find minimum node
    AVLNode* findMin(AVLNode* node);
    
    // Recursive search
    AVLNode* searchHelper(AVLNode* node, int value, std::vector<AVLNode*>& path);
    
    // Clear all nodes
    void clearHelper(AVLNode* node);
    
    // Collect all nodes
    void collectNodes(AVLNode* node, std::vector<AVLNode*>& nodes);
    
    // In-order traversal helper
    void inorderHelper(AVLNode* node, std::vector<int>& result);

public:
    AVLTree();
    ~AVLTree();
    
    // Insert a value (returns rotation type for animation)
    bool insert(int value, std::vector<AVLNode*>& path, RotationType& rotation);
    
    // Delete a value
    bool remove(int value, std::vector<AVLNode*>& path, AVLNode*& deletedNode, 
                RotationType& rotation);
    
    // Search for a value
    AVLNode* search(int value, std::vector<AVLNode*>& path);
    
    // Check if contains
    bool contains(int value);
    
    // Clear tree
    void clear();
    
    // Check if empty
    bool isEmpty() const;
    
    // Get root
    AVLNode* getRoot() const;
    
    // Get all nodes
    std::vector<AVLNode*> getAllNodes();
    
    // Get tree height
    int getTreeHeight() const;
    
    // In-order traversal
    std::vector<int> inorderTraversal();
    
    // Get rotation name for display
    static std::string getRotationName(RotationType type);
};

#endif // AVLTREE_H

