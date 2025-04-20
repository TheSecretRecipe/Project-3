// Project 3: Comparison of 2 Data Structures
// B-Tree Custom Implementation

#include "BTree.h"

using namespace std;

BTree::BTree() {
    root = new BTreeNode(true);
}

void BTree::traverse() {
    if (root) root->traverse();
}

void BTree::insert(string& k, const Player& player) {
    if (root->n == 2 * T - 1) {
        BTreeNode* s = new BTreeNode(false);
        s->children[0] = root;
        s->splitChild(0, root);
        int i = (s->keys[0] < k) ? 1 : 0;
        s->children[i]->insertNonFull(k, player);
        root = s;
    } else {
        root->insertNonFull(k, player);
    }
}

Player BTree::search(const string& k) {
    return root->searchUsername(k);
}

BTree::~BTree()
{
    BTreeNode* node = root;
    unordered_set<BTreeNode*> visited;

    while(node != nullptr && visited.find(node) == visited.end()) {
        for(int i = 0; i < 2*T; i++) {
            if(node->children[i] != nullptr && visited.find(node->children[i]) == visited.end()) {
                node = node->children[i];
            } else {
                delete node;
                visited.insert(node);
            }
        }
    }
}

BTreeNode::BTreeNode(bool isLeaf) {
    leaf = isLeaf;
    n = 0;
    for (int i = 0; i < 2 * T; ++i) children[i] = nullptr;
}

void BTreeNode::insertNonFull(string& k, const Player& player) {
    int i = n - 1;
    if (leaf) {
        while (i >= 0 && keys[i] > k) {
            keys[i + 1] = keys[i];
            players[i + 1] = players[i];
            i--;
        }
        keys[i + 1] = k;
        players[i + 1] = player;
        n++;
    } else {
        while (i >= 0 && keys[i] > k) i--;
        if (children[i + 1]->n == 2 * T - 1) {
            splitChild(i + 1, children[i + 1]);
            if (keys[i + 1] < k) i++;
        }
        children[i + 1]->insertNonFull(k, player);
    }
}

void BTreeNode::splitChild(int i, BTreeNode* y) {
    BTreeNode* z = new BTreeNode(y->leaf);
    z->n = T - 1;
    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = y->keys[j + T];
        z->players[j] = y->players[j + T];
    }
    if (!y->leaf) {
        for (int j = 0; j < T; j++)
            z->children[j] = y->children[j + T];
    }
    y->n = T - 1;
    for (int j = n; j >= i + 1; j--)
        children[j + 1] = children[j];
    children[i + 1] = z;
    for (int j = n - 1; j >= i; j--) {
        keys[j + 1] = keys[j];
        players[j + 1] = players[j];
    }
    keys[i] = y->keys[T - 1];
    players[i] = y->players[T - 1];
    n++;
}

void BTreeNode::traverse() {
    for (int i = 0; i < n; i++) {
        if (!leaf) children[i]->traverse();
        cout << "[" << keys[i] << ": " << players[i].rate << "]\n";
    }
    if (!leaf) children[n]->traverse();
}

Player BTreeNode::searchUsername(const string& k) {
    int i = 0;
    while (i < n && k > keys[i]) i++;
    if (i < n && keys[i] == k) return players[i];
    if (leaf)
    {
        Player p{};
        return p;
    }
    return children[i]->searchUsername(k);
}