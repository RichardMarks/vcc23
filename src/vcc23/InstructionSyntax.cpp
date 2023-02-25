//
// Created by Richard Marks on 2/24/23.
//

#include "InstructionSyntax.h"

#include "ParserErrors.h"
#include "ParseUtils.h"

#include <iostream>

using namespace vcc23;

bool InstructionSyntax::compare(std::vector<Lexeme> &input, unsigned long start, const std::vector<LexemeType> &pattern)
{
  for (unsigned long i = 0; i < pattern.size(); i++)
  {
    if (start + i > input.size())
    {
      throw UnexpectedEndOfTokenStreamError(start + i, input.size());
    }
    
    auto &inputToken = input.at(start + i);
    auto &expectedType = pattern.at(i);
    
    bool typeFailed = !inputToken.isType(expectedType);
    
    std::cout << "comparing input \"" << inputToken.data << "\" " << std::hex << (unsigned) inputToken.type << std::dec
              << " and expected " << std::hex << (unsigned) expectedType << std::dec << std::endl;
    if (typeFailed)
    {
      // edge case - addressing via x,y,z
      if (i > 0 && pattern[i - 1] == LexemeType::AddressPrefix)
      {
        if (expectedType == LexemeType::DecimalNumber)
        {
          if (inputToken.isType(LexemeType::Unknown))
          {
            if (!(ParseUtils::isValidAddressToken(inputToken.data)))
            {
              std::cout << "address token edge case compare failed at " << (start + i) << std::endl;
              return false;
            } else
            {
              // std::cout << "address token edge case type comparison failure override at " << (start + i) << std::endl;
              typeFailed = false;
            }
          }
        }
      }
    }
    
    if (typeFailed)
    {
      std::cout << "compare failed at " << (start + i) << std::endl;
      return false;
    }
  }
  return true;
}