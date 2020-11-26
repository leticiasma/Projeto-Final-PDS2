#include <iostream>
#include <exception>

#include "../../lib/exceptions/unexistent_tweet_exception.hpp"
#include "../../lib/exceptions/unexistent_entity_exception.hpp"

UnexistentTweetException::UnexistentTweetException(int id){
  this->message = "Unexistent tweet for id #"+std::to_string(id);
  this->id = id;
}
