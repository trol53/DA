#include <iostream>
#include <vector>
#include <map>
#include <cstdio>
#include <string>
#include <queue>
#include <fstream>
#include <ctime>

class TNode {
        public:
        TNode (TNode *p, long long l, long long r) {
            pred = p;
            left = l;
            right = r;
            leafend = -1;
            edgelength = 0;
        }
        TNode *pred;
        TNode *link;
        long long left;
        long long right;
        long long edgelength;
        long long leafend;

        std::map <char, TNode*> child;
};

class STree {
    public:
    STree(std::string &str){
        text = str;
        Build();
    }

    ~STree(){
        DeleteTree(root);
    }

    std::vector <long long> TextSearch(std::string &pattern){
        TNode *tmp = root;
        std::vector <long long> ans;
        ans.push_back(-1);
        if (pattern.size() > text.size())
            return ans;
        if (tmp->child.count(pattern[0]) == 0)
            return ans;
        tmp = tmp->child[pattern[0]];
        long long i = 0, j = tmp->left;
        while (true){
            if (i == pattern.size()){
                Get_Pos(tmp, ans);
                break;
            }
            if (j == tmp->right + 1){
                if (tmp->child.count(pattern[i]) == 0){
                    return ans;
                } else {
                    tmp = tmp->child[pattern[i]];
                    i++;
                    j = tmp->left + 1;
                    continue;
                }
            }
            if (pattern[i] != text[j]){
                return ans;
            } else {
                i++;
                j++;
            }
        }
        return ans;
    }


    std::vector <long long> PatternSearch(std::string &txt){
        std::vector <long long> ans(txt.size(), 0);
        TNode *active = root;
        std::pair <long long, TNode*> tmp;
        long long activesize = 0, checkway = 0;
        if (root->child.count(txt[0]) == 0){
            ans[0] = 0;
        }else{ 
            activesize = 1;
            active = active->child[txt[0]];
            if (txt.size() == 1){
                ans[0] = 1;
                return ans;
            }
            for (long long i = 1; i < txt.size(); i++){
                if (active->left + activesize > active->right){
                    if (active->child.count(txt[i]) == 0){
                        ans[0] = active->edgelength;
                        activesize = 0;
                        active = active->link;
                        break;
                    } else {
                        active = active->child[txt[i]];
                        activesize = 1;
                        if (i == txt.size() - 1){
                            ans[0] = active->pred->edgelength + activesize;
                            active = root;
                            activesize = 0;
                            break;
                        }
                        continue;
                    }
                }
                if (txt[i] == text[active->left + activesize]){
                    activesize++;
                    if (i == txt.size() - 1){
                            ans[0] = active->pred->edgelength + activesize;
                            active = root;
                            activesize = 0;
                            break;
                        }
                    continue;
                } else {
                    ans[0] = active->pred->edgelength + activesize;
                    if (active->pred->link == root || active->pred == root){
                        active = root;
                        activesize = 0;
                        break;
                    }
                    tmp = WalkDown(activesize, active);
                    activesize = tmp.first;
                    active = tmp.second;
                    checkway = active->pred->edgelength + activesize;
                    break;
                }
            }
        }
            for (long long i = 1; i < ans.size() ; i++){
                if (ans[i - 1] == 0){
                    if (active->child.count(txt[i]) == 0){
                        ans[i] = 0;
                        continue;
                    } 
                }
                if (ans[i - 1] == 1){
                    if (GetLength(active) == 1){
                        if (active->child.count(txt[i]) == 0){
                            ans[i] = 0;
                            active = root;
                            activesize = 0;
                            continue;
                        }
                    } else{
                        if (txt[i] != text[active->left + activesize] && text.size() != 2){
                            ans[i] = 0;
                            active = root;
                            activesize = 0;
                            continue;
                        }
                    }
                }
                for (long long j = i + checkway;; j++){
                    if (checkway != 0)
                        checkway = 0;
                    if (active == root) {
                        if (root->child.count(txt[j]) != 0){
                            activesize = 1;
                            active = root->child[txt[j]];
                            continue;
                        } else {
                            ans[i] = 0;
                            activesize = 0;
                            break;
                        }
                    }
                    if (active->left + activesize > active->right){
                        if (active->child.count(txt[j]) == 0){
                            ans[i] = active->edgelength;
                            active = active->link;
                            activesize = GetLength(active);
                            checkway = active->edgelength;
                            break;
                        } else {
                            active = active->child[txt[j]];
                            activesize = 1;
                            continue;
                        }
                    }
                    if (txt[j] == text[active->left + activesize]){
                        activesize++;
                        if (j == txt.size() - 1){
                            ans[i] = active->pred->edgelength + activesize;
                            if (active->pred->link == root || active->pred == root){
                                active = root;
                                activesize = 0;
                                break;
                            }
                            tmp = WalkDown(activesize, active);
                            activesize = tmp.first;
                            active = tmp.second;
                            checkway = active->pred->edgelength + activesize;
                            break;
                        }
                        continue;
                    } else {
                        ans[i] = active->pred->edgelength + activesize;
                        if (active->pred->link == root || active->pred == root){
                            active = root;
                            activesize = 0;
                            break;
                        }
                        tmp = WalkDown(activesize, active);
                        activesize = tmp.first;
                        active = tmp.second;
                        checkway = active->pred->edgelength + activesize;
                        break;
                    }
                }
        }
        return ans;
    } 
    

