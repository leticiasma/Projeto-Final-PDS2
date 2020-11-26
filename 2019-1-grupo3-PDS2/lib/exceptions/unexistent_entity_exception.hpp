#ifndef UNEXISTENT_ENTITY_EXCEPTION_H
#define UNEXISTENT_ENTITY_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class UnexistentEntityException : public std::exception {
  public:
    UnexistentEntityException();
    ~UnexistentEntityException();
    std::string what();
    std::string get_message();
    int get_id();

  protected:
    std::string message;
    int id;
};

#endif
