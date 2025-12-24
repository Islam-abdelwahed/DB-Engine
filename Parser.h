// include/Parser.h
#pragma once
#include <string>
#include "Query.h"
using namespace std;


class Parser {
public:
    Query* parse(const string& sqlText);
};
