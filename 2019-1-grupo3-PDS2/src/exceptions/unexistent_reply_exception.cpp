#include <iostream>
#include <exception>

#include "../../lib/exceptions/unexistent_reply_exception.hpp"
#include "../../lib/exceptions/unexistent_entity_exception.hpp"

UnexistentReplyException::UnexistentReplyException(int id){
  this->message = "Unexistent reply for id #"+std::to_string(id);
  this->id = id;
}
