// File: MinHeap.h
// Description: Min-Heap data structure (Priority Queue)
// A complete binary tree where each parent is smaller than its children.
// Supports insert (with sift-up) and extract-min (with sift-down).

#ifndef MINHEAP_H
#define MINHEAP_H

#include <vector>
#include <string>

// ============================================================================
// HEAP NODE STRUCTURE
// ============================================================================
struct HeapNode {
    int value;
    int id;
    float x, y;
    float targetX, targetY;
    
    HeapNode(int val, int nodeId) 
        : value(val), id(nodeId), x(0), y(0), targetX(0), targetY(0) {}
};

// ============================================================================
// MIN HEAP CLASS
// ============================================================================
class MinHeap {
private:
    std::vector<HeapNode*> heap;
    int nextNodeId;
    
    // Get parent index
    int parent(int i) { return (i - 1) / 2; }
    
    // Get left child index
    int leftChild(int i) { return 2 * i + 1; }
    
    // Get right child index
    int rightChild(int i) { return 2 * i + 2; }
    
    // Swap two elements
    void swap(int i, int j);

public:
    MinHeap();
    ~MinHeap();
    
    // Insert a value (sift-up animation path returned)
    void insert(int value, std::vector<int>& siftPath);
    
    // Extract minimum (sift-down animation path returned)
    HeapNode* extractMin(std::vector<int>& siftPath);
    
    // Peek at minimum without removing
    HeapNode* peekMin();
    
    // Search for a value (returns index, -1 if not found)
    int search(int value, std::vector<int>& searchPath);
    
    // Delete a specific value
    bool remove(int value, std::vector<int>& siftPath);
    
    // Clear heap
    void clear();
    
    // Check if empty
    bool isEmpty() const;
    
    // Get size
    int getSize() const;
    
    // Get all nodes (in array order)
    std::vector<HeapNode*> getAllNodes();
    
    // Get node at index
    HeapNode* getNode(int index);
    
    // Get heap as string
    std::string toString();
    
    // Check if index is valid
    bool isValidIndex(int index) const;
};

#endif // MINHEAP_H

