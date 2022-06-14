/*
 * Name: Sohail Lokhandwala
 * Email: slokhandwala@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file provides the driver code to compress an input file
 * to the given output file.
 */

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

/**
* Count the occurances of each character in the file
* and return the frequencies as a vector
*
* @param *inFile input file object
* @param frequencies vector of frequencies of each char
*/
vector<int> countOccurances(FancyInputStream *inFile){
    
    // count frequencies 
    vector<int> frequencies(256,0);
    unsigned int count = 0;
    size_t filesize = inFile->filesize();

    // loop through the file, increment frequencies for 
    // each occurance of a character
    while(inFile->good() && count < filesize){
        unsigned char ch = inFile->read<char>();
        frequencies[ch] += 1;
        count++;
    }

    // reset file reader
    inFile->reset();

    // return vector
    return frequencies;
}

/**
* Encode each character in the input file and write it to the 
* output file using HCTree::encode helper function
*
* @param *inFile input file object
* @param &outFile output file object
* @param tree huffman tree object
*/
void encodeFile(FancyInputStream *inFile, FancyOutputStream &outFile, HCTree *tree){
    // initialize counters
    unsigned int count = 0;
    size_t filesize = inFile->filesize();

    // loop through the file
    while(inFile->good()==true && count < filesize){
        unsigned char ch = inFile->read<char>();
        // call helper function to encode current character
        tree->encode(ch, outFile);
        count++;
    }
}

/**
* Add the frequency vector as the header of the file
*
* @param frequencies vector of frequencies of each char
* @param *outFile output file object
*/
void addHeader(vector<int> frequencies, FancyOutputStream *outFile){
    // loop through frequencies vector
    for (unsigned int i = 0; i < frequencies.size(); i++){
        // write frequency as an unsigned short (16 bits) to output
        unsigned short sht = frequencies[i];
        outFile->write<unsigned short>(sht);
    }
}

/**
* Main function with driver code
*
* @param argc number of command line arguments
* @param *argv[] command line arguments
*/
int main(int argc, char* argv[]) {
    // throw error if input and output files not given
    if(argc != 3){
        error("Arguments must be input and output files");
    }

    // get input and output file names
    string ifile = argv[1];
    string ofile = argv[2];

    // create FIS object
    FancyInputStream *inFile = new FancyInputStream(ifile);

    // if file is empty, create and return empty output file
    if(inFile->filesize() == 0){
        FancyOutputStream *outFile = new FancyOutputStream(ofile);
        return 0;
    }

    // get freqeuncies vector
    vector<int> frequencies = countOccurances(inFile);

    // initialize and build tree
    HCTree *tree = new HCTree();
    tree->build(frequencies);

    // create FOS object
    FancyOutputStream *outFile = new FancyOutputStream(ofile);

    // add freq array as header of output file
    addHeader(frequencies, outFile);

    // add tree serialization as header of output file
    // tree->addHeader(outFile);

    // encode the file
    encodeFile(inFile, *outFile, tree);

    // free dynamically allocated memory
    delete inFile;
    delete outFile;
    delete tree;
    
    // return 0 success exit code 
    return 0;
}
