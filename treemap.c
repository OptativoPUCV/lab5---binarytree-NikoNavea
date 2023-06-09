#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap *mapa = (TreeMap *) malloc(sizeof(TreeMap));
  if (mapa == NULL) exit(EXIT_FAILURE);
  mapa->root = NULL;
  mapa->current = NULL;
  mapa->lower_than = lower_than;
  //new->lower_than = lower_than;
  return mapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* newNode = createTreeNode(key, value);
  
  if(tree->root == NULL){
    tree->root = newNode;
    tree->current = newNode;
    return;
  }
  
  if (searchTreeMap(tree, key) != NULL) return;
  
  TreeNode *current = tree->root;
  TreeNode *parent = NULL;
  
  
  while(current){
    parent = current;
    
    if(!tree -> lower_than(key, current->pair->key)) current = current->right;
    else current = current->left;
  }

  newNode->parent = parent;

  if(!tree->lower_than(key, parent->pair->key)) parent = current->right;
  else parent->left = newNode;

  tree->current = newNode;
}

TreeNode * minimum(TreeNode * x){
  if (x == NULL) return NULL;
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node){
  if (node == NULL) return;
  
  if(node->left == NULL && node->right == NULL){
    if (node == tree->root) tree->root = NULL;
    else if (node == node->parent->left) node->parent->left = NULL;
    else node->parent->right = NULL;
    free(node);
  }
  
  else if (node->left == NULL || node->right == NULL){
    TreeNode* child = (node->left != NULL) ? node->left : node->right;
    child->parent = node->parent;

    if(node == tree->root){
      tree->root = child;
    }
    else if(node == node->parent->left){
      node->parent->left = child;
    }
    else{
      node->parent->right = child;
    }
    free(node);
  }
  else{
    TreeNode* minNode = minimum(node->right);
    node->pair->key = minNode->pair->key;
    node->pair->value = minNode->pair->value;
    removeNode(tree, minNode);
  }
  
  
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  tree->current = tree->root;
  if (tree->root == NULL){
    return NULL;
  }

  while(tree->current != NULL){
    if (is_equal(tree, tree->current->pair->key, key)){
      return (tree->current->pair);
    }

    if(tree->lower_than(tree->current->pair->key, key)){
      tree->current = tree->current->right;
    }
    else{
      tree->current = tree->current->left;
    }
  }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  if(tree == NULL) return NULL;
  TreeNode* current = tree->root;
  TreeNode* ubNode = NULL;
  while(current){
    if(is_equal(tree, current->pair->key, key)){
      return current->pair;
    }
    else if (tree->lower_than(key, current->pair->key)){
      ubNode = current;
      current = current->left;
    }
    else current = current->right;
  }
  
  if(ubNode == NULL){
    return NULL;
  }
  else{
    return ubNode->pair;
  }
}

Pair * firstTreeMap(TreeMap * tree) {
  if (tree == NULL) return NULL;
  TreeNode* current = tree->root;
  
  while (current->left != NULL){
    current = current->left;
  }
  tree->current = current->left;
  
  
  return current->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  if (tree == NULL || tree->current == NULL) return NULL;
  
  TreeNode* current = tree->current;

  if(current->right != NULL){
    current = current->right;
    while(current->left != NULL){
      current = current->left;
    }
    
  }
  else{
    TreeNode* parent = current->parent;
    while(parent != NULL && current == parent->right){
      current = parent;
      parent = parent->parent;
    }
    current = parent;
  }
  tree->current = current;
  
  return (current != NULL) ? current->pair : NULL;
}
