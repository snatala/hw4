#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include <iostream>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* node){
  if (node==nullptr){
    return 0;
  } else{
    int leftHeight = height(node->left);
    int rightHeight = height(node->right);

    if (leftHeight > rightHeight){
      return leftHeight +1;
    } else {
      return rightHeight + 1;
    }
  }
}

bool equalPaths(Node * root)
{
  if (root ==NULL){
    return true;
  }

  if(root -> left == NULL && root->right == NULL){
    return true;
  }

  int left = height(root->left);
  int right = height(root->right);


  if(root->left != nullptr && root->right != nullptr && right != left){
    return false;
  } else{
    return equalPaths(root->left) && equalPaths(root->right);
  }

}

