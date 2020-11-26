#ifndef INVALID_ACTION_EXCEPTION_H
#define INVALID_ACTION_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class InvalidActionException : public std::exception {
  public:
    InvalidActionException(std::string action, std::string reason);
    std::string what();
    std::string get_message();

  private:
    std::string action;
    std::string reason;
};

#endif
