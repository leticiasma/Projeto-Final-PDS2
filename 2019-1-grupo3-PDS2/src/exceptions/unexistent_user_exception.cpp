#include <iostream>
#include <exception>
#include "../../lib/exceptions/unexistent_user_exception.hpp"

UnexistentUserException::UnexistentUserException(int id){
  this->message = "Unexistent user for id #"+std::to_string(id);
  this->id = id;
}
