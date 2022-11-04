#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 26

typedef struct TrieNode TrieNode;

struct TrieNode {
  char data;
  TrieNode* children[N];
  int is_leaf;
};

TrieNode* make_trienode(char data) {
  TrieNode* node = (TrieNode*)alloc(sizeof(TrieNode));
  for (int i = 0; i < N; i++) node->children[i] = NULL;
  node->is_leaf = 0;
  node->data = data;
  return node;
}

void free_trienode(TrieNode* node) {
  for (int i = 0; i < N; i++)
    if (node->children[i] != NULL) free_trienode(node->children[i]);
  free(node);
}

TrieNode* insert_trie(TrieNode* root, char* word) {
  TrieNode* temp = root;

  for (int i = 0; word[i] != '\0'; i++) {
    int idx = (int)word[i] - 'a';

    if (temp->children[idx] == NULL)
      temp->children[idx] = make_trienode(word[i]);
    else
      temp = temp->children[idx];
  }
  temp->is_leaf = 1;
  return root;
}

int search_trie(TrieNode* root, char* word) {
  TrieNode* temp = root;

  for (int i = 0; word[i] != '\0'; i++) {
    int position = word[i] - 'a';
    if (temp->children[position] == NULL) return 0;
    temp = temp->children[position];
  }
  if (temp != NULL && temp->is_leaf == 1) return 1;
  return 0;
}

int is_leaf_node(TrieNode* root, char* word) {
  // Checks if the prefix match of word and root
  // is a leaf node
  TrieNode* temp = root;
  for (int i = 0; word[i]; i++) {
    int position = (int)word[i] - 'a';
    if (temp->children[position]) {
      temp = temp->children[position];
    }
  }
  return temp->is_leaf;
}
