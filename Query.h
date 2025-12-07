// include/Query.h
#pragma once

enum class QueryType {
    SELECT,
    INSERT,
    UPDATE,
    DELETE,
    CREATE_TABLE,
    DROP_TABLE,
    UNKNOWN
};

class Query {
public:
    QueryType type;
    virtual ~Query() = default;
};