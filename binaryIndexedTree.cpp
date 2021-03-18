#pragma once
#include<iostream>
#include<vector>
using namespace std;


/*
    性质： t[x] begin at 1
    1) t[x]结点覆盖的长度为len = lowbit(x);
    2) 树的每一层二进制数后的0位数相等，并且与其len相关
    3) t[x]的父节点为t[x + lowbit(x)]
    4) 树深度=logN+1
    5) 求[1, x]的区间和，则 以x为起始点 迭代加上x -= lowbit(x) 作为根的值即可
*/
class BIT{
private:

    int size;
    int *arr;

    static int lowbit(int x) {
        return x & -x;
    }


public:
    // constructor1
    BIT(int size){
        this->size = size;
        arr = new int[size+1]();
    }
    // constructor2
    BIT(vector<int>& nums, int size){
        arr = new int[size+1]();

        for (int i = 0; i < nums.size(); ++i) {
            this->add(i, nums[i]);
        }
    }

    // copy constructor
    BIT(BIT& object) {
        delete[] this->arr;

        this->size = object.size;
        this->arr = new int[this->size];

        for (int i = 0; i <= this->size; ++i) {
            this->arr[i] = object.arr[i];
        }
    }
    // free constructor
    ~BIT() {
        delete[] this->arr;
    }

    // 为[pos]增加值
    void add(int pos, int val) {
        ++pos;
        for (; pos <= size; pos += lowbit(pos)) arr[pos] += val;
    }

    // 返回[0, pos]前缀和/区间和
    int query(int pos) {
        ++pos;

        int res = 0;
        for (; pos > 0; pos -= lowbit(pos)) res += arr[pos];

        return res;
    }
    // 返回[lPos, rPos]区间和
    int query(int lPos, int rPos) {
        return query(rPos) - query(lPos-1);
    }

    // setter & getter
    int range() const { return this->size; }
    
    int operator[](int pos) {
        return query(pos);
    }

};

int main () {

    BIT bitt(10);
    int nums[10] = {1,2,3,4,5,6,7,8,9,10};

    for (int i = 0; i < 10; ++i) {
        bitt.add(i, nums[i]);
    }

    cout << bitt.query(1,3) << endl;

    return -0;

}
