#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>

class Parser
{
    public:
        std::vector<std::string> values;
        void parse(char *buffer);
};

#endif // !PARSER_H
