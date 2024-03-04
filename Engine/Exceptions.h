#pragma once
#include <stdexcept>
#include <string>

class BaseException : public std::exception {
public:
    BaseException(const std::string& message) : m_Message(message) {}

    const char* what() const noexcept override {
        return m_Message.c_str();
    }

private:
    std::string m_Message;
};

class TooManyComponentsException final : public BaseException {
public:
    TooManyComponentsException() : BaseException("Attempted to add too many components.") {}
};

class SetParentIsInvalidException final : public BaseException {
public:
    SetParentIsInvalidException() : BaseException("Attempted to set parent but the parent is invalid.") {}
};