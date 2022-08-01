#include "assignment5.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

// takes a text file consisting of exactly one English word per line as parameter
// returns a WordsVector containing all the words in that file
WordsVector read_words_into_vector(const std::string& inFileName)
{   
    std::ifstream ifs(inFileName); // Create an input file stream

    if (!ifs.is_open()) { // Check that the file is open
        std::cout << "Could not open file " + inFileName << std::endl;
        throw std::runtime_error("Could not open file " + inFileName);
    }

    WordsVector words_vector;

    std::copy(std::istream_iterator<std::string>{ifs},              // start of input stream
              std::istream_iterator<std::string>{},                 // end of input stream
              std::inserter(words_vector, words_vector.begin()));   // destination
              // can also use: std::back_inserter(words_vector) alternatively
    return words_vector;
}

// takes a vector that stores words, counts the occurence of each word using a lambda function
WordsMap map_and_count_words_using_lambda(const WordsVector& wvec)
{
    WordsMap wmap;
    std::for_each(wvec.begin(),     // start of source
                  wvec.end(),       // end of source
                  // need to add wmap to the capture list of the lambda to use it inside the lambda
                  [&wmap](std::string s)  
                  {
                    // if the word doesn't yet exist in the map, create an entry for it
                    if(wmap.find(s) == wmap.end())
                    {
                        wmap[s] = 1;
                    }
                    // if the word already exists in map, increment its value by one
                    else 
                    {
                        wmap[s] = wmap[s] + 1;
                    }
                  }); // lambda expression
    return wmap;
}

// a functor is a class object that can be called like a function
class WordCountFunctor 
{   
    private:
        WordsMap wmap;

    public:    
        void operator()(std::string s) 
        {
            if(wmap.find(s) == wmap.end()) { wmap[s] = 1; }
            else { wmap[s] = wmap[s] + 1; }
        }

        WordsMap getmap() const { return wmap; }
};

// takes a vector that stores words, counts the occurence of each word using a functor
WordsMap map_and_count_words_using_functor(const WordsVector& wvec)
{
    WordCountFunctor wcf{};
    wcf = std::for_each(wvec.begin(),   // start of source
                        wvec.end(),     // end of source
                        wcf);           // a functor keeping trac of the frequencies
    
    return wcf.getmap();
}

// remove duplicate words from the word vector
WordsVector remove_duplicates(const WordsVector& words_vector)
{
    WordsVector words_vec{ words_vector }; // make a copy of the supplied words_vector
    // 1- use std::sort to sort words_vec alphabetically
    // so that we can locate the duplicate words in it.

    // 2- use std::unique to rearrange the words in the sorted words_vec
    // so that each word appears once in the front portion of words_vec.
    // store the returned iterator, which points to the element
    // immediately after all the unique elements in the front of words_vec.

    // 3- use std::vector's erase member function to erase the range of non-unique
    // words in words_vec, starting at the iterator stored in step 2 above
    // to the end of words_vec.
    return words_vec;
}