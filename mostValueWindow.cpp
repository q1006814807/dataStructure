#include<iostream>
#include<deque>
using namespace std;


class MostValueWindow {
private:

    deque<int> maxQ;
    deque<int> minQ;

public:

    void push(int elem) {
        while (maxQ.size() && maxQ.back() < elem) {
            maxQ.pop_back();
        }
        while (minQ.size() && minQ.back() > elem) {
            minQ.pop_back();
        }
        maxQ.push_back(elem);
        minQ.push_back(elem);
    }

    void pop(int elem) {
        if (maxQ.size() && maxQ.front() <= elem) maxQ.pop_front();
        if (minQ.size() && minQ.front() >= elem) minQ.pop_front();
    }

    int getMax() {
        return maxQ.size() ? maxQ.front() : 0;
    }

    int getMin() {
        return minQ.size() ? minQ.front() : 0;
    }

    int getDist() {
        return getMax() - getMin();
    }
};