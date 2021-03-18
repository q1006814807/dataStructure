#include<iostream>
#include<vector>
#include<math.h>
using namespace std;

class SegmentTree {
private:

    int size_nums;
    int size_tree;

    int* arr_sum  = nullptr;
    int* arr_lazy = nullptr;
    bool* arr_isUpdate = nullptr;
    int*  arr_update   = nullptr;
    
    void _pushUp(int i) {
        arr_sum[i] = arr_sum[(i << 1) + 1] + arr_sum[(i << 1) + 2];
    }

    void _pushDown(int i, int lRange, int rRange) {
        int lChild = (i << 1) + 1;
        int rChild = (i << 1) + 2; 

        if (arr_isUpdate[i]) {
            // 下发左孩子
            arr_isUpdate[lChild] = true;
            arr_lazy[lChild]     = 0;
            arr_update[lChild]   = arr_update[i];
            arr_sum[lChild]      = arr_update[i] * lRange;

            // 下发右孩子
            arr_isUpdate[rChild] = true;
            arr_lazy[rChild]     = 0;
            arr_update[rChild]   = arr_update[i];
            arr_sum[rChild]      = arr_update[i] * rRange;

            arr_isUpdate[i] = false;
        }
        
        if (arr_lazy[i] != 0) {
            // 下发左孩子
            arr_lazy[lChild] += arr_lazy[i];
            arr_sum[lChild]  += arr_lazy[i] * lRange;

            // 下发右孩子
            arr_lazy[rChild] += arr_lazy[i];
            arr_sum[rChild]  += arr_lazy[i] * rRange;

            arr_lazy[i] = 0;
        }
    }


    // 初始化建树
    void _build(vector<int>& nums, int lPos, int rPos, int curNode) {
        if (lPos == rPos) {
            arr_sum[curNode] = nums[lPos];
            return;
        }

        int mPos = lPos + ((rPos - lPos) >> 1);

        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        _build(nums, lPos,   mPos, lChild);
        _build(nums, mPos+1, rPos, rChild);

        _pushUp(curNode);        
    }

