#pragma once

#include <any>

class ReturnException : public std::exception {
public:
    const std::any value;

    ReturnException(const std::any value) : value(std::move(value)) {
        
    }
};