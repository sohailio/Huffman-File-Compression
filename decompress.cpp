/*
 * Name: Sohail Lokhandwala
 * Email: slokhandwala@ucsd.edu
 *
 * Sources Used: None.
 *
 * This file provides the driver code to decompress an input file
 * to the given output file.
 */

#include <iostream>
#include <vector>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

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
    // initialize vars
    int total_chars = 0;
    vector<int> frequencies(256,0);
    // loop over header
    for(int i = 0; i < 256; i++){
        // increment total_chars, set frequency of char i
        int sht = inFile->read<unsigned short>();
        frequencies[i]=sht;
        total_chars += sht;
    }
 

    // initialize and build tree
    HCTree *tree = new HCTree();
    tree->build(frequencies);

    // create FOS object
    FancyOutputStream *outFile = new FancyOutputStream(ofile);

    // decode input file
    unsigned int count = 0;
    // loop over input
    while(inFile->good() && total_chars != 0){
        // call helper to decode current char
        unsigned char ch = tree->decode(*inFile);
        // cout << ch;
        // write decoded ch to output file
        outFile->write<unsigned char>(ch);
        total_chars--;
        count++;
    }
    
    // cout << endl;
    // cout << "The count ended at: " << total_chars << endl;

    // free dynamically allocated memory
    delete inFile;
    delete outFile;
    delete tree;

    // return 0 successful exit code
    return 0;
}
