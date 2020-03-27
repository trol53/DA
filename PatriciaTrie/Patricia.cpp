#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include <fstream>

const int MAX_SIZE = 257;

using namespace std;

void ToLower(char *str){
    int size = strlen(str);
    for (int i = 0; i < size; i++){
        char tmp = str[i];
        str[i] = (char)tolower(tmp);
    }
}

bool CompareChar (char *s1, char *s2){
    int size1 = strlen(s1);
    int size2 = strlen(s2);
    if (size1 != size2)
        return false;
    for (int i = 0; i < size1; i++){
        if (s1[i] != s2[i])
            return false;
    }
    return true;
}

struct TNode {
    char *key;
    unsigned long long value;
    int bit;
    TNode *left;
    TNode *right;
    int index;
    int size;
    int str_size;
};

TNode* CreateNode (char *keys, unsigned long long values){
    TNode *tmp = new TNode();
    int length = strlen(keys);
    tmp->key = new char[length + 1];
    int i;
    ToLower(keys);
    for (i = 0; i < length; i++){
        tmp->key[i] = keys[i];
    }
    tmp->key[i] = '\0';
    tmp->value = values;
    tmp->bit = 0;
    tmp->left = nullptr;
    tmp->right = nullptr;
    tmp->size = 1;
    tmp->index = 0;
    tmp->str_size = length;    
    return tmp;
}

bool Digit (char *s, int bit, int size){
    int index = bit / 8;
    if (size <= index){
        return false;
    }
    char temp = s[index];
    return (temp >> (7 - bit % 8)) & 1;
}

int DifferentBit (char *s1, char *s2){
    int i;
    int size1 = strlen(s1);
    int size2 = strlen(s2);
    for (i = 0; Digit(s1,i,size1) == Digit(s2, i, size2); i++);
    return i;
}

TNode * Find(TNode *&root, char *key, int pred_bit){
    if (root->bit <= pred_bit){
        return root;
    }
    int size = strlen(key);
    if (Digit(key, root->bit, size)){
        return Find (root->right, key, root->bit);
    } else {
    return Find (root->left, key, root->bit);
    }
}

TNode* InsertR (TNode *&root, char *keys, unsigned long long values, int bits, int pred_bit){
    if (root->bit >= bits || root->bit <= pred_bit){
        TNode *tmp = CreateNode(keys, values);
        tmp->bit = bits;
        if (Digit(tmp->key, bits, tmp->str_size)){
            tmp->left = root;
            tmp->right = tmp;
        } else {
            tmp->left = tmp;
            tmp->right = root;
        }
        return tmp;
    }
    if (Digit(keys, root->bit, strlen(keys))){
        root->right = InsertR(root->right, keys, values, bits, root->bit);
    } else {
        root->left = InsertR(root->left, keys, values, bits, root->bit);
    }
    return root;
}

TNode* Insert (TNode *root, char *keys, unsigned long long values){
    if (root == nullptr){
        root = CreateNode(keys, values);
        root->left = root;
        cout << "OK\n";
        return root;
    }
    ToLower(keys);
    TNode *tmp = Find(root->left, keys, 0);
    if (CompareChar(keys, tmp->key)){
        cout << "Exist\n";
        return root;
    }
    root->left = InsertR(root->left, keys, values, DifferentBit(keys, tmp->key), 0);
    root->size++;
    cout << "OK\n";
    return root;
}

TNode* Parent (TNode *root, TNode *search, char &flag){
    TNode *prev = root;
    TNode *now = root->left;
    flag = 'l';
    while (now != search){
        if (Digit(search->key, now->bit, search->str_size)){
            prev = now;
            now = now->right;
            flag = 'r';
        } else {
            prev = now;
            now = now->left;
            flag = 'l';
        }
    }
    return prev;
}

TNode* CicleNode (TNode *root, TNode *search, char &flag, int pred_bit, TNode *pred){
    if (root->bit <= pred_bit){
        return pred;
    }
    if (Digit(search->key, root->bit, search->str_size)){
        flag = 'r';
        return CicleNode (root->right, search, flag, root->bit, root);
    } else {
        flag = 'l';
        return CicleNode (root->left, search, flag, root->bit, root);
    }
}

