#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the height as a data member, plus
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
    int getHeight () const;
    void setHeight (int height);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int height_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), height_(1)
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
* A getter for the height of a AVLNode.
*/
template<class Key, class Value>
int AVLNode<Key, Value>::getHeight() const
{
    return height_;
}

/**
* A setter for the height of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setHeight(int height)
{
    height_ = height;
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
    void inorder(AVLNode<Key, Value> *root);
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    AVLNode<Key, Value>* leftRotate (AVLNode<Key, Value> *n,AVLNode<Key,Value> * parent);
    AVLNode<Key, Value>* rightRotate (AVLNode<Key, Value> *n,AVLNode<Key,Value> * parent);
    AVLNode<Key,Value> * inserthelper(AVLNode<Key, Value> *root, const std::pair<const Key, Value> &new_item,AVLNode<Key,Value> * parent);
    static AVLNode<Key, Value>* getPredecessor(AVLNode<Key, Value>* root); // TODO
    AVLNode<Key, Value>* removehelper(AVLNode<Key, Value>* root, const Key& key, AVLNode<Key,Value> * parent);
    int getBalance(AVLNode<Key, Value> *n);
    int suggestHeight(AVLNode<Key, Value> *n);
};

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    AVLNode<Key,Value> * parent = NULL;
    this->root_ = inserthelper(static_cast<AVLNode<Key, Value>*> (this->root_),new_item,parent);
}

template<typename Key, typename Value>
AVLNode<Key,Value> * AVLTree<Key, Value>::inserthelper(AVLNode<Key, Value> *node, const std::pair<const Key, Value> &new_item, AVLNode<Key,Value> * parent)
 {
    if(node==NULL){
        AVLNode<Key,Value>* new_node = new AVLNode<Key,Value>(new_item.first, new_item.second, parent);
        new_node->setRight(NULL);
        new_node->setLeft(NULL);
        return new_node;
    }

    if(new_item.first == node->getKey()){
        node->setValue(new_item.second);
        return node;

    }
    else if(new_item.first > node->getKey()){
        AVLNode<Key,Value> * temp = inserthelper(node->getRight(),new_item,node);
        node->setRight(temp);
    }
    else{
        AVLNode<Key,Value> * temp = inserthelper(node->getLeft(),new_item,node);
        node->setLeft(temp);
    }

    node->setHeight(suggestHeight(node));

    int balance = getBalance(node);

    if(balance > 1 && new_item.first < node->getLeft()->getKey()){
        AVLNode<Key,Value> * temp = rightRotate(node,parent);
        return temp;
    }

    else if(balance > 1 && new_item.first > node->getLeft()->getKey()){
        AVLNode<Key,Value> * temp = leftRotate(node->getLeft(),parent);
        node->setLeft(temp);
        temp = rightRotate(node,parent);
        return temp;
    }

    else if(balance < -1 && new_item.first > node->getRight()->getKey()){
        AVLNode<Key,Value> * temp = leftRotate(node,parent);
        return temp;
    }

    else if(balance < -1 && new_item.first < node->getRight()->getKey()){
        AVLNode<Key,Value> * temp = rightRotate(node->getRight(),parent);
        node->setRight(temp);
        temp = leftRotate(node,parent);
        return temp;
    }

    return node;
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key,Value> * parent = NULL;
    this->root_ = removehelper(static_cast<AVLNode<Key, Value>*> (this->root_), key, parent);
}

template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::removehelper(AVLNode<Key, Value>* root, const Key& key, AVLNode<Key,Value> * parent)
{
    if(root == NULL){
        return root;
    }

    if(key < root->getKey()){
        AVLNode<Key, Value>* temp = removehelper(root->getLeft(), key,root);
        root->setLeft(temp);
    }

    else if(key > root->getKey()){
        AVLNode<Key, Value>* temp = removehelper(root->getRight(), key, root);
        root->setRight(temp);
    }

    else{
        if(root->getLeft() == NULL || root->getRight() == NULL){
            AVLNode<Key, Value>* temp;

            if(root->getLeft() != NULL){
                nodeSwap(root, root->getLeft());
                temp = root;
                root = root->getParent();
                root->setLeft(NULL);
            }
            else if(root->getRight() != NULL){
                nodeSwap(root, root->getRight());
                temp = root;
                root = root->getParent();
                root->setRight(NULL);
            }
            else{
                temp = root;
                root = NULL;
            }
            delete(temp);
        }
        else{
            AVLNode<Key, Value>* temp = getPredecessor(root);
            nodeSwap(root, temp);
            root = temp;
            temp = removehelper(root->getLeft(), key, root);
            root->setLeft(temp);
        }
    }

    if(root == NULL)
        return root;

    root->setHeight(suggestHeight(root));

    int balance = getBalance(root);

    if(balance > 1 && getBalance(root->getLeft()) >= 0){
        AVLNode<Key, Value>* temp = rightRotate(root, parent);
        return temp;
    }

    if(balance>1 && getBalance(root->getLeft())<0){
        AVLNode<Key, Value>* temp = leftRotate(root->getLeft(), parent);
        root->setLeft(temp);
        temp = rightRotate(root, parent);
        return temp;
    }

    if(balance < -1 && getBalance(root->getRight())<= 0){
        AVLNode<Key, Value>* temp = leftRotate(root, parent);
        return temp;
    }

    if(balance<-1 && getBalance(root->getRight())>0){
        AVLNode<Key, Value>* temp = rightRotate(root->getRight(), parent);
        root->setRight(temp);
        temp = leftRotate(root, parent);
        return temp;
    }

    return root;
}

/**
* Rotates n down and to the left
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::leftRotate (AVLNode<Key, Value> *x,AVLNode<Key,Value> * pp)
{
    AVLNode<Key, Value>* y = x->getRight();
    AVLNode<Key, Value>* left = y->getLeft();

    //perform rotation
    y->setLeft(x);
    x->setRight(left);
    x->setHeight(suggestHeight(x));
    y->setHeight(suggestHeight(y));
    if(left!=NULL)
        left->setParent(x);
    x->setParent(y);
    y->setParent(pp);
    return y;
}

/**
* Rotates n down and to the right
*/
template<typename Key, typename Value>
AVLNode<Key, Value>* AVLTree<Key, Value>::rightRotate (AVLNode<Key, Value> *y,AVLNode<Key,Value> * parent)
{
    AVLNode<Key, Value>* x = y->getLeft();
    AVLNode<Key, Value>* right = x->getRight();
    x->setRight(y);
    y->setLeft(right);
    y->setParent(x);
    y->setHeight(suggestHeight(y));
    if(right!=NULL)
        right->setParent(y);
    x->setParent(parent);
    x->setHeight(suggestHeight(x));
    return x;
}

