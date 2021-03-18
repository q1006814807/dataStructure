#include<iostream>
using namespace std;

// 字典树
class TrieNode {
private:

    TrieNode* next[26];     // 孩子列表[0-25]表示['a'-'z']，nullptr表示无
    TrieNode* parent;       // 该结点的父结点/双亲结点
    char flag;              // 控制标记。-1 逻辑已删除； 0：作为中间链路；1：有效单词结束
    char c;                 // 当前字符
    int numsOfNext;         // 当前结点的孩子个数，叶结点为0

public:
    // 构造函数
    // param: [bool] isRoot - 初始化的根结点需要置true
    TrieNode(bool isRoot = false) {
        for (int i = 0; i < 26; ++i) next[i] = nullptr;
        flag = isRoot;
        parent = nullptr;
        c = '\0';
        numsOfNext = 0;
    }

    // 在字典树的根结点插入单词记录 (若存在则跳过，默认在单词结尾的字符结点置其控制标记flag为1)
    // param: [TrieNode*] root - 一般传字典树根结点
    // param: [string] word - 单词
    // return: [TrieNode*] - 返回插入的单词末尾字符结点
    static TrieNode* insert(TrieNode* root, string word) {
        auto ptr = root;
        for (char c : word) {
            if (ptr->next[c-'a'] == nullptr) {
                ptr->next[c-'a'] = new TrieNode();
                ptr->next[c-'a']->parent = ptr;
                ptr->next[c-'a']->c = c;
                ++ptr->numsOfNext;
            }
            ptr = ptr->next[c-'a'];
        }
        ptr->flag = true;
        return ptr;
    }

    // 在字典树的某个结点插入下一个字符结点 (若存在则跳过，需要指定其是否是单词结尾)
    // param: [TrieNode*] root - 字典树的某个结点
    // param: [char] c - 字符
    // param: [bool] isEnd - 是否以此为结尾够成新单词
    // return: [TrieNode*] - 返回插入的新字符结点
    static TrieNode* insert(TrieNode* root, char c, bool isEnd) {
        if (root->next[c-'a'] == nullptr) {
            root->next[c-'a'] = new TrieNode();
            root->next[c-'a']->parent = root;
            root->next[c-'a']->c = c;
            ++root->numsOfNext;
        }
        root->next[c-'a']->flag = isEnd;
        return root->next[c-'a'];
    }

    // 在字典树的根个结点查找单词记录 (需满足单词结尾的字符结点其控制标记flag为1)
    // param: [TrieNode*] root - 一般传字典树根结点
    // param: [string] word - 单词
    // return: [bool] - 是否存在
    static bool has(TrieNode* root, string word) {
        if (!root) return false;
        auto ptr = root;
        for (char c : word) {
            if (ptr->next[c-'a'] == nullptr || ptr->next[c-'a']->flag == -1) {
                return false;
            }
            ptr = ptr->next[c-'a'];
        }
        return ptr->flag == 1;
    }

    // 在字典树的某结点查找是否存在下一个指定的字符
    // param: [TrieNode*] root - 字典树的某结点
    // param: [char] c - 查找的字符
    // return: [bool] - 是否存在
    static bool has(TrieNode* root, char c) {
        if (!root) return false;
        if (root->next[c-'a'] == nullptr || root->next[c-'a']->flag == -1) {
            return false;
        }
        return true;
    }   

    // 物理上删除，删除在字典树的某结点的所有孩子记录
    // param: [TrieNode*] root - 字典树的某结点
    static void del_down(TrieNode* root) {
        for (int i = 0; i < 26; ++i) {
            if (root->next[i]) {
                del_down(root->next[i]);
                delete root->next[i];
                root->next[i] = nullptr;
            }
        }
        root->numsOfNext = 0;
    }

    // 逻辑上删除(flag置-1)，删除在字典树的某结点的所有孩子记录
    // param: [TrieNode*] root - 字典树的某结点
    static void del_down_logicly(TrieNode* root) {
        for (int i = 0; i < 26; ++i) {
            if (root->next[i] && root->next[i]->flag != -1) {
                del_down_logicly(root->next[i]);
                root->next[i]->flag = -1;
            }
        }
        root->numsOfNext = 0;
    }

    // 物理上删除 其 逻辑上已被删除的子树
    // param: [TrieNode*] root - 字典树的某结点
    static void del_down_forLogicly(TrieNode* root) {
        for (int i = 0; i < 26; ++i) {
            if (root->next[i] && root->next[i]->flag == -1) {
                del_down_forLogicly(root->next[i]);
                delete root->next[i];
                root->next[i] = nullptr;
            }
        }
    }

    // 物理上删除，在字典树的某子结点开始向上追溯父结点/双亲结点并删除，若父结点/双亲结点有其他分叉或其flag为1(作为单词结尾)则停止删除
    // param: [TrieNode*] root - 字典树的某结点
    // return: [TrieNode*] - 返回停止删除的父结点
    static TrieNode* del_up(TrieNode* root) {
        auto cur = root;
        do {
            char childChar = cur->c;
            cur = cur->parent;
            delete cur->next[childChar-'a'];
            cur->next[childChar-'a'] = nullptr;
            --cur->numsOfNext;

        } while (cur->flag <= 0 && cur->numsOfNext == 0);
        return cur;
    }

    // 逻辑上删除(flag置-1)，在字典树的某子结点开始向上追溯父结点/双亲结点并删除，若父结点/双亲结点有其他分叉或其flag为1(作为单词结尾)则停止删除
    // param: [TrieNode*] root - 字典树的某结点
    // return: [TrieNode*] - 返回停止删除的父结点
    static TrieNode* del_up_logicly(TrieNode* root) {
        auto cur = root;
        do {
            cur->flag = -1;
            cur = cur->parent;
            --cur->numsOfNext;

        } while (cur->flag <= 0 && cur->numsOfNext == 0);
        return cur;
    }

    //=========================================================
    // setter & getter 不做详细注释

    // 返回当前结点的下一个指定字符结点
    static TrieNode* getNextNode(TrieNode* root, char c) {
        return root->next[c-'a'];
    }

    // 返回当前结点的父结点/双亲结点
    static TrieNode* getParentNode(TrieNode* root) {
        return root->parent;
    }   

    // 返回当前结点的字符
    static char getChar(TrieNode* root) {
        return root->c;
    }   

    // 返回当前结点是否是单词结尾
    static bool isEnd(TrieNode* root) {
        return root->flag == 1;
    }   

    // 返回当前结点的分支next数量
    static int getNumsOfNext(TrieNode* root) {
        return root->numsOfNext;
    }   

    // 设置当前结点是否是单词结尾
    static void setEnd(TrieNode* root, bool isEnd) {
        root->flag = isEnd;
    }   
};