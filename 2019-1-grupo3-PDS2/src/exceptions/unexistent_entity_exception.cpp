#include <iostream>
#include <exception>

#include "../../lib/exceptions/unexistent_entity_exception.hpp"

UnexistentEntityException::UnexistentEntityException(){

}

std::string UnexistentEntityException::what(){
  return this->message;
}

std::string UnexistentEntityException::get_message(){
  return this->message;
}

int UnexistentEntityException::get_id(){
  return this->id;
}

UnexistentEntityException::~UnexistentEntityException(){

}