    private:

    void Get_Pos(TNode *suf, std::vector <long long> &ans){
        
        ans.clear();
        std::queue <TNode*> nodes;
        nodes.push(suf);
        while (!nodes.empty()){
            TNode *tmp = nodes.front();
            nodes.pop();
            if (tmp->child.size() != 0){
                for (auto i : tmp->child){
                    nodes.push(i.second);
                }
            } else {
                ans.push_back(text.size() - tmp->edgelength + 1);
            }
        }


        
    }

    std::pair<long long, TNode*> WalkDown(long long size, TNode *tmp){
        long long pos = 0;
        TNode *next = tmp->pred->link->child[text[tmp->left + pos]];
        while (size > GetLength(next)){
            size -= GetLength(next);
            pos += GetLength(next);
            next = next->child[text[tmp->left + pos]];
        }
        return std::make_pair(size, next);

    }

    void SetLength (TNode *node){
        if (node != root){
            node->edgelength = node->pred->edgelength + GetLength(node);
        }
        for (auto i : node->child){
            SetLength(i.second);
        }
            
    }

    long long GetLength(TNode *req){
        return req->right + 1 - req->left;
    }

    void Build(){
        rootend = -1;
        root = new TNode(nullptr, -1, rootend);
        leaf = text.size() - 1;
        activenode = root;
        lastnode = nullptr;
        root->link = nullptr;
        activeEdge = -1;
        activeLength = 0;
        remainingcount = 0;
        for (long long i = 0; i < text.size(); i++){
            AddChar(i);
        }
        SetLength(root);
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
        remainingcount++;
        lastnode = nullptr;
        while (remainingcount > 0){
            if (activeLength == 0)
                activeEdge = pos;
            if (activenode->child.count(text[activeEdge]) == 0){
                TNode *node = new TNode(activenode, pos, leaf);
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
                splitend = next->left + activeLength - 1;
                TNode *split = new TNode (next->pred ,next->left, splitend);
                split->link = root;
                activenode->child[text[activeEdge]] = split;
                TNode *tmp = new TNode(split, pos, leaf);
                tmp->link = root;
                next->left += activeLength;
                split->child[text[pos]] = tmp;
                split->child[text[next->left]] = next;
                next->pred = split;
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
    long long rootend;
    long long splitend;
    long long activeEdge; 
    long long activeLength; 
    long long remainingcount;
    long long leaf;
};


int main(){
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::string pattern, text;
    // std::ifstream file("input.txt"); 
    // getline(file,pattern);
    // getline(file, text);
    std::cin >> text;
    //unsigned long long begin = clock();
    // if (pattern.size() == 0 || text.size() == 0)
    //     return 0;
    text = text + "~";
    STree suff(text);
    int count = 1;
    // if (text.size() < pattern.size() - 1)
    //     return 0;
    while (std::cin >> pattern){
        std::vector <long long> v = suff.TextSearch(pattern);
        if (v[0] == -1){
            count++;
            continue;
        }
        std::cout << count << ':' << " " << v[0];
        for (long long i = 1; i < v.size(); i++){
            std::cout << ',' << " " << v[i];
        }    
        std::cout << '\n';
        count++;
    }
    //unsigned long long finish = clock();
    //std::cout << finish - begin << '\n';
}