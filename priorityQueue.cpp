#include<iostream>
#include<vector>
using namespace std;




template<typename ValType, typename Compare>
class PriorityQueue {
private:
    vector<ValType> heapArr;
    Compare cmp;
    int iterator = 0;

    int heapSize = 0;
    void heapify(int index) {
        
        while (1) {
            int lChild = index * 2 + 1;
            int rChild = index * 2 + 2;

            if (lChild >= heapSize) return;
            if (rChild == heapSize) rChild = lChild;

            int mostChild = cmp(heapArr[lChild], heapArr[rChild]) ? lChild : rChild;

            if (!cmp(heapArr[mostChild], heapArr[index])) return;

            swap(heapArr[index], heapArr[mostChild]);
            index = mostChild;
        }
    }

    void upSwap(int index) {
        while (index > 0) {
            int parent = (index - 1) >> 1;
            if (!cmp(heapArr[index], heapArr[parent])) return;
            swap(heapArr[index], heapArr[parent]);
            index = parent;
        }
    }

public:

    PriorityQueue() {
        
    }
    PriorityQueue(int reserve) {
        heapArr.reserve(reserve);
    }

    void push(ValType x) {
        if (heapArr.size() < heapSize+1) heapArr.push_back(x);
        heapArr[heapSize] = x;
        upSwap(heapSize++);
    }

    ValType top() {
        return heapArr[0];
    }

    void pop() {
        if (heapSize <= 0) return;
        heapArr[0] = heapArr[--heapSize];
        heapify(0);
    }

    int size() {
        return heapSize;
    }


    void initIterator() {
        iterator = 0;
    }

    bool hasNext() {
        return iterator < heapSize;
    }
    
    ValType get() {
        return heapArr[iterator++];
    }
};


int main() {
    struct Cmp {
        bool operator()(int a, int b) {
            return a < b;
        }
    };

    PriorityQueue<int, Cmp> PQ;

    PQ.push(4);
    PQ.push(3);
    PQ.push(5);
    PQ.push(2);
    PQ.push(8);

    PQ.initIterator();

    while (PQ.hasNext()) {
        cout << PQ.get() << endl;
    }

    system(("pause"));
}