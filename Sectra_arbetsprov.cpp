// Sectra_arbetsprov.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <locale>// std::tolower
#include "List.h"
#include <vector>


// Program takes its input as a file
const std::string file1 = "test1.txt";
const std::string file2 = "test2.txt";
const std::string file3 = "test3.txt";


// Check if there is a digit or not in a string
bool isNumber(const std::string& s)
{
    // remove this and only use for-loop?
    if (!isdigit(s[0])) {
        return false; // can't an integer if first digit is not a digit
    }
    if (s[0] == '0' && isdigit(s[0])) {
        return false; //first digit in an integer can't be 0, would not compile when e.g adding division operator
    }
    for (char const& ch : s) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return true;
}

// Any name consisting of alphanumeric characters should be allowed as register names
bool isAlphaNumeric(const std::string& s) {

    for (char const& ch : s) {
        if (std::isalnum(ch)) {
            return true;
        }
    }
    return false;
}

int getNumWords(const std::string& s) {

    //default declaration
    //a string can't contain negative quantity of words
    //choosiing -1 as default feels safe
    int num = -1;

    // iterate through string s, check whitespace, count words (strings) in s
    return num = std::distance(std::istream_iterator<std::string>(
        std::istringstream(s) >> std::ws),
        std::istream_iterator<std::string>());
}

// All input should be case insensitive
// I choose everything to be lowercase in this case
std::string toLowerCase(std::string s) {
    
    for (auto& c : s)
    {
        c = tolower(c);
    }
    std::cout << s << "\n";
    return s;
}

// the inputs/registers can only be alphanumeric or integers
bool isValidCommand(std::vector<std::string> v) {

    if (isAlphaNumeric(v[0])) {
        std::cout << "First register is alphanumeric\n";
    }
    else {
        std::cout << "First register is NOT alphanumeric\n";
        return false;
    }

    if (!isNumber(v[1])) {
        std::cout << "Operation is valid\n";
    }
    else {
        std::cout << "Operation is NOT valid\n";
        return false;
    }

    // Second register can be either "the name of a variable"
    // or a value that should be operated on first register
    if (isAlphaNumeric(v[2])) {
        std::cout << "Second register is alphanumeric\n";
    }
    else {
        std::cout << "Second register is NOT alphanumeric\n";
        return false;
    }
   
    std::cout << "\n";

    return true; // can't return true earlier, everything needs to be checked
}



// recursive function to access commands stored on the stack
int result(std::string word, List& L, std::vector<bool>&booleans, int r) {

    std::pair<std::string, std::string> command= L.getCommand(word,booleans);

    if (command.second == "add") {
        
        if (isNumber(command.first)) {         
            std::cout << command.first << "\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r += std::stoi(command.first);
            }
            else {
                return r += std::stoi(command.first) + result(word, L, booleans, r);
            }
        }
        
        if(!isNumber(command.first)) {       
            std::cout << command.first <<"\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r += result(command.first, L, booleans, r);
            }
            else {
                return r += result(word, L, booleans, r);
            }
        }
    }

    if (command.second == "subtract") {
        if (isNumber(command.first)) {      
            std::cout << command.first << "\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r -= std::stoi(command.first);
            }
            else {
                return r+=result(word, L, booleans, r) - std::stoi(command.first);;
            }
        }

        if (!isNumber(command.first)) {
            std::cout << command.first << "\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r -= result(command.first, L, booleans, r);
            }
            else {
                return r -= result(word, L, booleans, r);
            }
        }
    }


    if (command.second == "multiply") {
        if (isNumber(command.first)) {
            std::cout << command.first << "\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r *= std::stoi(command.first);
            }
            else {
                return r += (result(word, L, booleans, r) * std::stoi(command.first));
            }
        }

        if (!isNumber(command.first)) {
            std::cout << command.first << "\n\n";
            if (L.allRegisterCalculated(word, booleans)) {
                return r *= result(word, L, booleans, r);
            }
            else {
                return r += (result(word, L, booleans, r)*result(command.first, L, booleans, r));
            }
        }
    }
    return r;
}



void readFile(std::string file) {

    // Declarations constants
    const int PRINT_operation = 2;//two words, print + register
    const int CALC_operation = 3;//three words, register + operation + register/value (all three syntax used here)

    // Declarations of varables to read & handle/sort data from file
    std::ifstream inFile;
    List L1;
    std::string line;
    int res = 0;

    inFile.open(file);

    if (!inFile) {
        std::cout << "Can't open file!";
        exit(1); // error, terminate
    }

    while (std::getline(inFile, line)) {
        
        line = toLowerCase(line);

        std::string word;
        std::istringstream ss(line);
        std::vector<std::string> vectorWords{};

        while (ss >> word) {
            //std::cout << word <<std:: endl;
            vectorWords.push_back(word);
        }

        // variable used to detect if the program should perform/save operations
        // or print the operations stored
        int howmany = getNumWords(line);
        //std::cout << "How many words:" << howmany << "\n";

        if (howmany == CALC_operation) {
            std::cout << " --> Calculation command\n";

            if (isValidCommand(vectorWords)) {

                L1.add_first(vectorWords[0], vectorWords[1], vectorWords[2]);
            }

        }else if (howmany == PRINT_operation && vectorWords[0]=="print") {
            // NOTE: print lazy evaluaiton
            std::cout << "  --> Print command \n\n";

            std::cout << "Size: " << L1.getSize() << "\n";
      
            std::vector<bool> isEvaluated(L1.getSize(), false);

            std::cout << "\nResult: " << result(vectorWords[1], L1, isEvaluated, res) <<"\n\n" <<
                std::setfill('-') << std::setw(20) <<"\n";

        }else {
            std::cout << "  --> Wrong type of command or input\n\n";
        }
    }
    
    if (inFile.is_open()) {
        std::cout << "\n" << std::setfill('-')
            << std::setw(20) << "\n" << "Close file!\n\n";
        inFile.close();
    }
}


int main()
{
    std::cout << "File 1\n"<< std::setfill('-') << std::setw(20) << "\n";
    readFile(file1);
    std::cout << "\nFile 2\n"<< std::setfill('-') << std::setw(20) << "\n";
    readFile(file2);
    std::cout << "\nFile 3\n" << std::setfill('-') << std::setw(20) <<"\n";
    readFile(file3);

    return 0;
}
