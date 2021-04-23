#pragma once
#include "kernelMem.hpp"

namespace Kernel{

template<typename T>
class LinkedList{
    struct Node {
        union Data{
            T t;
            uint8_t buf[sizeof(T)];
            ~Data() {}
            Data(): buf{} {}
        } _t;
        Node* next;
        Node* prev;
        bool valid;
    };
    Node* start;
    friend class Iterator;
public:
    class Iterator{
        Node* n;
        bool _valid;
        LinkedList* list;
    public:
        Iterator(): _valid(false){};
        Iterator(Node* n, LinkedList* list): n(n), _valid(true), list(list){}
        T* get() {
            if(!_valid)
                return nullptr;
            return &n->_t.t;
        }
        bool next() {
            if(!_valid)
                return false;
            if(!n)
                return false;
            if(n->next) {
                n = n->next;
                return true;
            }
            return false;
        }
        bool prev() {
            if(!_valid)
                return false;
            if(n->prev) {
                n = n->prev;
                return true;
            }
            return false;
        }
        bool valid() {
            if(!_valid)
                return false;
            return n && n->valid;
        }
        void remove() {
            if(!_valid) {
                return;
            }
            if(!n) {
                return;
            }
            Node* next = n->next;
            Node* prev = n->prev;
            if(next) {
                next->prev = prev;
            }
            if(prev) {
                prev->next = next;
            }
            if(list->start == n)
                list->start = next;
            n->next = nullptr;
            n->prev = nullptr;
            n->valid = false;
            delete n;
            if(next) {
                n = next;
            } else {
                n = prev;
            }
        }
        void insert(const T& t) {
            if(!_valid)
                return;
            if(n == nullptr) {
                n = new Node();
                list->start = n;
                n->next = nullptr;
                n->prev = nullptr;
                n->valid = true;
                new(&n->_t.t)T(t);
                return;
            }
            Node* nn = new Node();
            Node* next = n->next;
            new(&nn->_t.t)T(t);
            nn->valid = true;
            nn->prev = n;
            nn->next = next;
            if(next) {
                next->prev = nn;
            }
            n->next = nn;
        }
        void insert(T&& t) {
            if(!_valid)
                return;
            if(n == nullptr) {
                n = new Node();
                list->start = n;
                n->next = nullptr;
                n->prev = nullptr;
                n->valid = true;
                new(&n->_t.t)T((T&&)t);
                return;
            }
            Node* nn = new Node();
            Node* next = n->next;
            new(&nn->_t.t)T((T&&)t);
            nn->valid = true;
            nn->prev = n;
            nn->next = next;
            if(next) {
                next->prev = nn;
            }
            n->next = nn;
        }
    };

    LinkedList() {
        start = nullptr;
    }

    LinkedList(const LinkedList&) = delete;
    LinkedList(LinkedList&&) = default;
    LinkedList& operator=(const LinkedList&) = delete;
    LinkedList& operator=(LinkedList&&) = default;

    ~LinkedList() {
        Node* tmp;
        for(Node* n = start; n; n = tmp) {
            tmp = n->next;
            delete n;
        }
    }

    Iterator getIterator() {
        Iterator i(start, this);
        return i;
    }
};

}