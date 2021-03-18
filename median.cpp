#include<set>
#include<map>
#include<unordered_map>
#include<queue>
#include<iostream>
using namespace std;

// 一种快速求中位数的容器，增删时间复杂度都是o(logN) - multiset实现
class Median1 {
private:
    multiset<int> left, right;  

    void balance() {
        // 平衡
        if (left.size() > right.size()+1) {
            // 右平衡
            int elem = *left.rbegin();
            left.erase(left.find(elem));
            right.insert(elem);
        }
        else if (right.size() > left.size()) {
            // 左平衡
            int elem = *right.begin();
            right.erase(right.find(elem));
            left.insert(elem);
        }
    }

public:
    

    Median1() {}

    void insert(int x) {

        int lsize = left.size();
        int rsize = right.size();

        if (lsize == 0) {
            left.insert(x);
            return;
        }

        int pivot = *left.rbegin();

        if (x >= pivot) {
            // 放右边
            right.insert(x);
        }
        else {
            // 放左边
            left.insert(x);
        }
        
        // 保持左右平衡
        balance();

    }

    void erase(int x) {
        int pivot = *left.rbegin();
        if (x <= pivot) {
            auto res = left.find(x);
            if (res != left.end())
                left.erase(res);
        }
        else {
            auto res = right.find(x);
            if (res != right.end())
                right.erase(res);
        }
        // 保持左右平衡
        balance();
    }

    double getMdian() const {
        if (left.size() == right.size()) {
            double L = *left.rbegin();
            double R = *right.begin();
            double res = (L + R) / 2.0f;
            return res;
        }
        return *left.rbegin();
    }

};

// 一种快速求中位数的容器，增删时间复杂度都是o(logN) - map实现
class Median2 {
private:
    // <num, count>
    map<int, int> left, right;  
    int lsize = 0, rsize = 0;

    void balance() {
        // 平衡
        if (lsize > rsize+1) {
            // 左边向右平衡

            auto iter = --left.end();

            int elem = iter->first;

            auto& lcount = iter->second;
            --lcount;
            if (lcount <= 0) {
                left.erase(iter);
            }

            --lsize;
            ++right[elem];
            ++rsize;
        }
        else if (rsize > lsize) {
            // 右边向左平衡
            auto iter = right.begin();
            int elem = iter->first;

            auto& rcount = iter->second;
            --rcount;
            if (rcount <= 0) {
                right.erase(iter);
            }

            --rsize;
            ++left[elem];
            ++lsize;
        }
    }

public:
    

    Median2() {}

    void insert(int x) {

        if (lsize == 0) {
            left[x] = 1;
            ++lsize;
            return;
        }

        int pivot = (*left.rbegin()).first;

        if (x >= pivot) {
            // 放右边
            ++right[x];
            ++rsize;
        }
        else {
            // 放左边
            ++left[x];
            ++lsize;
        }
        
        // 保持左右平衡
        balance();

    }

    void erase(int x) {
        int pivot = (*left.rbegin()).first;

        if (x <= pivot) {
            auto res = left.find(x);
            if (res != left.end()) {
                --res->second;
                if (res->second <= 0) {
                    left.erase(res);
                }   
                --lsize;
            }
                
        }
        else {
            auto res = right.find(x);
            if (res != right.end()) {
                --res->second;
                if (res->second <= 0) {
                    right.erase(res);
                }
                --rsize;
            }
                
        }
        // 保持左右平衡
        balance();
    }

    double getMdian() const {
        if (lsize == rsize) {
            double L = (*left.rbegin()).first;
            double R = (*right.begin()).first;
            double res = (L + R) / 2.0f;
            return res;
        }
        return (*left.rbegin()).first;
    }

};

// 一种快速求中位数的容器，增删时间复杂度都是o(logN) - heap/pqueue实现 (最优)
class Median3 {
private:
    // <num, count>
    priority_queue<int, vector<int>, less<int>> left;  
    priority_queue<int, vector<int>, greater<int>> right;  
    int lsize = 0, rsize = 0;
    
    // 延迟删除记录
    unordered_map<int, int> ldelMemo, rdelMemo;
    

    void balance() {
        // 平衡
        if (lsize > rsize+1) {
            // 左边向右平衡

            int elem = getLeftTop();
            left.pop();

            right.push(elem);

            --lsize;
            ++rsize;
        }
        else if (rsize > lsize) {
            // 右边向左平衡
            int elem = getRightTop();
            right.pop();

            left.push(elem);

            --rsize;
            ++lsize;
        }
    }

    int getLeftTop() {
        if (lsize == 0) return 0;
        if (lsize == left.size()) return left.top();

        int elem = left.top();
        auto it = ldelMemo.find(elem);
        while (it != ldelMemo.end()) {  
            left.pop();
            elem = left.top();
            --it->second;
            if (it->second <= 0) {
                ldelMemo.erase(it);
            }

            it = ldelMemo.find(elem);
        }

        return elem;
    }

    int getRightTop() {
        if (rsize == 0) return 0;
        if (rsize == right.size()) return right.top();

        int elem = right.top();
        auto it = rdelMemo.find(elem);
        while (it != rdelMemo.end()) {  
            right.pop();
            elem = right.top();
            --it->second;
            if (it->second <= 0) {
                rdelMemo.erase(it);
            }

            it = rdelMemo.find(elem);
        }
        return elem;
    }


public:
    

    Median3() {}

    void insert(int x) {

        if (lsize == 0) {
            left.push(x);
            ++lsize;
            return;
        }

        int pivot = getLeftTop();

        if (x >= pivot) {
            // 放右边
            right.push(x);
            ++rsize;
        }
        else {
            // 放左边
            left.push(x);
            ++lsize;
        }
        
        // 保持左右平衡
        balance();

    }

    void erase(int x) {
        int pivot = getLeftTop();
        
        if (x <= pivot) {
            // 删除左边的
            ++ldelMemo[x];
            --lsize;
        }
        else {
            // 删除右边的
            ++rdelMemo[x];
            --rsize;
        }

        // 保持左右平衡
        balance();
    }

    double getMdian() {
        if (lsize == rsize) {
            double L = getLeftTop();
            double R = getRightTop();
            double res = (L + R) / 2.0f;
            return res;
        }
        return getLeftTop();
    }

};