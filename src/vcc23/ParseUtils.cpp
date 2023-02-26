//
// Created by Richard Marks on 2/24/23.
//

#include "ParserErrors.h"
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


bool ParseUtils::compare(
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset,
  const std::vector<LexemeType> &pattern)
{
  for (unsigned long i = 0; i < pattern.size(); i++)
  {
    if (inputOffset + i > inputLexemes.size())
    {
      throw UnexpectedEndOfTokenStreamError(inputOffset + i, inputLexemes.size());
    }
    
    auto &inputToken = inputLexemes.at(inputOffset + i);
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
              std::cout << "address token edge case compare failed at " << (inputOffset + i) << std::endl;
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
      std::cout << "compare failed at " << (inputOffset + i) << std::endl;
      return false;
    }
  }
  return true;
}

bool ParseUtils::matchInstructionToken(
  const std::string &expected,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset)
{
  auto &lexeme = inputLexemes[inputOffset];
  return lexeme.isType(LexemeType::Instruction) && lexeme.data == expected;
}


MatchResult ParseUtils::matchDecLitRef(
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  auto &refLexeme = inputLexemes[inputOffset + 4];
  MatchResult result{
    4, {
      ParseUtils::parseULong(litLexeme.data),
      ParseUtils::translateAddressToken(refLexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchHexLitRef(const std::vector<Lexeme> &inputLexemes,
                                       unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  auto &refLexeme = inputLexemes[inputOffset + 4];
  MatchResult result{
    4, {
      ParseUtils::parseULong(litLexeme.data, 16),
      ParseUtils::translateAddressToken(refLexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchRefRef(const std::vector<Lexeme> &inputLexemes,
                                    unsigned long inputOffset)
{
  auto &ref1Lexeme = inputLexemes[inputOffset + 2];
  auto &ref2Lexeme = inputLexemes[inputOffset + 4];
  MatchResult result{
    4, {
      ParseUtils::translateAddressToken(ref1Lexeme.data),
      ParseUtils::translateAddressToken(ref2Lexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchDecLitDevRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  auto &devLexeme = inputLexemes[inputOffset + 4];
  auto &refLexeme = inputLexemes[inputOffset + 7];
  MatchResult result{
    7, {
      ParseUtils::parseULong(litLexeme.data),
      ParseUtils::parseULong(devLexeme.data),
      ParseUtils::translateAddressToken(refLexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchHexLitDevRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  auto &devLexeme = inputLexemes[inputOffset + 4];
  auto &refLexeme = inputLexemes[inputOffset + 7];
  MatchResult result{
    7, {
      ParseUtils::parseULong(litLexeme.data, 16),
      ParseUtils::parseULong(devLexeme.data),
      ParseUtils::translateAddressToken(refLexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchRefDevRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &ref1Lexeme = inputLexemes[inputOffset + 2];
  auto &devLexeme = inputLexemes[inputOffset + 4];
  auto &ref2Lexeme = inputLexemes[inputOffset + 7];
  MatchResult result{
    7, {
      ParseUtils::translateAddressToken(ref1Lexeme.data),
      ParseUtils::parseULong(devLexeme.data),
      ParseUtils::translateAddressToken(ref2Lexeme.data)
    }
  };
  return result;
}

MatchResult ParseUtils::matchRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &refLexeme = inputLexemes[inputOffset + 2];
  MatchResult result{
    2, {
      ParseUtils::translateAddressToken(refLexeme.data),
    }
  };
  return result;
}

MatchResult ParseUtils::matchDecLit(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  MatchResult result{
    2, {
      ParseUtils::parseULong(litLexeme.data),
    }
  };
  return result;
}

MatchResult ParseUtils::matchHexLit(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 2];
  MatchResult result{
    2, {
      ParseUtils::parseULong(litLexeme.data, 16),
    }
  };
  return result;
}

MatchResult ParseUtils::matchNONE(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  MatchResult result{
    0, {}
  };
  return result;
}

MatchResult ParseUtils::matchInsDecLit(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  // 0  1  2   3  4
  // .  d  13  @  x
  // g  |  d   13
  auto &litLexeme = inputLexemes[inputOffset + 3];
  MatchResult result{
    3, {
      ParseUtils::parseULong(litLexeme.data),
    }
  };
  return result;
}

MatchResult ParseUtils::matchInsHexLit(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &litLexeme = inputLexemes[inputOffset + 3];
  MatchResult result{
    3, {
      ParseUtils::parseULong(litLexeme.data, 16),
    }
  };
  return result;
}

MatchResult ParseUtils::matchInsRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &refLexeme = inputLexemes[inputOffset + 3];
  MatchResult result{
    3, {
      ParseUtils::translateAddressToken(refLexeme.data),
    }
  };
  return result;
}

MatchResult ParseUtils::matchDevRef(const std::vector<Lexeme> &inputLexemes, unsigned long inputOffset)
{
  auto &devLexeme = inputLexemes[inputOffset + 2];
  auto &refLexeme = inputLexemes[inputOffset + 5];
  MatchResult result{
    5, {
      ParseUtils::parseULong(devLexeme.data),
      ParseUtils::translateAddressToken(refLexeme.data)
    }
  };
  return result;
}
