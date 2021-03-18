
#include<iostream>
#include<vector>
#include<stack>
#include<queue>
#include<time.h>
using namespace std;

// 若使用自定义数据类型，请务必重载<, ==, =运算符，以及拷贝构造函数
template<typename T>
struct TreeNode {
    T key;
    TreeNode* left, * right;
    char lHeight, rHeight;

    TreeNode(T val) : key(val),
                        left(nullptr), right(nullptr), 
                        lHeight(0), rHeight(0) 
                        {}
};


// 若使用自定义数据类型，请务必重载<, ==, =运算符，以及拷贝构造函数
template<typename T>
class BalancedBinaryTree {
private:

    template<typename _T>
    friend class BSTIterator;
    
    TreeNode<T>* root; // 根结点
    size_t size;       // 结点数量
    TreeNode<T>* cache;
    
    // 左右孩子高度信息汇报与采纳
    inline void _pushUp(TreeNode<T>* root) {
        root->lHeight = root->left  ? max(root->left ->lHeight, root->left ->rHeight) + 1 : 0;
        root->rHeight = root->right ? max(root->right->lHeight, root->right->rHeight) + 1 : 0;
    }

    // 获取平衡状态
    int _getBalanceStatus(TreeNode<T>* root) {

        char diff = root->lHeight - root->rHeight;

        if (-1 <= diff && diff <= 1) return 0;

        // 特别注意：
        // 当在删除后需要进行调整时，可能会出现左子树或右子树的diff=0的情况
        // 若在左子树出现，则默认返回进行左左调整的状态值
        // 若在右子树出现，则默认返回进行右右调整的状态值

        // 左边高了
        if (diff > 1) {
            char leftDiff = root->left->lHeight - root->left->rHeight;
            if (leftDiff  == 1 || leftDiff == 0) return 2; // 左左
            else                                 return 1; // 左右
        }
        // 右边高了
        else {
            char rightDiff = root->right->lHeight - root->right->rHeight;
            if (rightDiff == 1) return -1; // 右左
            else                return -2; // 右右
        }
    }

    // 自旋调整
    TreeNode<T>* _adjust(TreeNode<T>* root, int status) {

        if (status == 2) {
            // 左左调整
            auto rNode = root;              // 大
            auto mNode = root->left;        // 中
            auto lNode = root->left->left;  // 小
            
            auto midRightNode = mNode->right; // 中间的右子树

            mNode->right = rNode;
            rNode->left  = midRightNode;

            _pushUp(rNode);
            _pushUp(mNode);
            return mNode;
        }
        else if (status == 1) {
            // 左右调整
            auto rNode = root;              // 大
            auto mNode = root->left->right; // 中
            auto lNode = root->left;        // 小

            auto midLeftNode  = mNode->left;
            auto midRightNode = mNode->right;

            lNode->right = midLeftNode;
            rNode->left  = midRightNode;

            mNode->left  = lNode;
            mNode->right = rNode;

            _pushUp(lNode);
            _pushUp(rNode);
            _pushUp(mNode);

            return mNode;
        }
        else if (status == -2) {
            // 右右调整
            auto lNode = root;               // 小
            auto mNode = root->right;        // 中
            auto rNode = root->right->right; // 大

            auto midLeftNode = mNode->left;

            mNode->left  = lNode;
            lNode->right = midLeftNode;

            _pushUp(lNode);
            _pushUp(mNode);

            return mNode;
        }
        else if (status == -1) {
            // 右左调整
            auto lNode = root;              // 小
            auto mNode = root->right->left; // 中
            auto rNode = root->right;       // 大

            auto midLeftNode  = mNode->left;
            auto midRightNode = mNode->right;

            mNode->left  = lNode;
            mNode->right = rNode;

            lNode->right = midLeftNode;
            rNode->left  = midRightNode;

            _pushUp(lNode);
            _pushUp(rNode);
            _pushUp(mNode);

            return mNode;
        }

        return root;
    }

    // 搜索目标结点
    TreeNode<T>* _search(TreeNode<T>* root, T& key) {
        if (root == nullptr)  return nullptr;
        if (root->key == key) return root;
        if (key < root->key)  return _search(root->left, key);
        else                  return _search(root->right, key);
    }

