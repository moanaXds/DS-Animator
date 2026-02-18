// File: Stack.cpp
// Description: Stack (LIFO) implementation.

#include "Stack.h"
#include <sstream>

Stack::Stack() : nextNodeId(0) {}

Stack::~Stack() {
    clear();
}

StackNode* Stack::push(int value) {
    StackNode* newNode = new StackNode(value, nextNodeId++);
    elements.push_back(newNode);
    return newNode;
}

StackNode* Stack::pop() {
    if (elements.empty()) {
        return nullptr;
    }
    
    StackNode* topNode = elements.back();
    elements.pop_back();
    return topNode;  // Caller is responsible for deletion
}

StackNode* Stack::peek() {
    if (elements.empty()) {
        return nullptr;
    }
    return elements.back();
}

StackNode* Stack::search(int value, std::vector<StackNode*>& path) {
    // Search from top to bottom
    for (int i = static_cast<int>(elements.size()) - 1; i >= 0; i--) {
        path.push_back(elements[i]);
        if (elements[i]->value == value) {
            return elements[i];
        }
    }
    return nullptr;
}

bool Stack::contains(int value) {
    std::vector<StackNode*> path;
    return search(value, path) != nullptr;
}

void Stack::clear() {
    for (StackNode* node : elements) {
        delete node;
    }
    elements.clear();
}

bool Stack::isEmpty() const {
    return elements.empty();
}

int Stack::getSize() const {
    return static_cast<int>(elements.size());
}

std::vector<StackNode*> Stack::getAllNodes() {
    return elements;  // Returns copy
}

std::string Stack::toString() {
    if (isEmpty()) {
        return "[ Empty ]";
    }
    
    std::ostringstream ss;
    ss << "Top -> [ ";
    // Print from top to bottom
    for (int i = static_cast<int>(elements.size()) - 1; i >= 0; i--) {
        ss << elements[i]->value;
        if (i > 0) {
            ss << ", ";
        }
    }
    ss << " ] <- Bottom";
    return ss.str();
}

