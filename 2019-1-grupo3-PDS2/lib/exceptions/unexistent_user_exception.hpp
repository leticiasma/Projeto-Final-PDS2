#ifndef UNEXISTENT_USER_EXCEPTION_H
#define UNEXISTENT_USER_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

#include "unexistent_entity_exception.hpp"

class UnexistentUserException : public UnexistentEntityException {
  public:
    UnexistentUserException(int id);
};

#endif
