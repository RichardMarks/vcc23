//
// Created by Richard Marks on 2/26/23.
//

#include "Runtime.h"

#include <sstream>
#include <stdexcept>
#include <iostream>

using namespace vc23;

void Runtime::registerOperationMethod(const std::string &name, const RuntimeOperationMethod &method)
{
  if (operationMethods.count(name) == 0)
  {
    operationMethods.emplace(name, method);
  }
}

void Runtime::deregisterOperationMethod(const std::string &name)
{
  if (operationMethods.count(name))
  {
    operationMethods.erase(name);
  }
}

void Runtime::executeOperationMethod(const std::string &name, const std::vector<unsigned long> &operands)
{
  if (operationMethods.count(name))
  {
    auto method = operationMethods.at(name);
    try
    {
      method(operands);
    } catch (std::exception &ex)
    {
      std::ostringstream ss;
      ss << "Runtime Operation Method Exception in \""
         << name << "\": " << ex.what() << std::endl;
      throw std::runtime_error(ss.str());
    }
  }
}

bool Runtime::hasOperationMethod(const std::string &name)
{
  return operationMethods.count(name) > 0;
}