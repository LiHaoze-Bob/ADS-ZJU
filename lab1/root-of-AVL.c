#include <stdio.h>

typedef struct Node{
  int key;
  int height;
  struct Node *left;
  struct Node *right;
} Node;

Node nodes[20];
int nodeCount = 0;

Node *createNode(int key){
    Node *node = &nodes[nodeCount++];
    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

int max(int a, int b){
    return a > b ? a : b;
}

int getHeight(Node* root){
    return root == NULL ? 0 : root->height;
}

void updateHeight(Node *root){
    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
}

int getBalance(Node *root){
    if (root == NULL){
        return 0;
    }
    return getHeight(root->left) - getHeight(root->right);
}

Node* rotateRight(Node *y){
    Node *x = y->left;
    Node *B = x->right;

    x->right = y;
    y->left = B;
    updateHeight(y);
    updateHeight(x);
    return x;
}

Node* rotateLeft(Node *x){
    Node *y = x->right;
    Node *B = y->left;
    y->left = x;
    x->right = B;
    updateHeight(x);
    updateHeight(y);
    return y;
}

Node* insert(Node* root, int key){
  if (root == NULL){
    return createNode(key);
  }
  if (key < root->key){
    root->left = insert(root->left, key);
  }
  else if (key > root->key){
    root->right = insert(root->right, key);
  }
  updateHeight(root);
  int balance = getBalance(root);
    if (balance > 1 && key < root->left->key) {
        return rotateRight(root);
    }
    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);  
        return rotateRight(root);             
    }

    if (balance < -1 && key > root->right->key) {
        return rotateLeft(root);        
    }

    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);               
    }

    return root; 
}

int main(){
    Node* root = NULL;
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n; i++){
        int key;
        scanf("%d", &key);
        root = insert(root, key);
    }
    printf("%d", root->key);
    return 0;
}