// *****************************************************************
// @ Copyright 2020        December 14, 2021        Semester Project
// *****************************************************************
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / /  / / /
// *****************************************************************
// Data Science            Section N
// *****************************************************************
// / / / / / / / / / / / / / / / / / / / / / / / / / / / / /  / / /
// *****************************************************************
// Musa DAC i191765     Hassan Waqar i191895     Aqsa Rahman i191908
// *****************************************************************
#include<iostream>
#include<string>

using namespace std;

int max_allowed = 0; // A global var that controls the Order of B tree

// ************************ TEMPLATE BASED NODE STRUCTURE ************************* //
// This is a node structure that stores the keys and Directories of the Folders in which
// Data will be Stored
template <class T>
struct Node
{
    int max; // Individual var to store the Order of B tree so that Global is not accessed everytime
    bool _isleaf; // Var that tells that weather it is a Leaf node or not Leaf node are the once in which item can be inserted
    T* key; // Array of Order that contains keys or id / elements template based can be int or string
    string* directory; // Array of Order that contains directory of elements data stored of specfic key
    int sofarinserted; // &o far inserted is used to know how many are in the key inserted
    Node** sub_branch; // Sub childs of Root Also Nodes 2d Array
    // ********************* CONSTRUCTORS ************************ //
    Node() {
        max = max_allowed; // Node is Intialized with Pre Said max (global)
        key = new T[max];  // Array of keys of Size max
        directory = new string[max + 1]; // Array of keys of Size max
        sub_branch = new Node * [max + 1]; // Array of keys of Size max
        sofarinserted = 0;
    }
};