template<class Key, class Value>
AVLNode<Key, Value>*
AVLTree<Key, Value>::getPredecessor(AVLNode<Key, Value>* node)
{
    if (node && node->getLeft() != NULL) {
        node = node->getLeft();
        while (node->getRight() != NULL) {
            node = node->getRight();
        }
        return node;
    }
    else if(node){
        AVLNode<Key, Value>* parent = node->getParent();
        while(parent != NULL && node == parent->getLeft()){
            node = parent;
            parent = parent->getParent();
        }
        return parent;
    }
    else{
        return NULL;
    }
}

template<class Key, class Value>
int AVLTree<Key, Value>::getBalance(AVLNode<Key, Value> *n){
    if(n == NULL)
        return 0;
    int h1 = 0, h2 = 0;
    if(n->getLeft())
        h1 = n->getLeft()->getHeight();
    if(n->getRight())
        h2 = n->getRight()->getHeight();
    int diff = h1 - h2;
    return diff;
}

template<class Key, class Value>
int AVLTree<Key, Value>::suggestHeight(AVLNode<Key, Value> *node){
    int height = 0;
    if(node->getLeft()){
        height = node->getLeft()->getHeight();
    }
    if(node->getRight() && height < node->getRight()->getHeight()){
        height = node->getRight()->getHeight();
    }
    return 1 + height;
}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int tempH = n1->getHeight();
    n1->setHeight(n2->getHeight());
    n2->setHeight(tempH);
}

#endif