    // 插入目标结点
    TreeNode<T>* _insert(TreeNode<T>* root, T& key) {
        if (root == nullptr) {
            ++size;
            return new TreeNode<T>(key);
        }
        if (root->key == key) {
            root->key = key;
            return root;
        }
        if (key < root->key)  root->left  = _insert(root->left,  key);
        else                  root->right = _insert(root->right, key);

        _pushUp(root);
        int status = _getBalanceStatus(root);
        return _adjust(root, status);
    }

    // _remove的子方法，删除当前结点
    TreeNode<T>* _removing(TreeNode<T>* root) {
        // 自己作为叶结点
        if (root->lHeight == 0 && root->rHeight == 0) {
            delete root;
            return nullptr;
        }
        // 只有左孩子
        else if (root->rHeight == 0) {
            auto leftNode = root->left;
            delete root;
            return leftNode;
        }
        // 只有右孩子
        else if (root->lHeight == 0) {
            auto rightNode = root->right;
            delete root;
            return rightNode;
        }
        // 左右孩子都有
        else {
            
            // 此处_remove一定会收敛于上面3种情况之一

            // 若左子树高度大于右子树，则使用左子树最大值代替
            if (root->lHeight > root->rHeight) {
                auto leftMaxNode = _searchMax(root->left);
                root->key = leftMaxNode->key;
                root->left = _remove(root->left, leftMaxNode->key);

            }
            // 反之，则使用右子树最小值代替
            else {
                auto rightMinNode = _searchMin(root->right);
                root->key = rightMinNode->key;
                root->right = _remove(root->right, rightMinNode->key);
            }

            return root;
        }
    }

    // 搜索并删除目标结点
    TreeNode<T>* _remove(TreeNode<T>* root, T& key) {
        if (root == nullptr) {
            ++size;
            return nullptr;
        }

        if (root->key == key) {
            root = _removing(root);
            if (!root) return nullptr;
        }
        else if (key < root->key) root->left  = _remove(root->left,  key);
        else                      root->right = _remove(root->right, key);

        _pushUp(root);
        int status = _getBalanceStatus(root);
        return _adjust(root, status);
    }

    // 搜索小于目标key的最大结点
    TreeNode<T>* _searchLessThan(TreeNode<T>* root, T& key, TreeNode<T>* preNode = nullptr) {
        if (root == nullptr)  return preNode;
        
        if (key <= root->key) return _searchLessThan(root->left,  key, preNode);

        // root->key < key
        // 该root符合条件
        else                  return _searchLessThan(root->right, key, root);
    }

    // 搜索大于目标key的最小结点
    TreeNode<T>* _searchMoreThan(TreeNode<T>* root, T& key, TreeNode<T>* preNode = nullptr) {
        if (root == nullptr)  return preNode;
        
        if (root->key <= key) return _searchMoreThan(root->right,  key, preNode);

        // root->key > key
        // 该root符合条件
        else                  return _searchMoreThan(root->left, key, root);
    }

    // 工具方法，搜索该查找树的最大值
    TreeNode<T>* _searchMax(TreeNode<T>* root) {
        if (!root) return nullptr;

        while (root->right) {
            root = root->right;
        }

        return root;
    }

    // 工具方法，搜索该查找树的最小值
    TreeNode<T>* _searchMin(TreeNode<T>* root) {
        if (!root) return nullptr;

        while (root->left) {
            root = root->left;
        }

        return root;
    }

    // 工具方法，释放内存
    static void _clear(TreeNode<T>* node) {
        if (!node) return;
        _clear(node->left);
        _clear(node->right);
        delete node;
    }


public:

    BalancedBinaryTree() {
        root = nullptr;
        size = 0;
    }
    ~BalancedBinaryTree() {
        _clear(root);
    }

    void insert(T key) {
        root = _insert(root, key);
    }

    bool contain(T key) {
        auto res = _search(root, key);
        if (!res) return false;

        cache = res;
        return true;
    }

    // 请一定在使用contain后并且为真时使用
    T get() {
        return cache->key;
    }

    void remove(T key) {
        root = _remove(root, key);
        --size;
    }

    void clear() {
        _clear(root);
        root = nullptr;
        size = 0;
    }

    T searchLessThan(T key) {
        auto res = _searchLessThan(root, key);
        if (!res || res->key >= key) return key;
        return res->key;
    }

    T searchMoreThan(T key) {
        auto res = _searchMoreThan(root, key);
        if (!res || res->key <= key) return key;
        return res->key;
    }

    T begin() {
        if (size == 0) return T();
        return _searchMin(root)->key;
    }

    T end() {
        if (size == 0) return T();
        return _searchMax(root)->key;
    }

