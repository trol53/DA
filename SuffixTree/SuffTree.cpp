#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <string>

class TNode {
        public:
        TNode (TNode *p, long long l, long long *r) {
            pred = p;
            left = l;
            right = r;
            leafend = -1;
        }
        TNode *pred;
        TNode *link;
        long long left;
        long long *right;
        long long leafend;
        std::map <char, TNode*> child;
};

class STree {
    public:
    STree(std::string &str){
        text = str; 
        rootend = new long long();
        *rootend = -1;
        root = new TNode(nullptr, -1, rootend);
        Build();
    }

    ~STree(){
        DeleteTree(root);
    }

    void Print(){
        //if (root->child.empty())
        //    return;
        PrintR(root, 0);
    }
    bool TextSearch(std::string &pattern){
        TNode *tmp = root;
        if (pattern.size() > text.size())
            return false;
        if (tmp->child.count(pattern[0]) == 0)
            return false;
        tmp = tmp->child[pattern[0]];
        long long i = 0, j = tmp->left;
        while (true){
            if (i == pattern.size())
                return true;
            if (j == *tmp->right + 1){
                if (tmp->child.count(pattern[i]) == 0){
                    return false;
                } else {
                    tmp = tmp->child[pattern[i]];
                    i++;
                    j = tmp->left + 1;
                    continue;
                }
            }
            if (pattern[i] != text[j]){
                return false;
            } else {
                i++;
                j++;
            }
        }
    }

     

    private:

    void PrintR(TNode *r, long long dpth){
        for (long long i = 0; i < dpth; ++i) {
            putchar('\t');
        }
        //if (r != root){
            std::cout << "node represents: ";
            for (long long i = r->left; i <= *r->right; i++) {   
                std::cout << text[i];
            }
        //}
        std::cout << '\n';

        for (auto it : r->child) {
            PrintR(it.second, dpth + 1);
        }
    }

    long long GetLength(TNode *req){
        return *req->right + 1 - req->left;
    }

    void Build(){
        activenode = root;
        lastnode = nullptr;
        activeEdge = -1;
        activeLength = 0;
        remainingcount = 0;
        for (long long i = 0; i < text.size(); i++){
            AddChar(i);
        }
    }

    bool GoEdge (TNode *curr) {
        if (activeLength >= GetLength(curr)){
            activeEdge += GetLength(curr); 
            activeLength -= GetLength(curr); 
            activenode = curr; 
            return true;
        }
        return false;
    }

    void AddChar(long long pos){
        leaf = pos;
        remainingcount++;
        lastnode = nullptr;
        while (remainingcount > 0){
            if (activeLength == 0)
                activeEdge = pos;
            if (activenode->child.count(text[activeEdge]) == 0){
                TNode *node = new TNode(activenode, pos, &leaf);
                node->link = root;
                activenode->child.insert(std::make_pair(text[activeEdge], node));
                if (lastnode != nullptr){
                    lastnode->link = activenode;
                    lastnode = activenode;
                }
            } else {
                TNode *next = activenode->child[text[activeEdge]];
                if (GoEdge(next))
                    continue;
                if (text[next->left + activeLength] == text[pos]){
                    if (lastnode != nullptr && activenode != root){
                        lastnode->link = activenode;
                        lastnode = activenode;
                    }
                    activeLength++;
                    break;
                }
                splitend = new long long();
                *splitend = next->left + activeLength - 1;
                TNode *split = new TNode (activenode ,next->left, splitend);
                split->link = root;
                activenode->child[text[activeEdge]] = split;
                TNode *tmp = new TNode(split, pos, &leaf);
                tmp->link = root;
                next->left += activeLength;
                split->child[text[pos]] = tmp;
                split->child[text[next->left]] = next;
                if (lastnode != nullptr)
                    lastnode->link = split;
                lastnode = split;
            }
            remainingcount--;
            if (activenode == root && activeLength > 0){
                activeLength--;
                activeEdge = pos - remainingcount + 1;
            } else if (activenode != root){
                activenode = activenode->link;
            }
        }
    }


    void DeleteTree(TNode *node){
        for (auto r : node->child){
            DeleteTree(r.second);
        }
        delete node;
    }

    std::string text;
    TNode *root;
    TNode *lastnode;
    TNode *activenode;
    long long *rootend;
    long long *splitend;
    long long activeEdge; 
    long long activeLength; 
    long long remainingcount;
    long long leaf;
};


int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::string pattern, text;
    std::cin >> pattern >> text;
    text = text + "~";
    STree suff(text);
    std::cout << suff.TextSearch(pattern) << '\n';
    //suff.Print();
    
    
}