#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>
#include <string>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        // Add your code here
        if (k.size() == 0) {
            return 0;
        }

        std::string stringCopy = k;
        std::string aString = "a";
        while (stringCopy.size() % 6 != 0) { //if not a multiple of 6, add zero values to the front until we get to a mod of 6
            stringCopy.insert(0, aString);
        }
        
        std::size_t valArray[stringCopy.size()]; //value array of size of stringCopy
        for (std::size_t i = 0; i < stringCopy.size(); i++) { //fill valArray with decimal equivalent to base36 vals
            valArray[i] = letterDigitToNumber(stringCopy[i]);
        }

        unsigned long long hashedValsArray[5] = {0, 0, 0, 0, 0}; //at most will be size of 5
        std::size_t valIndex = stringCopy.size() - 1; //start at last index of valArray
        bool shouldContinue = false;
        for (int i = 4; i >=0; i--) {
            if (shouldContinue) {
                continue;
            }
            //std::cout<<"Checking range: "<< valIndex << " to " << valIndex-5 << std::endl;
            hashedValsArray[i] = 36*(36*( 36*( 36*( 36*(valArray[valIndex-5]) + (valArray[valIndex-4])) + (valArray[valIndex-3])) + (valArray[valIndex-2])) + (valArray[valIndex-1])) + valArray[valIndex];
            if (valIndex - 6 > valIndex) {
                shouldContinue = true;
            }  else {
                valIndex -= 6;
            }
        }

        unsigned long long returnVal = 0;
        for (int i = 0; i < 5; i ++) {
            //std::cout << "w["<< i<< "] = " << hashedValsArray[i] << std::endl;
            returnVal += (rValues[i] * hashedValsArray[i]);
        }
        return returnVal;

    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        // Add code here or delete this helper function if you do not want it
        std::size_t returnVal = (std::size_t) std::tolower(letter); //ASCII value of letter
        if (returnVal >= 97) { //then letter is 'a' to 'z'
            returnVal = returnVal - 97;
        } else { //letter is '0' to '9'
            returnVal = returnVal - 22;
        }
        return returnVal;
    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
