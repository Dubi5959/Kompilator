#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>


struct variable{
    std::string name;
    long long address;
    bool isArray;
    bool isArg;

};

struct symbolTable{
    long long addressNumber;
    std::vector<std::pair<std::vector<variable>, std::string>> Scopes;
};





