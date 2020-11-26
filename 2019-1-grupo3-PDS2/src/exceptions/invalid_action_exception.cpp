#include <iostream>
#include <exception>
#include "../../lib/exceptions/invalid_action_exception.hpp"

InvalidActionException::InvalidActionException(std::string action, std::string reason){
  this->action = action;
  this->reason = reason;
}

std::string InvalidActionException::what(){
  return this->reason;
}

std::string InvalidActionException::get_message(){
  return "Invalid action '"+this->action+"' reason: "+this->reason;
}
