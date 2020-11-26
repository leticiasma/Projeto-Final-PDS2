#ifndef RESOURCE_NOT_FOUND_EXCEPTION_H
#define RESOURCE_NOT_FOUND_EXCEPTION_H

#include <iostream>
#include <exception>
#include <string>

class ResourceNotFoundException : public std::exception {
  public:
    ResourceNotFoundException(std::string resource_name);
    std::string get_resource_name();

  private:
    std::string resource_name;
};

#endif
