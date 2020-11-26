#include <iostream>
#include <exception>
#include "../../lib/exceptions/resource_not_found_exception.hpp"

ResourceNotFoundException::ResourceNotFoundException(std::string resource_name){
  this->resource_name = resource_name;
}

std::string ResourceNotFoundException::get_resource_name(){
  return this->resource_name;
}
