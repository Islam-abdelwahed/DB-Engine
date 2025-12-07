// include/Parser.h
#pragma once
#include <string>
#include "Query.h"

class Parser {
public:
    Query* parse(const std::string& sqlText);
};