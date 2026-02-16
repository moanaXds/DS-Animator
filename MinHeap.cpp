// File: MinHeap.cpp
// Description: Min-Heap implementation with sift-up and sift-down

#include "MinHeap.h"
#include <sstream>
#include <algorithm>

MinHeap::MinHeap() : nextNodeId(0) {}

MinHeap::~MinHeap() {
    clear();
}

void MinHeap::swap(int i, int j) {
    HeapNode* temp = heap[i];
    heap[i] = heap[j];
    heap[j] = temp;
}

void MinHeap::insert(int value, std::vector<int>& siftPath) {
    // Create new node and add at end
    HeapNode* newNode = new HeapNode(value, nextNodeId++);
    heap.push_back(newNode);
    
    // Sift up to maintain heap property
    int current = static_cast<int>(heap.size()) - 1;
    siftPath.push_back(current);
    
    while (current > 0 && heap[parent(current)]->value > heap[current]->value) {
        swap(current, parent(current));
        current = parent(current);
        siftPath.push_back(current);
    }
}

HeapNode* MinHeap::extractMin(std::vector<int>& siftPath) {
    if (heap.empty()) return nullptr;
    
    HeapNode* minNode = heap[0];
    siftPath.push_back(0);
    
    // Move last element to root
    heap[0] = heap.back();
    heap.pop_back();
    
    if (!heap.empty()) {
        // Sift down to maintain heap property
        int current = 0;
        
        while (true) {
            int smallest = current;
            int left = leftChild(current);
            int right = rightChild(current);
            
            if (left < static_cast<int>(heap.size()) && 
                heap[left]->value < heap[smallest]->value) {
                smallest = left;
            }
            
            if (right < static_cast<int>(heap.size()) && 
                heap[right]->value < heap[smallest]->value) {
                smallest = right;
            }
            
            if (smallest != current) {
                siftPath.push_back(smallest);
                swap(current, smallest);
                current = smallest;
            } else {
                break;
            }
        }
    }
    
    return minNode;
}

HeapNode* MinHeap::peekMin() {
    return heap.empty() ? nullptr : heap[0];
}

int MinHeap::search(int value, std::vector<int>& searchPath) {
    for (int i = 0; i < static_cast<int>(heap.size()); i++) {
        searchPath.push_back(i);
        if (heap[i]->value == value) {
            return i;
        }
    }
    return -1;
}

bool MinHeap::remove(int value, std::vector<int>& siftPath) {
    // Find the value
    int index = -1;
    for (int i = 0; i < static_cast<int>(heap.size()); i++) {
        if (heap[i]->value == value) {
            index = i;
            break;
        }
    }
    
    if (index == -1) return false;
    
    siftPath.push_back(index);
    
    // Replace with last element
    HeapNode* toDelete = heap[index];
    heap[index] = heap.back();
    heap.pop_back();
    delete toDelete;
    
    if (index < static_cast<int>(heap.size())) {
        // Sift down
        int current = index;
        while (true) {
            int smallest = current;
            int left = leftChild(current);
            int right = rightChild(current);
            
            if (left < static_cast<int>(heap.size()) && 
                heap[left]->value < heap[smallest]->value) {
                smallest = left;
            }
            
            if (right < static_cast<int>(heap.size()) && 
                heap[right]->value < heap[smallest]->value) {
                smallest = right;
            }
            
            if (smallest != current) {
                siftPath.push_back(smallest);
                swap(current, smallest);
                current = smallest;
            } else {
                break;
            }
        }
        
        // Also try sift up in case new value is smaller than parent
        while (current > 0 && heap[parent(current)]->value > heap[current]->value) {
            swap(current, parent(current));
            current = parent(current);
            siftPath.push_back(current);
        }
    }
    
    return true;
}

void MinHeap::clear() {
    for (HeapNode* node : heap) {
        delete node;
    }
    heap.clear();
}

bool MinHeap::isEmpty() const {
    return heap.empty();
}

int MinHeap::getSize() const {
    return static_cast<int>(heap.size());
}

std::vector<HeapNode*> MinHeap::getAllNodes() {
    return heap;
}

HeapNode* MinHeap::getNode(int index) {
    if (index >= 0 && index < static_cast<int>(heap.size())) {
        return heap[index];
    }
    return nullptr;
}

std::string MinHeap::toString() {
    if (heap.empty()) return "[ Empty ]";
    
    std::ostringstream ss;
    ss << "[ ";
    for (size_t i = 0; i < heap.size(); i++) {
        ss << heap[i]->value;
        if (i < heap.size() - 1) ss << ", ";
    }
    ss << " ]";
    return ss.str();
}

bool MinHeap::isValidIndex(int index) const {
    return index >= 0 && index < static_cast<int>(heap.size());
}