void Delete(TNode *&root, char *keys){
    if (root == nullptr){
        cout << "NoSuchWord\n";
        return;
    }
    TNode *delete_node = Find (root->left, keys, 0);
    if (!CompareChar(delete_node->key, keys)){
        cout << "NoSuchWord\n";
        return;
    }
    if (root->bit == root->left->bit){
        delete[] root->key;
        delete root;
        root = nullptr;
        cout << "OK\n";
        return;
    }
    char flag_p, flag_c1, flag_c2;
    if (delete_node->bit != root->bit){
        if (delete_node->bit == delete_node->left->bit){
            TNode *p = Parent(root, delete_node, flag_p);
            if (flag_p == 'l'){
                p->left = delete_node->right;
                delete_node->left = nullptr;
                delete[] delete_node->key;
                delete delete_node;
                delete_node = nullptr;
                cout << "OK\n";
                root->size--;
                return;
            } else {
                p->right = delete_node->right;
                delete_node->left = nullptr;
                delete[] delete_node->key;
                delete delete_node;
                delete_node = nullptr;
                cout << "OK\n";
                root->size--;
                return;
            }
        }
        if (delete_node->bit == delete_node->right->bit){
            TNode *p = Parent(root, delete_node, flag_p);
            if (flag_p == 'l'){
                p->left = delete_node->left;
                delete_node->right = nullptr;
                delete[] delete_node->key;
                delete delete_node;
                delete_node = nullptr;
                cout << "OK\n";
                root->size--;
                return;
            } else {
                p->right = delete_node->left;
                delete_node->right = nullptr;
                delete[] delete_node->key;
                delete delete_node;
                delete_node = nullptr;
                cout << "OK\n";
                root->size--;
                return;
            }
        }
    }
    TNode *swap_node = CicleNode(root->left, delete_node, flag_c1, 0, root);
    delete[] delete_node->key;
    delete_node->str_size = swap_node->str_size;
    int i;
    delete_node->key = new char[delete_node->str_size + 1];
    for (i = 0; i < delete_node->str_size; i++){
        delete_node->key[i] = swap_node->key[i];
    }
    delete_node->key[i] = '\0';
    delete_node->value = swap_node->value;
    TNode *p = Parent(root, swap_node, flag_p);
    TNode *swap_cicle = CicleNode(root->left, swap_node, flag_c2, 0, root);
    if (flag_c2 == 'l'){
        if (flag_c1 == 'l'){
            swap_cicle->left = swap_node->left;
        } else {
            swap_cicle->left = swap_node->right;
        }
    } else {
        if (flag_c1 == 'l'){
            swap_cicle->right = swap_node->left;
        } else {
            swap_cicle->right = swap_node->right;
        }
    }
    if (flag_p == 'l'){
        if (flag_c1 == 'l'){
            p->left = swap_node->right;
        } else {
            p->left = swap_node->left;
        }
    } else {
        if (flag_c1 == 'l'){
            p->right = swap_node->right;
        } else {
            p->right = swap_node->left;
        }
    }
    delete[] swap_node->key;
    delete swap_node;
    swap_node = nullptr;
    cout << "OK\n";
    root->size--;
    return;
}

void Read (TNode *&tree, char *read){
    unsigned long long val;
    cin >> read >> val;
    tree = Insert(tree, read, val);
    return;
}

void Remove (TNode *&root, char *read){
    cin >> read;
    ToLower(read);
    Delete(root, read);
    return;
}

void Search (TNode *root, char *read){
    if (root == nullptr){
        cout << "NoSuchWord\n";
        return;
    }
    ToLower(read);
    TNode *f = Find(root->left, read, 0);
    if (CompareChar(f->key, read)){
        cout << "OK: " << f->value << '\n';
    } else {
        cout << "NoSuchWord\n";
    }
    return;
}

void DelTree (TNode *&node){
    if (node == nullptr)
        return;
    if (node->bit == 0){
        delete[] node->key;
        delete node;
        node = nullptr;
        return;
    }
    if (node->right->bit > node->bit) {
        DelTree(node->right);
    }
    if (node->left->bit > node->bit) {
        DelTree(node->left);
    }
    delete[] node->key;
    delete node;
    node = nullptr;
}

void SetIndex (TNode *&tree, int &ind){
    if (tree->bit == 0){
        tree->index = ind;
        return;
    }
    tree->index = ind;
    ind++;
    if (tree->left->bit > tree->bit){
        SetIndex(tree->left, ind);
    }
    if (tree->right->bit > tree->bit){
        SetIndex(tree->right, ind);
    }
}

