//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_PARSEUTILS_H
#define VC23_PARSEUTILS_H

#include <string>

namespace vcc23
{
  class ParseUtils
  {
  public:
    static bool isValidAddressToken(const std::string &token);
    
    static unsigned long parseULong(const std::string &token, int base = 10);
    
    static unsigned long translateAddressToken(const std::string &token);
    
  };
  
}

#endif //VC23_PARSEUTILS_H
