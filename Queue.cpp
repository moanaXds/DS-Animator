// File: Queue.cpp
// Description: Queue (FIFO) implementation.

#include "Queue.h"
#include <sstream>

Queue::Queue() : nextNodeId(0) {}

Queue::~Queue() {
    clear();
}

QueueNode* Queue::enqueue(int value) {
    QueueNode* newNode = new QueueNode(value, nextNodeId++);
    elements.push_back(newNode);
    return newNode;
}

QueueNode* Queue::dequeue() {
    if (elements.empty()) {
        return nullptr;
    }
    
    QueueNode* frontNode = elements.front();
    elements.erase(elements.begin());
    return frontNode;  // Caller is responsible for deletion
}

QueueNode* Queue::peekFront() {
    if (elements.empty()) {
        return nullptr;
    }
    return elements.front();
}

QueueNode* Queue::peekRear() {
    if (elements.empty()) {
        return nullptr;
    }
    return elements.back();
}

QueueNode* Queue::search(int value, std::vector<QueueNode*>& path) {
    // Search from front to rear
    for (size_t i = 0; i < elements.size(); i++) {
        path.push_back(elements[i]);
        if (elements[i]->value == value) {
            return elements[i];
        }
    }
    return nullptr;
}

bool Queue::contains(int value) {
    std::vector<QueueNode*> path;
    return search(value, path) != nullptr;
}

void Queue::clear() {
    for (QueueNode* node : elements) {
        delete node;
    }
    elements.clear();
}

bool Queue::isEmpty() const {
    return elements.empty();
}

int Queue::getSize() const {
    return static_cast<int>(elements.size());
}

std::vector<QueueNode*> Queue::getAllNodes() {
    return elements;
}

std::string Queue::toString() {
    if (isEmpty()) {
        return "[ Empty ]";
    }
    
    std::ostringstream ss;
    ss << "Front -> [ ";
    for (size_t i = 0; i < elements.size(); i++) {
        ss << elements[i]->value;
        if (i < elements.size() - 1) {
            ss << ", ";
        }
    }
    ss << " ] <- Rear";
    return ss.str();
}

