#ifndef PARSER_H
#define PARSE_H

#include <vector>
#include <string>

class Parser
{
    public:
        std::vector<std::string> values;
        void prase(const char *buffer);
       
};

#endif // !PARSER_H