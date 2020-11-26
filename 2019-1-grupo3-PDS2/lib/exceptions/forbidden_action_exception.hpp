#ifndef FORBIDDEN_ACTION_EXCEPTION_H
#define FORBIDDEN_ACTION_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class ForbiddenActionException : public std::exception {
  public:
    ForbiddenActionException(std::string action, std::string reason);
    std::string what();
    std::string get_message();

  private:
    std::string action;
    std::string reason;
};

#endif
