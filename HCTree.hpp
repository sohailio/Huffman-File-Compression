/*
 * Name: CSE 100 Staff, Sohail Lokhandwala
 * Email: cs100sp22@ucsd.edu, slokhandwala@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file provides a skeleton for building a huffman tree and
 * encoding and decoding algorithms
 */

#ifndef HCTREE_HPP
#define HCTREE_HPP
#include <queue>
#include <vector>
#include <fstream>
#include "Helper.hpp"
using namespace std;

/**
 * A Huffman Code Tree class
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;

public:
    /**
     * Constructor, which initializes everything to null pointers
     */
    HCTree() : root(nullptr) {
        leaves = vector<HCNode*>(256, nullptr);
    }

    /**
    * HCTree object destructor, calls helper function 
    * postOrderDeletion() to free dynamically alloc memory
    */
    ~HCTree();

    /**
     * Use the Huffman algorithm to build a Huffman coding tree.
     * PRECONDITION:  freqs is a vector of ints, such that freqs[i] is the
     *                frequency of occurrence of byte i in the input file.
     * POSTCONDITION: root points to the root of the trie, and leaves[i]
     *                points to the leaf node containing byte i.
     *
     * @param freqs frequency vector
     */
    void build(const vector<int>& freqs);

    /**
     * Write to the given FancyOutputStream the sequence of bits coding the
     * given symbol.
     * PRECONDITION: build() has been called, to create the coding tree,
     *               and initialize root pointer and leaves vector.
     *
     * @param symbol symbol to encode
     * @param out output stream for the encoded bits
     */
    void encode(unsigned char symbol, FancyOutputStream & out) const;

    /**
     * Return symbol coded in the next sequence of bits from the stream.
     * PRECONDITION: build() has been called, to create the coding tree, and
     *               initialize root pointer and leaves vector.
     *
     * @param in input stream to find encoded bits
     * @return a single char decoded from the input stream
     */
    unsigned char decode(FancyInputStream & in) const;
    
    /**
    * Add serialized header using helper function
    *
    * @param outFile output file object
    */
    void addHeader(FancyOutputStream *outFile);
    
    /**
    * Generate and write huffman tree serialization to output file
    *
    * @param *curr node to start serialization
    * @param outFile output file object
    */
    void serialize(HCNode *curr, FancyOutputStream *outFile);
};
#endif // HCTREE_HPP