// ************************ TEMPLATE BASED CLASS BTree  ************************* //
// This class stores functions for insert, delete, remove, search, edit and others
template <class T>
class Bplustree {
private: // Private Fuctions have InsertInternal, removeInternal
    Node<T>* root; // Main Node of Template T as Root
    void insertInternal(T x, string dir, Node<T>* at, Node<T>* child) { // This
        // This Function gets an ID , a Directory where the data of that Id is Stored, and Child of Root in which
        // It has to be Stored and at which root we are
        if (at->sofarinserted < at->max) { // If the keys Inserted are less then Max Order
            int t = 0; // A tracker is intialized to track which key should it be added
            for (int i = 0; x > at->key[i] && i < at->sofarinserted; i++) {
                t = i; // Tracker is moved ahead if the id is greater then that already present at tracker position
            }
            for (int j = at->sofarinserted + 1; j > t + 1; j--)
            {
                at->sub_branch[j] = at->sub_branch[j - 1]; // and Sub Branches are also moved ahead
            }
            for (int j = at->sofarinserted; j > t; j--) {
                at->directory[j] = at->directory[j - 1]; // As well as Directories
                at->key[j] = at->key[j - 1]; // Keys are stored to add i in postion
            }
            at->key[t] = x; // and at tracker position Id
            at->directory[t] = dir; // dir and
            at->sub_branch[t + 1] = child; // branch is added
            at->sofarinserted++; // Incrementing number of keys added so far
        }
        else { // If the parent is also overflowed in node
            //create new internal node that goes into sub branch
            Node<T>* new_internal = new Node<T>;
            //create temp keys, dir, sub_class;
            T* temp_keys = new T[new_internal->max + 2]; // Array of key is made +2 for adding extra
            string* temp_dir = new string[new_internal->max + 2]; // Array of dir is made +2 for adding extra
            Node<T>** temp_subclass = new Node<T> *[new_internal->max + 2]; // Array of sub class is made +2 for adding extra
            for (int i = 0; i < new_internal->max + 1; i++) {
                temp_subclass[i] = at->sub_branch[i]; // moving all the sub classes are subbranches to temporary sub classes
            }
            for (int i = 0; i < new_internal->max; i++) {
                temp_keys[i] = at->key[i]; // moving keys moving keys from Child key to temporary key
                temp_dir[i] = at->directory[i]; // moving directories from Child directories to temporary directories
            }
            int i = 0, j; // Trackers
            for (int n = 0; x > temp_keys[n] && n < new_internal->max; n++) {
                n = i; // tracker is incremented until either it is below the max or the ID matches the temp key ID are the index tracker
            }

            for (int j = new_internal->max + 1; j > i; j--) {
                temp_keys[j] = temp_keys[j - 1]; //all the keys are sorted in temporary key
                temp_dir[j] = temp_dir[j - 1]; // all the directories are sorted in temporary directories
            }
            for (int j = new_internal->max + 2; j > i + 1; j--)
            {
                temp_subclass[j] = temp_subclass[j - 1]; //all the sub classes are also sorted
            }
            //All the sorting makes the position for a new ID to be inserted in it
            temp_keys[i] = x; // directories and keys are added at the tracker position
            temp_dir[i] = dir;
            temp_subclass[i + 1] = child;
            new_internal->_isleaf = false;
            //split cursor into two nodes
            at->sofarinserted = (new_internal->max + 1) / 2; //as it is a perfect split off right biased tree so divide by 2
            new_internal->sofarinserted = new_internal->max - (new_internal->max + 1) / 2;
            j = at->sofarinserted + 1;
            for (i = 0; i < new_internal->sofarinserted; i++) {
                new_internal->key[i] = temp_keys[j]; // X elements are given to the new internal key from the temp key
                new_internal->directory[i] = temp_dir[j];// X elements are given to the new internal key from the temp key
                j++;
            }
            j = at->sofarinserted + 1;
            for (i = 0; i < new_internal->sofarinserted + 1; i++) {
                new_internal->sub_branch[i] = temp_subclass[j];// X elements are given to the new internal key from the temp key
                j++;
            }
            if (at == root) {
                //if we are at a root node, we create a new root
                Node<T>* newRoot = new Node<T>; // New Root is Created
                newRoot->key[0] = at->key[at->sofarinserted]; // At Index 0 is added tacker no
                newRoot->directory[0] = at->directory[at->sofarinserted];
                newRoot->sub_branch[0] = at; // at branch is added
                newRoot->_isleaf = false; // leaf_node is declared false
                newRoot->sub_branch[1] = new_internal; // new internal is added at position 1
                root = newRoot; // root is equal to new root
                newRoot->sofarinserted = 1; // so far added is 1 at 0 position
            }
            else
            {
                //recursion
                insertInternal(at->key[at->sofarinserted], dir, findParent(root, at), new_internal);
                // find parents find the root of ptr at which we are currently available
            }
        }
    }
   bool removeInternal(T x, Node<T>* at, Node<T>* child)
    {
        if (at == root) { // removing is at root due to sub class
            if (at->sofarinserted == 1) {//if only one key is left, change root
                if (child->_isleaf == true) {
                    at->_isleaf = true; // child is a leaf root also  becomes leaf
                }
                if (at->sub_branch[1] == child) { // if the child and at subclasses matches then 1
                    root = at->sub_branch[0]; // Chandiing the root to the subbranch of node at which we are
                    delete[] child->key; // Deleting the Child key
                    delete[] child->directory; // Directories
                    delete[] child->sub_branch; // and it sub branches
                    delete child; // and Ultimatily deleting the whole Node Child
                    delete[] at->key; // Deleting the key of  the node at which we are
                    delete[] at->directory; // Deleting the key of  the node at which we are
                    delete[] at->sub_branch; // Deleting the sub classess of  the node at which we are
                    delete at; // Deleting the node at which we are
                    // Only left will be root all other will be deleted and cleaned
                    return true;
                }
                else if (at->sub_branch[0] == child) {
                    root = at->sub_branch[1]; // Changing the root to the subbranch of node at which we are at 1
                    delete[] child->key; // Deleting the Child key
                    delete[] child->sub_branch; // Deleting the Child Sub_branch
                    delete[] child->directory; // Deleting the Directory
                    delete child; // Deleting the Child
                    delete[] at->key; // Deleting the key of  the node at which we are
                    delete[] at->directory; // Deleting the Directories of  the node at which we are
                    delete[] at->sub_branch; // Deleting the Sub classes of  the node at which we are
                    delete at;
                    return  true;
                }
            }
        }
        int tracker = 0; // Tracker to keep Track
        for (tracker = 0; tracker < at->sofarinserted; tracker++) { // Tracker is Intialized to Zero and goes till sofarinserted
            if (at->key[tracker] == x) { // If tracker at key  == id
                break; // It breaks the for loop / stop
            }
        }
        for (int i = tracker; i < at->sofarinserted; i++) {
            at->key[i] = at->key[i + 1]; // keys are sorted
            at->directory[i] = at->directory[i + 1]; // directories are sorted
        }
        tracker = 0;
        while (tracker < at->sofarinserted + 1) {
            if (at->sub_branch[tracker] == child) { // tracker is again started to find the position in node at which we are
                break; // It breaks the for loop / stop
            }
            tracker++;
        }
        int tt = tracker;
        while (tt < at->sofarinserted + 1) {
            at->sub_branch[tt] = at->sub_branch[tt + 1]; // sorts all the values in node we are
            tt++;
        }
        at->sofarinserted--; // decreases the number if keys inserted in this
        if (at->sofarinserted >= (at->max + 1) / 2 - 1) { // Check wheather their is a Overflow
            cout << "Deleted " << x << " from node successfully" << endl; // if no overflow the node has been deleted
            return true; // and no further is supposed to be done
        }
        if (at == root) {
            return true;
        }
        Node<T>* parent = findParent(root, at); // finding the parent of node at which we are
        int lefttracker, righttracker; // to find left and right nodes keys
        tracker = 0;
        while (tracker < parent->sofarinserted + 1) {
            if (parent->sub_branch[tracker] == at) { // sub branch is find in parent
                lefttracker = tracker - 1; // when found left is found
                righttracker = tracker + 1; // when found right is found
                break; // and for loop is stopped
            }
            tracker++;
        }
        if (lefttracker >= 0) {//if left sibling exists
            Node<T>* leftNode = parent->sub_branch[lefttracker];
            //check if it is possible to transfer
            if (leftNode->sofarinserted >= (leftNode->max + 1) / 2) {
                for (int i = at->sofarinserted; i > 0; i--) {
                    at->key[i] = at->key[i - 1]; // sorted to make space for new one
                    at->directory[i] = at->directory[i - 1]; // sorted directories to make space for new one
                }
                for (int i = at->sofarinserted + 1; i > 0; i--) {
                    at->sub_branch[i] = at->sub_branch[i - 1];// Sub nodes are also arranged
                }
                //transfer ptr
                //transfer key from left sibling to parent
                at->key[0] = parent->key[lefttracker]; //
                at->directory[0] = parent->directory[lefttracker];
                parent->key[lefttracker] = leftNode->key[leftNode->sofarinserted - 1];
                parent->directory[lefttracker] = leftNode->directory[leftNode->sofarinserted - 1];
                //transfer last pointer from leftnode to at pointer
                at->sub_branch[0] = leftNode->sub_branch[leftNode->sofarinserted]; //leftnode is at position 0

                at->sofarinserted++;
                leftNode->sofarinserted--;
                return true;
            }
        }
        if (righttracker <= parent->sofarinserted) {//check if right sibling exist
            Node<T>* rightNode = parent->sub_branch[righttracker];
            //check if it is possible to transfer
            if (rightNode->sofarinserted >= (rightNode->max + 1) / 2)
            {
                //transfer key from right sibling to parent
                at->key[at->sofarinserted] = parent->key[tracker];
                at->directory[at->sofarinserted] = parent->directory[tracker];
                parent->key[tracker] = rightNode->key[0];
                parent->directory[tracker] = rightNode->directory[0];
                int i = 0;
                while (i < rightNode->sofarinserted - 1) {
                    rightNode->key[i] = rightNode->key[i + 1]; // Keys are sorted
                    rightNode->directory[i] = rightNode->directory[i + 1]; // Directories are sorted
                    i++;
                }
                at->sub_branch[at->sofarinserted + 1] = rightNode->sub_branch[0];
                i = 0;
                while (i < rightNode->sofarinserted) {
                    rightNode->sub_branch[i] = rightNode->sub_branch[i + 1]; // Sub branch are sorted
                    i++;
                }
                at->sofarinserted++; // inserted are increased
                rightNode->sofarinserted--; // right nodes are decreased
                return true;
            }
        }
        // if right node and left node wasn't possible merging will occur
        if (lefttracker >= 0) {
            //leftnode + parent key + cursor
            Node<T>* left = parent->sub_branch[lefttracker]; // leftnode is made upon left
            left->key[left->sofarinserted] = parent->key[lefttracker]; // leftnode key is and parent sub node is inserted
            left->directory[left->sofarinserted] = parent->directory[lefttracker]; // leftnode directories is and parent sub node is inserted
            int j = 0;
            for (int i = left->sofarinserted + 1; j < at->sofarinserted; j++) {
                left->key[i] = at->key[j]; // keys are transferred to leftNode
                left->directory[i] = at->directory[j]; // directories are transferred to leftNode
            }
            j = 0;
            for (int i = left->sofarinserted + 1; j < at->sofarinserted + 1; j++) {
                left->sub_branch[i] = at->sub_branch[j]; // all branches are also transferred
                at->sub_branch[j] = NULL; // sub branch at which we are is made null
            }
            left->sofarinserted += at->sofarinserted + 1;
            at->sofarinserted = 0;
            removeInternal(parent->key[lefttracker], parent, at); // recursion for other keys
        }
        else if (righttracker <= parent->sofarinserted)
        {
            // Same as above but for Right
            Node<T>* right = parent->sub_branch[righttracker];
            at->key[at->sofarinserted] = parent->key[righttracker - 1];
            at->directory[at->sofarinserted] = parent->directory[righttracker - 1];
            int j = 0;
            for (int i = at->sofarinserted + 1; j < right->sofarinserted; j++)
            {
                at->key[i] = right->key[j];
                at->directory[i] = right->directory[j];
            }
            j = 0;
            int i = at->sofarinserted + 1;
            while (j < right->sofarinserted + 1) {
                at->sub_branch[i] = right->sub_branch[j];
                right->sub_branch[j] = NULL;
                j++;
            }
            at->sofarinserted += right->sofarinserted + 1;
            right->sofarinserted = 0;
            removeInternal(parent->key[righttracker - 1], parent, right);
        }
        // Same as above but for Right
    }

