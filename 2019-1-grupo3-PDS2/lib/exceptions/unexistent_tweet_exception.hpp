#ifndef UNEXISTENT_TWEET_EXCEPTION_H
#define UNEXISTENT_TWEET_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

#include "unexistent_entity_exception.hpp"

class UnexistentTweetException : public UnexistentEntityException {
  public:
    UnexistentTweetException(int id);
};

#endif