    size_t getSize() {
        return this->size;
    }

    // TreeNode* getRoot() {
    //     return root;
    // }

};

template<typename T>
class BSTIterator {
private:
    stack<TreeNode<T>*> stk;

public:
    BSTIterator(BalancedBinaryTree<T>& BST) {
        auto cur = BST.root;
        while (cur) {
            stk.push(cur);
            cur = cur->left;
        }
    }

    bool hasNext() {
        while (stk.size() && stk.top() == nullptr) {
            stk.pop();
        }

        return stk.size();
    }

    T next() {
        if (stk.empty()) return T();
        
        auto cur = stk.top(); 
        stk.pop();
        
        T& res = cur->key;

        cur = cur->right;
        while (cur) {
            stk.push(cur);
            cur = cur->left;
        }

        return res;
    }

};




// class Solution_isBalanced {
// public:
//     bool ans = true;
//     bool isBalanced(TreeNode* root) {
//         check(root);
//         return ans;
//     }

//     int check(TreeNode* root) {
//         if (!root) return 0;
//         if (ans == false) return 0;

//         int left  = check(root->left );
//         int right = check(root->right);

//         if (abs(left - right) > 1) {
//             ans = false;
//             return 0;
//         }

//         return max(left, right) + 1;
//     }
// };

// class Solution_isBST {
// public:
//     bool isValidBST(TreeNode* root) {
//         stack<TreeNode*> stack;
//         long long inorder = (long long)INT32_MIN - 1;

//         while (!stack.empty() || root != nullptr) {
//             while (root != nullptr) {
//                 stack.push(root);
//                 root = root -> left;
//             }
//             root = stack.top();
//             stack.pop();
//             // 如果中序遍历得到的节点的值小于等于前一个 inorder，说明不是二叉搜索树
//             if (root -> key <= inorder) {
//                 return false;
//             }
//             inorder = root -> key;
//             root = root -> right;
//         }
//         return true;
//     }
// };

// void randomTester() {
//     srand(unsigned(time(nullptr)));

    
//     Solution_isBalanced test_isBalanced;
//     Solution_isBST test_isBST;

//     const int testTimes = 100;
//     const int eachTimesMaxInsertionNums = 50000;

    
//     for (int i = 0; i < testTimes; ++i) {

//         BalancedBinaryTree BBT;
//         vector<int> memo; memo.reserve(eachTimesMaxInsertionNums);
//         vector<int> memo2; memo2.reserve(eachTimesMaxInsertionNums); 

//         for (int j = 0; j < eachTimesMaxInsertionNums; ++j) {
            
//             // 33% 概率 删除， 66概率增加
//             if ((rand() % 3) == 0 && memo.size()) {
//                 int idx = (rand() % memo.size());
//                 swap(memo[memo.size()-1], memo[idx]);
//                 int val = memo.back();
//                 memo.pop_back();
//                 memo2.push_back(-val);
//                 BBT.remove(val);
//             }
//             else {
//                 int num = rand();
//                 if (!BBT.contain(num)) {
//                     memo.push_back(num);
//                     memo2.push_back(num);
//                     BBT.insert(num);
//                 }
//             }

//             if (memo.size() != BBT.getSize()) {
//                 cout << "size error!" << endl;
//                 cout << "memoSize = " << memo.size() << ", BBTSize = " << BBT.getSize() << endl;
//                 cout << "===========" << endl;

//                 for (int num : memo2) {
//                     cout << num << ' ';
//                 }
//                 cout << endl;
//                 return;
//             }

//         }
//         cout << "test times : " << i+1 << endl;
//     }
// }


int main() {
    // 3905 -3905 17486 9164 9994 29460 -17486 22893 27856 27856
    // randomTester();


    BalancedBinaryTree<int> bbt;

    bbt.insert(55);
    bbt.insert(11);
    bbt.insert(33);
    bbt.insert(64);
    bbt.insert(76);
    bbt.insert(21);
    bbt.insert(44);

    cout << bbt.getSize() << endl;
    cout << bbt.begin() << endl;
    cout << bbt.end() << endl;
    cout << bbt.contain(45) << endl;
    cout << bbt.contain(11) << endl;
    bbt.remove(65);
    bbt.remove(64);
    bbt.remove(33);
    cout << bbt.getSize() << endl;
    cout << bbt.searchLessThan(76) << endl;


    BSTIterator<int> it(bbt);

    while (it.hasNext()) cout << it.next() << endl;
    
    


    system("pause");
}