void SaveR (TNode *node, ofstream &file){
    file << node->key << ' ' << node->value << ' ' << node->bit << ' ' << node->index << ' ';
    if (node->left->bit > node->bit){
        file << -1 << ' ';
    } else {
        file << node->left->index << ' ';
    }
    if (node->right->bit > node->bit){
        file << -1;
    } else {
        file << node->right->index;
    }
    file << '\n';
    if (node->left->bit > node->bit){
        SaveR(node->left, file);
    }
    if (node->right->bit > node->bit){
        SaveR(node->right, file);
    }
}

void Save (TNode *tree, ofstream &file){
    if (tree == nullptr){
        file << "Header" << '\0' << ' ' << 0 << '\n';
        return;
    }
    file << "Header" << '\0' << ' ' << tree->size << '\n';
    if (tree->left->bit <= tree->bit){
        file << tree->key << ' ' << tree->value << ' ' << tree->bit << ' ' << tree->index << ' ' << 0 << '\n';
    } else {
        file << tree->key << ' ' << tree->value << ' ' << tree->bit << ' ' << tree->index << ' ' << -1 << '\n';
        int ind = 1;
        SetIndex(tree->left, ind);
        SaveR (tree->left, file);
    }
}

TNode* Load (TNode **mass, ifstream &file, char *read){
    unsigned long long val;
    int bits, indexs, checkl, checkr;
    file >> read >> val >> bits >> indexs >> checkl >> checkr;
    mass[indexs] = CreateNode(read, val);
    mass[indexs]->bit = bits;
    mass[indexs]->index = indexs;
    if (checkl == -1){
        mass[indexs]->left = Load(mass, file, read);
    } else {
        mass[indexs]->left = mass[checkl];
    }
    if (checkr == -1){
        mass[indexs]->right = Load(mass, file, read);
    } else {
        mass[indexs]->right = mass[checkr];
    }
    return mass[indexs];
}

void SaveLoad (TNode *&tree, char *read){
    cin >> read;
    if (read[0] == 'S'){
        cin >> read;
        ofstream file;
        file.open(read, ios::binary);
        if (!file.is_open()) {
            cout << "ERROR: error create file\n";
            return;
        }
        Save(tree, file);
        file.close();
        cout << "OK\n";
    } else{
        cin >> read;
        ifstream file;
        file.open(read, ios::binary);
        if (!file.is_open()) {
            cout << "ERROR: error create file\n";
            return;
        }
        file >> read;
        char check[] = {'H', 'e', 'a', 'd', 'e', 'r', '\0'};
        if (!CompareChar(read, check)){
            cout << "ERROR: wrong file\n";
            file.close();
            return;
        }
        if (tree != nullptr){
            if (tree->left->bit != tree->bit){
                DelTree(tree->left);
                delete[] tree->key;
                delete tree;
                tree = nullptr;
            } else {
                delete[] tree->key;
                delete tree;
                tree = nullptr;
            }
        }
        int size;
        file >> size;
        if (size == 0){
            cout << "OK\n";
            return;
        }
        TNode **mass = (TNode **)malloc(size * sizeof(TNode *));
        unsigned long long val;
        int bits, indexs, checkl;
        file >> read >> val >> bits >> indexs >> checkl;
        mass[0] = CreateNode(read, val);
        mass[0]->bit = bits;
        mass[0]->index = indexs;
        if (checkl == 0){
            mass[0]->left = mass[0];
            tree = mass[0];
            free(mass);
            file.close();
            return;
        }
        mass[0]->left = Load (mass, file, read);
        tree = mass[0];
        free(mass);
        file.close();
        cout << "OK\n";
    }
}

void Show(TNode *node, int i) {
    if (node == nullptr)
        return;
    if (node->bit == 0){
        return;
    }
    if (node->right->bit > node->bit) {
        Show(node->right, i + 1);
    }
    for (int j = 0; j < i*6; ++j) {
        cout << ' ';
    }
    cout << node->key << " " << node->index << " "; 
    std::cout << '\n';
    if (node->left->bit > node->bit) {
        Show(node->left, i + 1);
    }
}

int main(){
    cin.tie(nullptr);
	ios::sync_with_stdio(false);
    TNode *tree = nullptr;
    char read[MAX_SIZE];
    while (cin >> read){
        switch (read[0])
        {
        case '+':
            Read(tree, read);
            break;
        case '-':
            Remove(tree, read);
            break;
        case '!':
            SaveLoad(tree, read);
            break;
        default:
            Search(tree, read);
            break;
        }
    }
    if (tree == nullptr){
        return 0;
    }
    if (tree->left->bit != tree->bit){
        DelTree(tree->left);
        delete[] tree->key;
        delete tree;
    } else {
        delete[] tree->key;
        delete tree;
    }
}