    Node<T>* findParent(Node<T>* at, Node<T>* child) {
        Node<T>* parent = NULL; // Node is made of Parent intialized with NULL
        if (at->_isleaf == true || (at->sub_branch[0])->_isleaf == true) {
            return NULL; // If we are leaf return null
        }
        for (int i = 0; i < at->sofarinserted + 1; i++) { // for loop to go to all sub branches
            if (at->sub_branch[i] != child) // sub branch is not equal child
            {
                parent = findParent(at->sub_branch[i], child); // recursion
                if (parent != NULL) {
                    return parent; // parent is returned if not equal to NULL avoiding leaf nodes
                }
            }
            else
            {
                return at; // else on which we are returned
            }
        }
        return parent; // Parent is returned
    };

public:

    string search(T x) {
        if (root == NULL) {
            cout << "Tree is empty" << endl;
        }
        else {
            Node<T>* at = root;
            // Determine the Root at which we will start and move on
            while (at->_isleaf == false) {
                int i = 0; // i for while loop
                while (i < at->sofarinserted) { // runs for all key in specfic sub and key
                    if (x < at->key[i]) { // if loop to check is keys matches
                        at = at->sub_branch[i]; // at is moved to subbranch
                        break;
                    }
                    else if (i == at->sofarinserted - 1) {
                        at = at->sub_branch[i + 1]; // moved to right last sub branch
                        break;
                    }
                    i++; // i is incremented
                }
            }
            int k = 0; // k for while loop
            while (k < at->sofarinserted) { // all keys are searched in that leaf node
                if (at->key[k] == x) {
                    cout << "Found" << endl;
                    return at->directory[k];
                }
                k++;
            }
            cout << "Not found" << endl; // This might change
            return "";
        }
    }
    void insert(T x, string dir) // Insert Function that  takes Id and Directory where it is stored
    {
        if (root == NULL) {
            root = new Node<T>;
            root->directory[root->sofarinserted] = dir;
            root->key[root->sofarinserted] = x;
            root->_isleaf = true;
            root->sofarinserted++;
        }
        else { // If not Null then
            Node<T>* at = root; //
            Node<T>* parent_to_at = at;
            while (at->_isleaf != true) {
                // while loop runs until it arrives the leaf node
                parent_to_at = at; // parent = at
                int i = 0;
                while (i < at->sofarinserted) {
                    if (x < at->key[i]) {
                        at = at->sub_branch[i];
                        break;
                    }
                    else if (i == at->sofarinserted - 1) {
                        at = at->sub_branch[i + 1];
                        break;
                    }
                    i++;
                }
            }
            if (at->sofarinserted < at->max) {
                int t = 0; // Tracker to find corrrect Postion
                for (int i = 0; x > at->key[i] && i < at->sofarinserted; i++) {
                    t = i;
                }
                //make space for new key
                int j = at->sofarinserted; // j for while loop for making space for x
                while (j > t) { // while j is greater then t all keys are moved one step ahead
                    at->key[j] = at->key[j - 1]; // keys and
                    at->directory[j] = at->directory[j - 1]; // Directories are shifted one str\ep ahead
                    j--; // j decremented
                }
                at->key[t] = x; // at postion of tracker new value is added
                at->directory[t] = dir; // at postion of tracker new value is added
                at->sofarinserted += 1; // incremented
                at->sub_branch[at->sofarinserted] = at->sub_branch[at->sofarinserted - 1]; // Sub branch is move one step behind
                at->sub_branch[at->sofarinserted - 1] = NULL;
                //cout << "Inserted " << x <<endl;
            }
            else {
                // Detect Overflow
                Node<T>* newnode = new Node<T>; // A new node is created
                T* temp_key = new T[at->max + 2]; // Temp key is created due to overflow the size is kept bigger + 2
                string* temp_dir = new string[at->max + 2]; // Temp directory is created due to overflow the size is kept bigger + 2
                for (int i = 0; i < at->max; i++) {
                    temp_key[i] = at->key[i]; // all keys are transffered
                    temp_dir[i] = at->directory[i]; // all directoriies are transferred
                }
                int i = 0, j = newnode->max + 1; //
                for (int k = 0; x > temp_key[k] && k < newnode->max; k++) {
                    i = k;
                }
                while (j > i) {
                    temp_key[j] = temp_key[j - 1]; // all keys are moved
                    temp_dir[j] = temp_dir[j - 1]; // all directories are moved one step ahead
                    j--; // j is decremented
                }
                temp_key[i] = x;  // x is added i
                temp_dir[i] = dir; // dir is added i
                newnode->_isleaf = true; // as it is a leaf node marked as leaf
                at->sofarinserted = (at->max + 1) / 2; // Right Biased tree
                newnode->sofarinserted = at->max + 1 - (at->max + 1) / 2; // Others are in new node
                at->sub_branch[at->sofarinserted] = newnode; // at so far inserted is added right sub tree
                newnode->sub_branch[newnode->sofarinserted] = at->sub_branch[at->max]; // new node so far added is right
                at->sub_branch[at->max] = NULL; // and at max of subbranch of that tree is emptied
                i = 0;
                while (i < at->sofarinserted) {
                    at->key[i] = temp_key[i]; // keys are copies in at key
                    at->directory[i] = temp_dir[i]; // dir are copied in at key
                    i++; // i is incremented
                }
                j = at->sofarinserted; // j is equal to so far inserted in at
                i = 0; // i for while loop
                while (i < newnode->sofarinserted) { // new node is given it key from temp
                    newnode->key[i] = temp_key[j]; // tranffering of key
                    newnode->directory[i] = temp_dir[j]; // transferring of dir
                    j++; // j is incremented
                    i++; // i is incremented
                }
                if (at == root) {
                    Node<T>* newroot = new Node<T>;
                    newroot->key[0] = newnode->key[0]; // as B+ root new node and new root first index matches
                    newroot->directory[0] = newnode->directory[0]; // directory will also match
                    newroot->_isleaf = false; // is leaf
                    newroot->sub_branch[0] = at; // sub branch
                    newroot->sofarinserted = 1; // so far inserted
                    newroot->sub_branch[1] = newnode; // new sub tree added
                    root = newroot; // root is changed
                }
                else {
                    insertInternal(newnode->key[0], dir, parent_to_at, newnode); // Insert internal is at was not node
                }
            }
        }
    }
    //
    bool remove(T x) { //  Remove Functionality that removes if matches id (x)
        if (root == NULL) {
            cout << "Empty Tree" << endl;
            return false;
        }
        else {
            Node<T>* at = root;
            Node<T>* at_parent = at;
            int leftsib, rightsib; // these are to find number of keys in left and right node sub nodes
            while (at->_isleaf != true) {
                int i = 0; // for while loop
                while (i < at->sofarinserted) { // while loop runs until i is smaller then so far inserted
                    at_parent = at; // parent is equal at we are
                    rightsib = i + 1; //right node is present at +1 (subtree node)
                    leftsib = i - 1; //left node is present at -1 (subtree node)
                    if (x < at->key[i]) { // If key matches moves to next subbranch
                        at = at->sub_branch[i];
                        break;
                    }
                    else if (i == at->sofarinserted - 1) { // if matches at last
                        leftsib = i; // left sbiling is i
                        rightsib = i + 2; // and right is i + 2
                        at = at->sub_branch[i + 1]; // and moves to i + 1  sub branch
                        break;
                    }
                    i++; // incrementing
                }
            }
            bool found = false; // bool to know key has been found or not
            int position = 0; // position tracker
            for (int k = 0; k < at->sofarinserted; k++) { // for loop to find key in that node
                if (at->key[k] == x) {
                    found = true;
                    break;
                }
                position = k;
            }
            if (found == false) {//if key does not exist in that leaf node
                cout << "No key found" << endl;
                return false;
            }
            int i = position; // i for while loop instialized
            while (i < at->sofarinserted) { // while loop  shift one steo back
                at->key[i] = at->key[i + 1]; // keys shift back
                at->directory[i] = at->directory[i + 1]; //directoies shift back
                i++; // incremented i
            }
            at->sofarinserted--; // number of keys inserted decreases as deleted
            if (at == root) {//if it is root node, then make all pointers NULL
                i = 0;
                while (i < at->max + 1) {
                    at->sub_branch[i] = NULL; // their will be no sub tree all sub tree null
                    i++; // i increments
                }
                if (at->sofarinserted == 0) {//if all keys are deleted
                    delete[] at->key; // delete keys
                    delete[] at->directory; // delete directories
                    delete[] at->sub_branch; // delete sub_branch
                    delete at; // delete at node
                    root = NULL; // root is equal null
                }
                return true;
            }
            at->sub_branch[at->sofarinserted] = at->sub_branch[at->sofarinserted + 1]; // subbrach is moved one step back
            at->sub_branch[at->sofarinserted + 1] = NULL; // and which node was moved back it position is made null
            if (at->sofarinserted >= (at->max + 1) / 2) {
                return true; // if no underflow
            }
            if (leftsib >= 0)
            {
                Node<T>* leftNode = at_parent->sub_branch[leftsib];
                //check if it is possible to transfer
                if (leftNode->sofarinserted >= (leftNode->max + 1) / 2 + 1) {
                    i = at->sofarinserted;
                    while (i > 0) {// While loop until greater then 0
                        at->key[i] = at->key[i - 1]; // key is moved one step ahead
                        at->directory[i] = at->directory[i - 1]; // dir is moved one step ahead
                        i--; // i is decremented
                    }
                    at->sub_branch[at->sofarinserted] = at->sub_branch[at->sofarinserted - 1]; //sub class is also moved
                    at->sofarinserted++; // number of nodes is increased in at node
                    at->sub_branch[at->sofarinserted - 1] = NULL;
                    //transfer
                    at->key[0] = leftNode->key[leftNode->sofarinserted - 1]; // key is added at 0 of leftnode
                    at->directory[0] = leftNode->directory[leftNode->sofarinserted - 1]; // key is added ar 0
                    leftNode->sub_branch[leftNode->sofarinserted] = at; // leftnode sub tree is at node
                    leftNode->sofarinserted--; // decrease in key of leftnode
                    leftNode->sub_branch[leftNode->sofarinserted + 1] = NULL; // left node last tree decreases
                    at_parent->key[leftsib] = at->key[0]; // left sib is at key 0 B+ tree
                    at_parent->directory[leftsib] = at->directory[0]; // left sib is at dir 0 B+ tree
                    if (leftNode->_isleaf != false) {
                        at_parent->_isleaf = true;
                    }
                    return true;
                }
            }
            if (rightsib <= at_parent->sofarinserted) {
                Node <T>* rightNode = at_parent->sub_branch[rightsib];
                //check if it is possible to transfer
                if (rightNode->sofarinserted >= (rightNode->max + 1) / 2 + 1) {
                    at->sofarinserted++; //increment in so far added
                    at->sub_branch[at->sofarinserted] = at->sub_branch[at->sofarinserted - 1]; // sub class iis moved one step ahead
                    at->sub_branch[at->sofarinserted - 1] = NULL;// that freed is NULLify
                    at->key[at->sofarinserted - 1] = rightNode->key[0]; // key is equal to 0 of right
                    at->directory[at->sofarinserted - 1] = rightNode->directory[0]; // same for dir
                    rightNode->sofarinserted--; // right node is decreased
                    rightNode->sub_branch[rightNode->sofarinserted] = rightNode->sub_branch[rightNode->sofarinserted + 1]; // right node last node s are swapped
                    rightNode->sub_branch[rightNode->sofarinserted + 1] = NULL; // last sub node is nulled
                    i = 0;
                    while (i < rightNode->sofarinserted) {
                        rightNode->key[i] = rightNode->key[i + 1]; // keys are transffered 1 back step
                        rightNode->directory[i] = rightNode->directory[i + 1]; // directories are transffered 1 back step
                        i++;
                    }
                    at_parent->key[rightsib - 1] = rightNode->key[0]; //Due to B+ tree
                    at_parent->directory[rightsib - 1] = rightNode->directory[0]; // Due to B+ tree
                    if (rightNode->_isleaf != false) {
                        at_parent->_isleaf = true; // right node was leaf parent will become leaf
                    }
                    return  true;
                }
            }
            //must merge and delete a node
            if (leftsib >= 0) {//if left sibling exist
                Node<T>* templef = at_parent->sub_branch[leftsib];
                int j = 0;
                for (int i = templef->sofarinserted; j < at->sofarinserted; i++) {
                    templef->key[i] = at->key[j]; // all keys and dir are transffered
                    templef->directory[i] = at->directory[j];
                    j++;
                }
                templef->sub_branch[templef->sofarinserted] = NULL; // temp lef 2nd last branch made null
                templef->sofarinserted += at->sofarinserted;
                templef->sub_branch[templef->sofarinserted] = at->sub_branch[at->sofarinserted];
                removeInternal(at_parent->key[leftsib], at_parent, at);// delete parent node key
                delete[] at->key;
                delete[] at->directory;
                delete[] at->sub_branch;
                delete at;
            }
            else if (rightsib <= at_parent->sofarinserted) {//if right sibling exist
                Node<T>* tempri = at_parent->sub_branch[rightsib];
                int j = 0;
                for (int i = at->sofarinserted; j < tempri->sofarinserted; i++) {
                    at->key[i] = tempri->key[j];// all keys and dir are transffered
                    at->directory[i] = tempri->directory[j];
                    j++;
                }
                at->sub_branch[at->sofarinserted] = NULL;
                at->sofarinserted += tempri->sofarinserted;
                at->sub_branch[at->sofarinserted] = tempri->sub_branch[tempri->sofarinserted];
                removeInternal(at_parent->key[rightsib - 1], at_parent, tempri);// delete parent node key
                delete[] tempri->directory;
                delete[] tempri->key;
                delete[] tempri->sub_branch;
                delete tempri;
            }
        }
        return true;
    }
    ;
    void display(Node<T>* cursor)
    {
        if (cursor != NULL)
        {
            int i = 0;
            while (i < cursor->sofarinserted) {
                cout << cursor->key[i] << "-" << cursor->directory[i] << " ";
                i++;
            }
            cout << endl;
            if (cursor->_isleaf == false) {
                for (int i = 0; i < cursor->sofarinserted + 1; i++) {
                    try {
                        display(cursor->sub_branch[i]);
                    }
                    catch (string e) {
                        //
                    }

                }
            }
        }
    }

    Node<T>* getRoot() {
        return root;
    }

    void cleanUp(Node<T>* cursor) {
        if (cursor != NULL) {
            if (cursor->_isleaf == false) {
                int i = 0;
                while (i < cursor->sofarinserted + 1) {
                    cleanUp(cursor->sub_branch[i]);
                    i++;
                }
            }
            int i = 0;
            while (i < cursor->sofarinserted) {
                cout << "Deleted key from memory: " << cursor->key[i] << "\n";
                i++;
            }
        }
    }
    ~Bplustree()
    {
        //calling cleanUp routine
        cleanUp(root);
    }

    Bplustree() {
        root = NULL;
    }
};





