// include/Query.h
#pragma once

enum class QueryType {
    SELECT,
    INSERT,
    UPDATE,
    DELETE,
    UNKNOWN
};

class Query {
public:
    QueryType type;
    virtual ~Query() = default;
};