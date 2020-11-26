#ifndef UNEXISTENT_REPLY_EXCEPTION_H
#define UNEXISTENT_REPLY_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

#include "unexistent_entity_exception.hpp"

class UnexistentReplyException : public UnexistentEntityException {
  public:
    UnexistentReplyException(int id);
};

#endif
