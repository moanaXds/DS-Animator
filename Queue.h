// File: Queue.h
// Description: Queue data structure (FIFO - First In First Out) for visualization.
// Supports enqueue, dequeue, peek, and search operations.

#ifndef QUEUE_H
#define QUEUE_H

#include <vector>
#include <string>

// ============================================================================
// QUEUE NODE
// ============================================================================
struct QueueNode {
    int value;
    int id;
    float x, y;
    float targetX, targetY;
    
    QueueNode(int val, int nodeId) 
        : value(val), id(nodeId), x(0), y(0), targetX(0), targetY(0) {}
};

// ============================================================================
// QUEUE CLASS
// ============================================================================
class Queue {
private:
    std::vector<QueueNode*> elements;
    int nextNodeId;

public:
    Queue();
    ~Queue();
    
    // Enqueue value at rear (returns the new node)
    QueueNode* enqueue(int value);
    
    // Dequeue value from front (returns the dequeued node, caller must manage memory)
    QueueNode* dequeue();
    
    // Peek at front value without removing
    QueueNode* peekFront();
    
    // Peek at rear value
    QueueNode* peekRear();
    
    // Search for a value (returns path from front to found element)
    QueueNode* search(int value, std::vector<QueueNode*>& path);
    
    // Check if contains value
    bool contains(int value);
    
    // Clear the queue
    void clear();
    
    // Check if empty
    bool isEmpty() const;
    
    // Get size
    int getSize() const;
    
    // Get all nodes (from front to rear)
    std::vector<QueueNode*> getAllNodes();
    
    // Get values as string
    std::string toString();
};

#endif // QUEUE_H

