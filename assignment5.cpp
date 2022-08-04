#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>
#include <set>

#include "assignment5.h"

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
    std::sort(words_vec.begin(), words_vec.end());

    // 2- use std::unique to rearrange the words in the sorted words_vec
    // so that each word appears once in the front portion of words_vec.
    // store the returned iterator, which points to the element
    // immediately after all the unique elements in the front of words_vec.
    std::vector<std::string>::iterator last = std::unique(words_vec.begin(), words_vec.end());

    // 3- use std::vector's erase member function to erase the range of non-unique
    // words in words_vec, starting at the iterator stored in step 2 above
    // to the end of words_vec.
    words_vec.erase(last, words_vec.end());
    return words_vec;
}

// checks if a phrase is a palindrome 
bool is_palindrome(const std::string& phrase)
{
    // create a copy of the phrase
    std::string copy = phrase;

    // remove all characters that are not a letter
    auto it = std::remove_if(copy.begin(), copy.end(), [](char c){ return !isalpha(c); });  
    copy.erase(it, copy.end());
    
    // check if the remaining letters in the string are exactly the same, from end to beginning to beginning to end
    bool result = std::equal(copy.begin(), copy.end(),
                             copy.rbegin(), copy.rend(),
                             [](char a, char b) { return tolower(a) == tolower(b); });
    return result;
}

// same func as above, but uses the suggested method from assignment just for practice
// bool is_palindrome(const std::string& phrase)
// {
//     std::string temp;
        
//     std::remove_copy_if(phrase.begin(), 
//                         phrase.end(), 
//                         std::back_inserter(temp),
//                         [](char c){ return !isalpha(c); });  
    
//     std::transform(temp.begin(), 
//                    temp.end(), 
//                    temp.begin(), 
//                    [](char c) { return tolower(c); });

//     return std::equal(temp.begin(), temp.end(),
//                       temp.rbegin(), temp.rend(),
//                       [](char a, char b) { return a == b; });
// }

// count and return elements in vector that are of length n, using a lambda function
size_t count_using_lambda(const std::vector<std::string>& vec, int n)
{
    return std::count_if(vec.begin(), 
                         vec.end(), 
                         [&](std::string s) { return s.length() == n; });
}

struct EqualLengthStringFunctor 
{   
    int lengthToCompare;
    EqualLengthStringFunctor(int n) : lengthToCompare{n} {};

    bool operator()(std::string s) 
    {
        if(s.length() == lengthToCompare) { return true; }
        else { return false; }
    }
};

// count and return elements in vector that are of length n, using a functor
size_t count_using_Functor(const std::vector<std::string>& vec, int n)
{
    EqualLengthStringFunctor elsf{n};
    return std::count_if(vec.begin(), 
                         vec.end(), 
                         elsf);
}

bool freeFunc(std::string s, int n) { return s.length() == n; }

// count and return elements in vector that are of length n, using a free function
size_t count_using_Free_Func(const std::vector<std::string>& vec, int n)
{
    using namespace std::placeholders;
    auto freeFuncOneArgument { std::bind(freeFunc, _1, n) };

    return std::count_if(vec.begin(), 
                         vec.end(), 
                         freeFuncOneArgument);
}

void multisetUsingDefaultComparator(const std::vector<std::string>& vec)
{
    std::multiset<std::string> strSet; // an empty set

    // to print a sorted verstion of the supplied vector vec,
    // we first copy vec to our strSet and then print the strSet.

    // note: since std::multiset does not provide push_front or push_back members,
    // we can't use a front or back inserter when we copy vec to our empty strSet,
    // meaning that we must use a general inserter:

    std::copy(vec.begin(), vec.end(),                   // source start and finish
              std::inserter(strSet, strSet.begin()));   // destination start using
                                                        // a general inserter

    // create an ostream_iterator attached to cout, using a space " " as a separator
    std::ostream_iterator<std::string> out(std::cout, " ");

    // output the set elements to the cout
    std::copy(strSet.begin(), strSet.end(), out);
}

struct CustomMultiSetComparator
{
    bool operator() (const std::string& lhs, const std::string& rhs) const 
    {
        if (lhs.length() == rhs.length()) 
        {
            return lhs < rhs;
        }
    return lhs.length() < rhs.length();
  }
};

void multisetUsingMyComparator(const std::vector<std::string>& vec)
{
    std::multiset<std::string, CustomMultiSetComparator> strSet;

    std::copy(vec.begin(), vec.end(),                   
              std::inserter(strSet, strSet.begin()));  

    std::ostream_iterator<std::string> out(std::cout, " ");
    std::copy(strSet.begin(), strSet.end(), out);
}

std::vector<int> getnerate_Fibonacci(int n)
{
    
}