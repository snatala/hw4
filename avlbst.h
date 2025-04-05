#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key,Value>* node);
    void removeFix(AVLNode<Key,Value>* node, int8_t diff);
    void rotateRight(AVLNode<Key,Value>* node);
    void rotateLeft(AVLNode<Key,Value>* node);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO

    if(this->root_ == nullptr){
        AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second,nullptr);
        this->root_=newNode;
        newNode->setBalance(0);
        return;
    }

    AVLNode<Key,Value>* parentNode = nullptr;

    AVLNode<Key,Value>* currentNode = static_cast<AVLNode<Key,Value>*>(this->root_);

    while(currentNode != nullptr){
        parentNode = currentNode;

        if(new_item.first < currentNode->getKey()){
            currentNode = currentNode->getLeft();
        } else if (new_item.first > currentNode->getKey()){
            currentNode = currentNode->getRight();
        } else {
            currentNode->setValue(new_item.second);
            return;
        }
    }

    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second,parentNode);

    if(new_item.first<parentNode->getKey()){

        parentNode->setLeft(newNode);
    } else {
        parentNode->setRight(newNode);
    }

    if(parentNode->getBalance()==-1){
        parentNode->setBalance(0);
    } else if(parentNode->getBalance() == 1){
        parentNode->setBalance(0);
    } else if(parentNode->getBalance() == 0){
        if(newNode==parentNode->getLeft()){
            parentNode->setBalance(-1);
        } else if(newNode==parentNode->getRight()){
            parentNode->setBalance(1);
        }
        insertFix(newNode->getParent(), newNode);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* piv = node->getRight();
    if(piv != nullptr){
        node->setRight(piv->getLeft());

        if(piv->getLeft() != nullptr){
            piv->getLeft()->setParent(node);
        }

        piv->setParent(node->getParent());

        if(node->getParent()==nullptr){
            this->root_ = piv;
        } else if(node == node->getParent()->getLeft()){
            node->getParent()->setLeft(piv);
        } else {
            node->getParent()->setRight(piv);
        }

        piv->setLeft(node);
        node->setParent(piv);
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* node)
{
    AVLNode<Key,Value>* piv = node->getLeft();
    if(piv != nullptr){
        node->setLeft(piv->getRight());

        if(piv->getRight() != nullptr){
            piv->getRight()->setParent(node);
        }

        piv->setParent(node->getParent());

        if(node->getParent()==nullptr){
            this->root_ = piv;
        } else if(node == node->getParent()->getRight()){
            node->getParent()->setRight(piv);
        } else {
            node->getParent()->setLeft(piv);
        }

        piv->setRight(node);
        node->setParent(piv);
    }
}



template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key,Value>* parent, AVLNode<Key,Value>* node)
{
    if(parent==nullptr || parent->getParent()==nullptr){
        return;
    }

    AVLNode<Key,Value>* grandparent = parent->getParent();
    if(parent == grandparent->getLeft()){
        grandparent->setBalance(grandparent->getBalance()-1);

        if(grandparent->getBalance() == 0){
            return;
        } else if (grandparent->getBalance() == -1){
            insertFix(grandparent,parent);
        } else if(grandparent->getBalance() == -2){
            if(node == parent->getLeft()){
                rotateRight(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else if(node == parent->getRight()){
                rotateLeft(parent);
                rotateRight(grandparent);

                if(node->getBalance() == -1){
                    parent->setBalance(0);
                    grandparent->setBalance(1);
                    node->setBalance(0);
                } else if(node->getBalance() == 0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                } else if(node->getBalance() == 1){
                    parent->setBalance(-1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    } else if(parent == grandparent->getRight()){
        grandparent->setBalance(grandparent->getBalance()+1);

        if(grandparent->getBalance() == 0){
            return;
        } else if(grandparent->getBalance()==1){
            insertFix(grandparent,parent);
        } else if(grandparent->getBalance()==2){
            if(node==parent->getRight()){
                rotateLeft(grandparent);
                parent->setBalance(0);
                grandparent->setBalance(0);
            } else if(node==parent->getLeft()){
                rotateRight(parent);
                rotateLeft(grandparent);
                if(node->getBalance() == 1){
                    parent->setBalance(0);
                    grandparent->setBalance(-1);
                    node->setBalance(0);
                } else if(node->getBalance()==0){
                    parent->setBalance(0);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                } else if(node->getBalance() == -1){
                    parent->setBalance(1);
                    grandparent->setBalance(0);
                    node->setBalance(0);
                }
            }
        }
    }
}



/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    // TODO

    AVLNode<Key,Value>* nodeRemove = (AVLNode<Key, Value>*) (this->internalFind(key));

    if(nodeRemove == nullptr){
        return;
    }

    if(nodeRemove->getLeft() != nullptr && nodeRemove->getRight() != nullptr) {
        Node<Key,Value>* pre = BinarySearchTree<Key,Value>::predecessor(nodeRemove);
        nodeSwap(nodeRemove, (AVLNode<Key,Value>*)pre);
    }

    int diff = 0;

    AVLNode<Key,Value>* parentNode = nodeRemove->getParent();

    if(parentNode != nullptr){
        if(nodeRemove == parentNode->getLeft()){
            diff = 1;
        } else if(nodeRemove == parentNode->getRight()){
            diff = -1;
        }
    }



    if(nodeRemove->getLeft() == nullptr && nodeRemove->getRight() == nullptr){
        if(nodeRemove == this->root_){
            delete this->root_;
            this->root_ = nullptr;
        } else {
            Node<Key,Value>* parent = nodeRemove->getParent();
            if(parent->getLeft() == nodeRemove){
                parent->setLeft(nullptr);
            } else{
                parent->setRight(nullptr);
            }

            delete nodeRemove;
            nodeRemove = nullptr;
        }
    } else if(nodeRemove->getLeft() == nullptr&& nodeRemove->getRight() != nullptr){
        AVLNode<Key,Value>* child = nodeRemove->getRight();

        if(nodeRemove == this->root_){
            this->root_ = child;
            child->setParent(nullptr);
            delete nodeRemove;
            nodeRemove = nullptr;
        } else {
            AVLNode<Key,Value>* parent = nodeRemove->getParent();
            if(nodeRemove==parent->getLeft()){
                parent->setLeft(child);
            } else if(nodeRemove==parent->getRight()){
                parent->setRight(child);
            }

            child->setParent(parent);
            delete nodeRemove;
            nodeRemove = nullptr;
        }
        
    } else if(nodeRemove->getLeft() != nullptr&& nodeRemove->getRight() == nullptr){
        AVLNode<Key,Value>* child = nodeRemove->getLeft();

        if(nodeRemove == this->root_){
            this->root_ = child;
            child->setParent(nullptr);
            delete nodeRemove;
            nodeRemove = nullptr;
        } else {
            AVLNode<Key,Value>* parent = nodeRemove->getParent();
            if(nodeRemove==parent->getLeft()){
                parent->setLeft(child);
            } else if(nodeRemove==parent->getRight()){
                parent->setRight(child);
            }

            child->setParent(parent);
            delete nodeRemove;
            nodeRemove = nullptr;
        }
        
    }

    removeFix(parentNode, diff);

}



template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key,Value>* node, int8_t diff){
    if(node == nullptr){
        return;
    }

    AVLNode<Key,Value>* parentNode = node->getParent();
    int nodeDiff = 0;

    if(parentNode != nullptr){
        if(node == parentNode->getLeft()){
            nodeDiff = 1;
        } else if(node == parentNode->getRight()){
            nodeDiff = -1;
        }
    }

    if(diff == -1){
        if(node ->getBalance() + diff == -2){
            AVLNode<Key,Value>* tNode = node->getLeft();

            if(tNode->getBalance() == -1){
                rotateRight(node);
                node->setBalance(0);
                tNode->setBalance(0);
                removeFix(parentNode, nodeDiff);
            } else if(tNode->getBalance() == 0){
                rotateRight(node);
                node->setBalance(-1);
                tNode->setBalance(1);
            } else if(tNode->getBalance() == 1){
                AVLNode<Key,Value>* grandparentNode = tNode->getRight();

                rotateLeft(tNode);
                rotateRight(node);

                if(grandparentNode->getBalance() == 1){
                    node->setBalance(0);
                    tNode->setBalance(-1);
                    grandparentNode->setBalance(0);
                } else if(grandparentNode->getBalance() == 0){
                    node->setBalance(0);
                    tNode->setBalance(0);
                    grandparentNode->setBalance(0);
                } else if(grandparentNode->getBalance() == -1){
                    node->setBalance(1);
                    tNode->setBalance(0);
                    grandparentNode->setBalance(0);
                }
                removeFix(parentNode,nodeDiff);
            }
        } else if(node->getBalance() + diff == -1){
            node->setBalance(-1);
        } else if (node->getBalance()+diff == 0){
            node->setBalance(0);
            removeFix(parentNode,nodeDiff);
        }
    } else if(diff == 1){
        if(node->getBalance() +1 ==2){
            AVLNode<Key,Value>* tNode = node->getRight();

            if(tNode->getBalance() == 1){
                rotateLeft(node);
                node->setBalance(0);
                tNode->setBalance(0);
                removeFix(parentNode,nodeDiff);
            } else if(tNode->getBalance() == 0) {
                rotateLeft(node);
                node->setBalance(1);
                tNode->setBalance(-1);
            } else if(tNode->getBalance() == -1){
                AVLNode<Key,Value>* grandparentNode = tNode->getLeft();

                rotateRight(tNode);
                rotateLeft(node);

                if(grandparentNode->getBalance() == 1){
                    node->setBalance(-1);
                    tNode->setBalance(0);
                    grandparentNode->setBalance(0);
                } else if(grandparentNode->getBalance() == 0){
                    node->setBalance(0);
                    tNode->setBalance(0);
                    grandparentNode->setBalance(0);
                } else if(grandparentNode->getBalance() == -1){
                    node->setBalance(0);
                    tNode->setBalance(1);
                    grandparentNode->setBalance(0);
                }

                removeFix(parentNode, nodeDiff);
            }
        } else if(node->getBalance() + diff == 1){
            node->setBalance(1);
        } else if(node->getBalance() + diff == 0){
            node->setBalance(0);
            removeFix(parentNode, nodeDiff);
        }
    }
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
