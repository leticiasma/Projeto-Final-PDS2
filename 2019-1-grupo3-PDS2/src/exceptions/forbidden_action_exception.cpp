#include <iostream>
#include <exception>
#include "../../lib/exceptions/forbidden_action_exception.hpp"

ForbiddenActionException::ForbiddenActionException(std::string action, std::string reason){
  this->action = action;
  this->reason = reason;
}

std::string ForbiddenActionException::what(){
  return this->reason;
}

std::string ForbiddenActionException::get_message(){
  return "Forbidden action '"+this->action+"' reason: "+this->reason;
}
