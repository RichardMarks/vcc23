//
// Created by Richard Marks on 2/24/23.
//

#include "ParseUtils.h"
#include "Lexer.h"

#include <iostream>

using namespace vcc23;


std::vector<LexemeType> DECLITREF{LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::AddressPrefix,
                                  LexemeType::DecimalNumber};
std::vector<LexemeType> DECLIT{LexemeType::DecimalPrefix, LexemeType::DecimalNumber};
std::vector<LexemeType> HEXLITREF{LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::AddressPrefix,
                                  LexemeType::DecimalNumber};
std::vector<LexemeType> HEXLIT{LexemeType::HexPrefix, LexemeType::HexNumber};
std::vector<LexemeType> REFREF{LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::AddressPrefix,
                               LexemeType::DecimalNumber};
std::vector<LexemeType> REF{LexemeType::AddressPrefix, LexemeType::DecimalNumber};


bool ParseUtils::isValidAddressToken(const std::string &token)
{
  if (token.size() == 1)
  {
    if (token[0] == 'x' || token[0] == 'y' || token[0] == 'z')
    {
      return true;
    }
  }
  
  try
  {
    int value = std::stoi(token);
    // string is a decimal number
    return true;
  }
  catch (std::invalid_argument const &)
  {
    // string is not a decimal number
    return false;
  }
  catch (std::out_of_range const &)
  {
    // string is not a decimal number
    return false;
  }
}

unsigned long ParseUtils::parseULong(const std::string &token, int base)
{
  try
  {
    unsigned long value = std::stoul(token, 0, 10);
    // string is a decimal number
    return value;
  }
  catch (std::invalid_argument const &)
  {
    // string is not a decimal number
    std::cerr << "not a decimal number? " << token << std::endl;
    return 0;
  }
  catch (std::out_of_range const &)
  {
    // string is not a decimal number
    std::cerr << "out of range? " << token << std::endl;
    return 0;
  }
}

unsigned long ParseUtils::translateAddressToken(const std::string &token)
{
  // std::cout << "translateAddressToken(\"" << token << "\")" << std::endl;
  if (token.size() == 1)
  {
    if (token[0] == 'x')
    {
      return 0;
    } else if (token[0] == 'y')
    {
      return 1;
    } else if (token[0] == 'z')
    {
      return 2;
    }
  }
  
  return parseULong(token);
}
