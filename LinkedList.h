// File: LinkedList.h
// Description: Singly Linked List data structure for visualization.
// Supports insert at head/tail, delete, and search operations.

#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <vector>
#include <string>

// ============================================================================
// LINKED LIST NODE
// ============================================================================
struct ListNode {
    int value;
    ListNode* next;
    
    // Visual properties
    int id;
    float x, y;
    float targetX, targetY;
    
    ListNode(int val, int nodeId) 
        : value(val), next(nullptr), id(nodeId), 
          x(0), y(0), targetX(0), targetY(0) {}
};

// ============================================================================
// LINKED LIST CLASS
// ============================================================================
class LinkedList {
private:
    ListNode* head;
    ListNode* tail;
    int nextNodeId;
    int size;

public:
    LinkedList();
    ~LinkedList();
    
    // Insert at the end (tail)
    bool insertAtTail(int value, std::vector<ListNode*>& path);
    
    // Insert at the beginning (head)
    bool insertAtHead(int value, std::vector<ListNode*>& path);
    
    // Delete a value
    bool remove(int value, std::vector<ListNode*>& path, ListNode*& deletedNode);
    
    // Search for a value
    ListNode* search(int value, std::vector<ListNode*>& path);
    
    // Check if contains value
    bool contains(int value);
    
    // Clear the list
    void clear();
    
    // Check if empty
    bool isEmpty() const;
    
    // Get size
    int getSize() const;
    
    // Get head node
    ListNode* getHead() const;
    
    // Get all nodes
    std::vector<ListNode*> getAllNodes();
    
    // Get values as string
    std::string toString();
};

#endif // LINKEDLIST_H

