#include<iostream>
using namespace std;

class hashSet {
private:
    struct Node {
        int val;
        Node* next;
        Node() {
            this->val = 0;
            this->next = nullptr;
        }
        Node(int num) {
            this->val = num;
            this->next = nullptr;
        }
    };

    // 散列函数
    int hash(int val) {
        uint32_t transVal = uint32_t(val) - INT32_MAX - 1;
        return transVal % capacity;
    }

    // 返回插入位置的父节点
    Node* get(int val) {
        int idx = hash(val);
        auto ptr = &arr[idx];
        while (ptr->next) {
            if (ptr->next->val == val) return ptr;
            ptr = ptr->next;
        }
        return ptr;
    }

    // 扩展容量
    void expand() {

        int oldCapacity = capacity;
        capacity = capacity * 2 + 1;

        size = 0;

        Node* oldArr = arr;
        arr = new Node[capacity];

        for (int i = 0; i < oldCapacity; ++i) {
            auto ptr = &oldArr[i];
            while (ptr->next) {
                // 重新放入新数组
                int data = ptr->next->val;
                // ========================

                insert(data);

                // ========================
                ptr = ptr->next;
            }
        }

        a = size / float(capacity);

        for (int i = 0; i < oldCapacity; ++i) {
            freeNode(&oldArr[i]);
        }
        delete[] oldArr;
    }
    // 减少容量
    void reduce() {

        int oldCapacity = capacity;
        capacity = capacity / 2;

        size = 0;

        Node* oldArr = arr;
        arr = new Node[capacity];

        for (int i = 0; i < oldCapacity; ++i) {
            auto ptr = &oldArr[i];
            while (ptr->next) {
                // 重新放入新数组
                int data = ptr->next->val;
                // ========================

                insert(data);

                // ========================
                ptr = ptr->next;
            }
        }

        a = size / float(capacity);


        for (int i = 0; i < oldCapacity; ++i) {
            freeNode(&oldArr[i]);
        }
        delete[] oldArr;
    }
    // 释放链内存(不释放自己)
    void freeNode(Node* node) {
        if (!node) return;
        freeNode(node->next);
        delete node->next;
    }
 
    const int initCapacity = 5;
    int capacity = initCapacity;
    Node* arr;
    int size = 0;
    float a = 0;

    bool canReduce = false;

public:
    /** Initialize your data structure here. */
    hashSet(int initCapacity = 0) {
        if (initCapacity < 5) {
            capacity = this->initCapacity;
        }
        else {
            capacity = initCapacity;
        }

        arr = new Node[capacity];
    }
    ~hashSet() {
        for (int i = 0; i < capacity; ++i) {
            freeNode(&arr[i]);
        }
        delete[] arr;
    }
    
    
    /** Inserts a value to the set. Returns true if the set did not already contain the specified element. */
    bool insert(int val) {
        auto ptr = get(val);
        if (ptr->next != nullptr) return false;

        ptr->next = new Node(val);

        ++arr[hash(val)].val;
        ++size;
        a = size / float(capacity);

        if (a > 0.75) expand();
        
        return true;
    }
    
    /** Removes a value from the set. Returns true if the set contained the specified element. */
    bool erase(int val) {
        if (size == 0) return false;

        auto ptr = get(val);
        if (ptr->next == nullptr) return false;

        auto temp = ptr->next;
        ptr->next = temp->next;

        delete temp;

        --arr[hash(val)].val;
        --size;
        a = size / float(capacity);
        if (canReduce && size > 0 && a < 0.1) reduce();

        return true;
    }

    bool has(int val) {
        return this->get(val)->next != nullptr;
    }
    int length() {
        return this->size;
    }

    void setReduce(bool canReduce) {
        this->canReduce = canReduce;
    }

};


int main () {

    return 0;

}