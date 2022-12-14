#include "parser.hpp"

#include <iostream>
#include <string>
#include <sstream>

void Parser::prase(char *buffer)//Add every single word to Vector
{
    std::string input = buffer;
    std::istringstream iss(input);

    std::string sub;
    while(iss >> sub)
    {
        values.push_back(sub);
    }
}