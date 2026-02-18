// File: Stack.h
// Description: Stack data structure (LIFO - Last In First Out) for visualization.
// Supports push, pop, peek, and search operations.

#ifndef STACK_H
#define STACK_H

#include <vector>
#include <string>

// ============================================================================
// STACK NODE
// ============================================================================
struct StackNode {
    int value;
    int id;
    float x, y;
    float targetX, targetY;
    
    StackNode(int val, int nodeId) 
        : value(val), id(nodeId), x(0), y(0), targetX(0), targetY(0) {}
};

// ============================================================================
// STACK CLASS
// ============================================================================
class Stack {
private:
    std::vector<StackNode*> elements;
    int nextNodeId;

public:
    Stack();
    ~Stack();
    
    // Push value onto stack (returns the new node)
    StackNode* push(int value);
    
    // Pop value from stack (returns the popped node, caller must manage memory)
    StackNode* pop();
    
    // Peek at top value without removing
    StackNode* peek();
    
    // Search for a value (returns path from top to found element)
    StackNode* search(int value, std::vector<StackNode*>& path);
    
    // Check if contains value
    bool contains(int value);
    
    // Clear the stack
    void clear();
    
    // Check if empty
    bool isEmpty() const;
    
    // Get size
    int getSize() const;
    
    // Get all nodes (from bottom to top)
    std::vector<StackNode*> getAllNodes();
    
    // Get values as string (top to bottom)
    std::string toString();
};

#endif // STACK_H