    // 区间查询
    int _query(int queryLPos, int queryRPos, int lBound, int rBound, int curNode) {
        if (queryLPos <= lBound && rBound <= queryRPos) {
            return arr_sum[curNode];
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        _pushDown(curNode, boundMid-lBound+1, rBound-boundMid);

        int res = 0;

        if (queryLPos <= boundMid) res += _query(queryLPos, queryRPos, lBound, boundMid, lChild);
        if (queryRPos >  boundMid) res += _query(queryLPos, queryRPos, boundMid+1, rBound, rChild);

        return res;
    }

    // 区间增减
    void _add(int addLPos, int addRPos, int value, int lBound, int rBound, int curNode) {

        // 落在增加区间之内
        if (addLPos <= lBound && rBound <= addRPos) {
            arr_sum[curNode]  += value * (rBound - lBound + 1);
            arr_lazy[curNode] += value;
            return;
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        _pushDown(curNode, boundMid-lBound+1, rBound-boundMid);

        if (addLPos <= boundMid) _add(addLPos, addRPos, value, lBound, boundMid, lChild);
        if (addRPos >  boundMid) _add(addLPos, addRPos, value, boundMid+1, rBound, rChild);
        
        _pushUp(curNode);
    }

    // 区间修改
    void _update(int addLPos, int addRPos, int value, int lBound, int rBound, int curNode) {

        // 落在修改区间之内
        if (addLPos <= lBound && rBound <= addRPos) {
            arr_isUpdate[curNode] = true;
            arr_lazy[curNode] = 0;
            arr_sum[curNode] = value * (rBound - lBound + 1);
            arr_update[curNode] = value;
            return;
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        _pushDown(curNode, boundMid-lBound+1, rBound-boundMid);

        if (addLPos <= boundMid) _update(addLPos, addRPos, value, lBound, boundMid, lChild);
        if (addRPos >  boundMid) _update(addLPos, addRPos, value, boundMid+1, rBound, rChild);

        _pushUp(curNode);
    }


    // 检查输入下标是否有效
    bool _valid(int pos) {
        return pos >= 0 && pos < size_nums;
    }

    int _calcSize(int size) {
        return 2 * (int)pow(2, ceil(log2(size))) - 1;
    }

public:
    
    // [0, size-1]
    SegmentTree(int size) {
        if (size <= 0) return;

        this->size_nums = size;
        this->size_tree = _calcSize(size);

        arr_sum  = new int[this->size_tree]();
        arr_lazy = new int[this->size_tree]();
        arr_isUpdate = new bool[this->size_tree]();
        arr_update   = new int[this->size_tree]();
    }

    // [0, size-1]
    SegmentTree(vector<int>& nums) {
        if (nums.empty()) return;
        this->size_nums = nums.size();
        this->size_tree = _calcSize(nums.size());

        arr_sum  = new int[this->size_tree]();
        arr_lazy = new int[this->size_tree]();
        arr_isUpdate = new bool[this->size_tree]();
        arr_update   = new int[this->size_tree]();

        _build(nums, 0, this->size_nums-1, 0);
    }

    ~SegmentTree() {
        delete[] arr_sum;
        delete[] arr_lazy;
        delete[] arr_isUpdate;
        delete[] arr_update;
    }


    
    int query(int lPos, int rPos) {
        if (lPos > rPos || !_valid(lPos) || ! _valid(rPos)) return 0;

        return _query(lPos, rPos, 0, size_nums-1, 0);
    }

    void add(int lPos, int rPos, int value) {
        if (!_valid(lPos) || !_valid(rPos)) return;
        _add(lPos, rPos, value, 0, size_nums-1, 0);
    }

    void update(int lPos, int rPos, int value) {
        if (!_valid(lPos) || !_valid(rPos)) return;
        _update(lPos, rPos, value, 0, size_nums-1, 0);
    }

    void print() {
        for (int i = 0; i < size_tree; ++i) {
            cout << i << '\t';
        }
        cout << endl;

        for (int i = 0; i < size_tree; ++i) {
            cout << arr_sum[i] << '\t';
        }
        cout << endl;
    }

};

class SegmentTreeLite {
private:

    int size_nums;
    int size_tree;

    int* arr_sum  = nullptr;

    void _pushUp(int i) {
        arr_sum[i] = arr_sum[(i << 1) + 1] + arr_sum[(i << 1) + 2];
    }

    // 初始化建树
    void _build(vector<int>& nums, int lPos, int rPos, int curNode) {
        if (lPos == rPos) {
            arr_sum[curNode] = nums[lPos];
            return;
        }

        int mPos = lPos + ((rPos - lPos) >> 1);

        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        _build(nums, lPos,   mPos, lChild);
        _build(nums, mPos+1, rPos, rChild);

        _pushUp(curNode);        
    }

    // 区间查询
    int _query(int queryLPos, int queryRPos, int lBound, int rBound, int curNode) {
        if (queryLPos <= lBound && rBound <= queryRPos) {
            return arr_sum[curNode];
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        int res = 0;

        if (queryLPos <= boundMid) res += _query(queryLPos, queryRPos, lBound, boundMid, lChild);
        if (queryRPos >  boundMid) res += _query(queryLPos, queryRPos, boundMid+1, rBound, rChild);

        return res;
    }

    // 单点增减
    void _add(int target, int value, int lBound, int rBound, int curNode) {
        if (lBound == target && rBound == target) {
            arr_sum[curNode] += value;
            return;
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        if (target <= boundMid) _add(target, value, lBound, boundMid, lChild);
        else                    _add(target, value, boundMid+1, rBound, rChild);
        
        _pushUp(curNode);
    }

    // 区间修改
    void _update(int target, int value, int lBound, int rBound, int curNode) {
        if (lBound == target && rBound == target) {
            arr_sum[curNode] = value;
            return;
        }

        int boundMid = lBound + ((rBound - lBound) >> 1);
        int lChild = (curNode << 1) + 1;
        int rChild = (curNode << 1) + 2;

        if (target <= boundMid) _update(target, value, lBound, boundMid, lChild);
        else                    _update(target, value, boundMid+1, rBound, rChild);
        
        _pushUp(curNode);
    }


    // 检查输入下标是否有效
    bool _valid(int pos) {
        return pos >= 0 && pos < size_nums;
    }

    int _calcSize(int size) {
        return 2 * (int)pow(2, ceil(log2(size))) - 1;
    }

public:
    
    // [0, size-1]
    SegmentTreeLite(int size) {
        if (size <= 0) return;

        this->size_nums = size;
        this->size_tree = _calcSize(size);

        arr_sum  = new int[this->size_tree]();
    }

    // [0, size-1]
    SegmentTreeLite(vector<int>& nums) {
        if (nums.empty()) return;
        this->size_nums = nums.size();
        this->size_tree = _calcSize(nums.size());

        arr_sum  = new int[this->size_tree]();

        _build(nums, 0, this->size_nums-1, 0);
    }

    ~SegmentTreeLite() {
        delete[] arr_sum;
    }


    
    int query(int lPos, int rPos) {
        if (lPos > rPos || !_valid(lPos) || ! _valid(rPos)) return 0;

        return _query(lPos, rPos, 0, size_nums-1, 0);
    }

    void add(int pos, int value) {
        if (!_valid(pos)) return;
        _add(pos, value, 0, size_nums-1, 0);
    }

    void update(int pos, int value) {
        if (!_valid(pos)) return;
        _update(pos, value, 0, size_nums-1, 0);
    }

};


int main(){
    
    vector<int> nums{1,3,5};
    SegmentTreeLite st(nums);
    

    st.update(1, 2);
    int res = st.query(0, 2);
    cout << res << endl;


    system("pause");

    return 0;
}
