
#include "_stdlib.h"
#include "lib.h"

#define N 26

typedef struct trieNode TrieNode;

typedef TrieNode* Trie;

TrieNode* makeTrienode(char data);

void freeTrienode(TrieNode* node);

TrieNode* insertTrie(TrieNode* root, char* word);

int searchTrie(TrieNode* root, char* word);

int isLeafNode(TrieNode* root, char* word);

void printTrie(TrieNode* root);

void printSearch(TrieNode* root, char* word);
