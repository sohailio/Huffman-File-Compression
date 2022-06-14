/*
 * Name: Sohail Lokhandwala
 * Email: slokhandwala@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file implements and builds a huffman tree with functions
 * to encode and decode characters.
 */

#include <stdio.h>
#include <iostream>
#include <queue>
#include "HCTree.hpp"

using namespace std;


/**
* Free dynamically allocated nodes using post order traversal
*
* @param *node node to begin deletion at
* @return 1 when node is deleted
*/
int postOrderDeletion(HCNode* node){
    
    // if the node is a leaf, delete it
    if (node->c0 == nullptr && node->c1 == nullptr){
        delete node;
        return 1;
    }

    // if the node has a 0 child, post order delete on it
    if (node->c0 != nullptr){
        if(postOrderDeletion(node->c0) == 1){
            node->c0 = nullptr;
        }   
    }

    // if the node has a 1 child, post order delete on it
    if (node->c1 != nullptr){
        if(postOrderDeletion(node->c1) == 1){
            node->c1 = nullptr;
        }
    }

    // post delete current node
    postOrderDeletion(node);
    
    // return 1 success
    return 1;

}

/**
* HCTree object destructor, calls helper function 
* postOrderDeletion() to free dynamically alloc memory
*/
HCTree::~HCTree(){
    postOrderDeletion(this->root);
}

/**
* Use the Huffman algorithm to build a Huffman coding tree.
* PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
*                frequency of occurrence of byte i in the input file.
* POSTCONDITION: root points to the root of the trie, and leaves[i]
*                points to the leaf node containing byte i.
*
* @param freqs frequency vector
*/
void HCTree::build(const vector<int>& freqs){
    // initialize priority queue, counters
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    unsigned char j = 0;

    //loop through frequencies
    for (unsigned int i = 0; i < freqs.size(); i++){
        // build a node for non-zero frequencies, push to priority queue
        if(freqs[i] != 0){
            HCNode *node = new HCNode(freqs[i], j);
            leaves[i] = node;
            pq.push(node);
        }
        // increment counter
        j++;
    }

    // initialize left and right child nodes
    HCNode *left = nullptr;
    HCNode *right = nullptr;

    while (pq.size()>1)
    {
        // pop from top of pq
        left = pq.top();
        pq.pop();

        // pop from top of pq
        right = pq.top();
        pq.pop();

        // initialize null char a
        unsigned char a = 0;

        // build internal node, with symbol as null char a
        HCNode *parent = new HCNode(left->count + right->count, a);
        parent->c0 = left;
        parent->c1 = right;
        left->p = parent;
        right->p = parent;

        // push internal back into the pq
        pq.push(parent);   
    }
    
    // if the pq isnt empty, the root is its only element
    if(pq.size() != 0){
        root = pq.top();
    }
    // if pq was empty, create a root node with null children
    else{
        root = new HCNode(0,0);
        root->c0 = nullptr;
        root->c1 = nullptr;
    }
    
}


/**
* Write to the given FancyOutputStream the sequence of bits coding the
* given symbol.
* PRECONDITION: build() has been called, to create the coding tree,
*               and initialize root pointer and leaves vector.
*
* @param symbol symbol to encode
* @param out output stream for the encoded bits
*/
void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    // get node corresponding to the symbol
    HCNode *node = leaves[symbol];
    HCNode *curr = node;

    // initialize a vector to store the encoding
    vector<unsigned char> encoding;
    unsigned char bit;

    // traverse up the tree till we reach the root
    while(curr != this->root){
        
        // if 0 child, add 0 to the front of encoding
        if(curr == curr->p->c0){
            bit = 0;
            encoding.insert(encoding.begin(),bit);
        }
        // if 1 child, add 1 to the front of encoding
        if(curr == curr->p->c1){
            bit = 1;
            encoding.insert(encoding.begin(),bit);
        }
        
        // set curr to parent
        curr = curr->p;
    }

    // cout << symbol << ": ";

    // write encoding to output 
    for (unsigned int i = 0; i < encoding.size(); i++){
        // write current bit
        bit = encoding[i];
        out.write_bit(bit);
        
        // debugging print statements
        // if(bit == 1){
        //     cout << "1";
        // }
        // if(bit == 0){
        //     cout << "0";
        // }
        
    }   
    // cout << endl;
}

/**
* Return symbol coded in the next sequence of bits from the stream.
* PRECONDITION: build() has been called, to create the coding tree, and
*               initialize root pointer and leaves vector.
*
* @param in input stream to find encoded bits
* @return a single char decoded from the input stream
*/
unsigned char HCTree::decode(FancyInputStream & in) const{
    // traverse down the tree, starting at root
    HCNode *curr = this->root;

    // while the current node has children
    while( curr->c1 != nullptr && curr->c0 != nullptr){
        // read next bit
        char bit = in.read_bit();
        
        // if bit is 1, traverse down 1 child
        if (bit == 1){
            curr = curr->c1;
            // cout << "1";
        }
        
        // if bit is 0, traverse down 0 child
        else if(bit == 0){
            curr = curr->c0;
            // cout << "0";
        }
    }

    // return symbol of leaf node
    return curr->symbol;
}

/**
* Add serialized header using helper function
*
* @param outFile output file object
*/
void HCTree::addHeader(FancyOutputStream *outFile){
    // call helper function
    this->serialize(root, outFile);
}

/**
* Generate and write huffman tree serialization to output file
*
* @param *curr node to start serialization
* @param outFile output file object
*/
void HCTree::serialize(HCNode *curr, FancyOutputStream *outFile){
    // if node is null, return
    if (curr == nullptr){
        return;
    }
    // if node is leaf, write 1 and symbol
    if (curr->c1 == nullptr && curr->c0 == nullptr){
        outFile->write_bit(1);
        outFile->flush();
        outFile->write<unsigned char>(curr->symbol);
    }
    // if node is internal, write 0
    else{
        outFile->write_bit(0);
    }
    // serialize on children
    this->serialize(curr->c0, outFile);
    this->serialize(curr->c1, outFile);
}


