//
// Created by Richard Marks on 2/26/23.
//

#ifndef VC23_RUNTIME_H
#define VC23_RUNTIME_H

#include <iostream>
#include <stdexcept>

#include <functional>
#include <unordered_map>
#include <string>

#define VC23_SAFE_BLOCK_START \
  try                         \
  {
#define VC23_SAFE_BLOCK_END                                           \
  }                                                                   \
  catch (std::exception & ex)                                         \
  {                                                                   \
    std::cerr << "VC23 SAFE BLOCK ERROR: " << ex.what() << std::endl; \
  }

namespace vc23
{
  // operation methods are passed the operands for the operation
  typedef std::function<void(const std::vector<unsigned long> &)> RuntimeOperationMethod;
  
  class Runtime
  {
  protected:
    std::unordered_map<std::string, RuntimeOperationMethod> operationMethods;
  
  public:
    Runtime() = default;
    
    virtual ~Runtime() = default;
    
    virtual void registerOperationMethod(const std::string &name, const RuntimeOperationMethod &method);
    
    virtual void deregisterOperationMethod(const std::string &name);
    
    virtual void executeOperationMethod(const std::string &name, const std::vector<unsigned long> &);
    
    virtual bool hasOperationMethod(const std::string &name);
    
    virtual void startup() = 0;
    
    virtual void step() = 0;
    
    virtual void shutdown() = 0;
    
    virtual unsigned char readU8FromDataBlock(unsigned long offset) = 0;
    
    virtual unsigned char readU8FromRAM(unsigned long offset) = 0;
    
    virtual void writeU8ToRAM(unsigned char value, unsigned long offset) = 0;
  };
}

#endif //VC23_RUNTIME_H
