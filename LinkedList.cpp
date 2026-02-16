// File: LinkedList.cpp
// Description: Singly Linked List implementation.

#include "LinkedList.h"
#include <sstream>

LinkedList::LinkedList() : head(nullptr), tail(nullptr), nextNodeId(0), size(0) {}

LinkedList::~LinkedList() {
    clear();
}

bool LinkedList::insertAtTail(int value, std::vector<ListNode*>& path) {
    ListNode* newNode = new ListNode(value, nextNodeId++);
    
    if (head == nullptr) {
        // Empty list
        head = tail = newNode;
        path.push_back(newNode);
    } else {
        // Traverse to show animation path
        ListNode* current = head;
        while (current != nullptr) {
            path.push_back(current);
            current = current->next;
        }
        // Add at tail
        tail->next = newNode;
        tail = newNode;
        path.push_back(newNode);
    }
    
    size++;
    return true;
}

bool LinkedList::insertAtHead(int value, std::vector<ListNode*>& path) {
    ListNode* newNode = new ListNode(value, nextNodeId++);
    
    if (head == nullptr) {
        head = tail = newNode;
    } else {
        newNode->next = head;
        head = newNode;
    }
    
    path.push_back(newNode);
    size++;
    return true;
}

bool LinkedList::remove(int value, std::vector<ListNode*>& path, ListNode*& deletedNode) {
    deletedNode = nullptr;
    
    if (head == nullptr) {
        return false;
    }
    
    // Special case: deleting head
    if (head->value == value) {
        path.push_back(head);
        deletedNode = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        }
        size--;
        return true;
    }
    
    // Search for the node
    ListNode* prev = head;
    ListNode* current = head->next;
    path.push_back(prev);
    
    while (current != nullptr) {
        path.push_back(current);
        if (current->value == value) {
            // Found it
            deletedNode = current;
            prev->next = current->next;
            if (current == tail) {
                tail = prev;
            }
            size--;
            return true;
        }
        prev = current;
        current = current->next;
    }
    
    return false;  // Not found
}

ListNode* LinkedList::search(int value, std::vector<ListNode*>& path) {
    ListNode* current = head;
    
    while (current != nullptr) {
        path.push_back(current);
        if (current->value == value) {
            return current;
        }
        current = current->next;
    }
    
    return nullptr;
}

bool LinkedList::contains(int value) {
    std::vector<ListNode*> path;
    return search(value, path) != nullptr;
}

void LinkedList::clear() {
    ListNode* current = head;
    while (current != nullptr) {
        ListNode* next = current->next;
        delete current;
        current = next;
    }
    head = tail = nullptr;
    size = 0;
}

bool LinkedList::isEmpty() const {
    return head == nullptr;
}

int LinkedList::getSize() const {
    return size;
}

ListNode* LinkedList::getHead() const {
    return head;
}

std::vector<ListNode*> LinkedList::getAllNodes() {
    std::vector<ListNode*> nodes;
    ListNode* current = head;
    while (current != nullptr) {
        nodes.push_back(current);
        current = current->next;
    }
    return nodes;
}

std::string LinkedList::toString() {
    if (isEmpty()) {
        return "[ Empty ]";
    }
    
    std::ostringstream ss;
    ss << "[ ";
    ListNode* current = head;
    while (current != nullptr) {
        ss << current->value;
        if (current->next != nullptr) {
            ss << " -> ";
        }
        current = current->next;
    }
    ss << " ]";
    return ss.str();
}

