//
// Created by Richard Marks on 2/21/23.
//

#include "VCCReader.h"
#include "Lexer.h"

#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <stdexcept>
#include <sstream>
#include <cctype>

using namespace vcc23;

bool isDecimalNumber(const std::string &str)
{
  try
  {
    int value = std::stoi(str);
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

bool isHexNumber(const std::string &str)
{
  try
  {
    long value = std::stol(str, nullptr, 16);
    // string is a hex number
    return true;
  }
  catch (std::invalid_argument const &)
  {
    // string is not a hex number
    return false;
  }
  catch (std::out_of_range const &)
  {
    // string is not a hex number
    return false;
  }
}

bool isLabel(const std::string &str)
{
  if (str.empty() || !std::isalpha(str[0]) && str[0] != '_')
  {
    return false;
  }
  
  for (std::size_t i = 1; i < str.size() - 1; ++i)
  {
    if (!std::isalnum(str[i]) && str[i] != '_')
    {
      return false;
    }
  }
  
  return str.back() == ':';
}

bool isIdentifier(const std::string &str)
{
  if (str.empty() || !std::isalpha(str[0]) && str[0] != '_')
  {
    return false;
  }
  
  for (std::size_t i = 1; i < str.size(); ++i)
  {
    if (!std::isalnum(str[i]) && str[i] != '_')
    {
      return false;
    }
  }
  
  return true;
}

std::unordered_set<char> INSTRUCTION_SEPARATORS = {
  '@',
  'd',
  '&',
  '>',
  '<',
  '?',
  '#',
  '/',
  '`',
  '^',
  '|',
  '$',
  '[',
  ']',
  '{',
  '}'};

std::unordered_map<char, LexemeType> CHAR_MAP{
  // instructions
  {'~', LexemeType::Instruction},
  {'.', LexemeType::Instruction},
  {'+', LexemeType::Instruction},
  {'-', LexemeType::Instruction},
  {'*', LexemeType::Instruction},
  {'/', LexemeType::Instruction},
  {'_', LexemeType::Instruction},
  {'^', LexemeType::Instruction},
  {'&', LexemeType::Instruction},
  {'|', LexemeType::Instruction},
  {'<', LexemeType::Instruction},
  {'>', LexemeType::Instruction},
  {'=', LexemeType::Instruction},
  {'z', LexemeType::Instruction},
  {'g', LexemeType::Instruction},
  {'`', LexemeType::Instruction},
  {'i', LexemeType::Instruction},
  {'r', LexemeType::Instruction},
  {'o', LexemeType::Instruction},
  {'p', LexemeType::Instruction},
  {'q', LexemeType::Instruction},
  {'?', LexemeType::Instruction},
  // prefixes
  {'d', LexemeType::DecimalPrefix},
  {'&', LexemeType::HexPrefix},
  {'@', LexemeType::AddressPrefix},
  {'{', LexemeType::DevicePrefix},
  {'[', LexemeType::IndexPrefix},
  {'#', LexemeType::RegisterPrefix},
  {'$', LexemeType::VariablePrefix},
  // suffixes
  {'}', LexemeType::DeviceSuffix},
  {']', LexemeType::IndexSuffix},
  // selectors
  {'R', LexemeType::RAMSelect},
  {'D', LexemeType::ROMSelect}};

std::unordered_map<char, std::string> LEXEME_NAMES{
  // instructions
  {'~', "NOP"},
  {'.', "ASSIGN"},
  {'+', "ADD"},
  {'-', "SUBTRACT"},
  {'*', "MULTIPLY"},
  {'/', "DIVIDE"},
  {'_', "NEGATE"},
  {'^', "XOR"},
  {'&', "AND"},
  {'|', "OR|ABS"},
  {'<', "SHIFTLEFT"},
  {'>', "SHIFTRIGHT"},
  {'=', "COMPARE"},
  {'?', "COMPARE"},
  {'x', "RAM0"},
  {'y', "RAM1"},
  {'z', "COMPAREZERO|RAM2"},
  {'g', "JUMP"},
  {'`', "SELECT"},
  {'i', "INPUT"},
  {'r', "READ"},
  {'o', "OUTPUT"},
  {'p', "PRINT"},
  {'q', "END"},
  // prefixes
  {'d', "DECIMAL_PREFIX"},
  {'&', "HEX_PREFIX"},
  {'@', "ADDRESS_PREFIX"},
  {'{', "DEVICE_PREFIX"},
  {'[', "INDEX_PREFIX"},
  {'#', "REGISTER_PREFIX"},
  {'$', "VARIABLE_PREFIX"},
  // suffixes
  {'}', "DEVICE_SUFFIX"},
  {']', "INDEX_SUFFIX"},
  // selectors
  {'R', "RAM"},
  {'D', "ROM"}};

std::string Lexeme::name() const
{
  if (data.size() == 1)
  {
    char character = data[0];
    if (LEXEME_NAMES.count(character))
    {
      return LEXEME_NAMES.at(character);
    }
  }
  if (isType(LexemeType::Label))
  {
    return "LABEL: " + data;
  }
  if (isType(LexemeType::Identifier))
  {
    return "IDENTIFIER: " + data;
  }
  if (isType(LexemeType::DecimalNumber))
  {
    return data;
  }
  if (isType(LexemeType::HexNumber))
  {
    return data;
  }
  
  return "Unknown";
}

Lexer::Lexer(VCCReader *reader) : readerPtr(reader)
{
}

void Lexer::tokenizeAndReduceToLexemeStream()
{
  lexemes.clear();
  
  auto lineCount = readerPtr->getCodeLineCount();
  for (size_t i = 0; i < lineCount; i++)
  {
    auto line = readerPtr->getCodeLine(i);
    auto rawTokens = Lexer::tokenize(line);
    auto tokens = Lexer::transform(rawTokens);
    for (auto &token: tokens)
    {
      lexemes.push_back(token);
    }
  }
}

std::vector<std::string> Lexer::tokenize(const std::string &input)
{
  std::vector<std::string> tokens;
  std::string token;
  
  std::cout << "tokenize(\"" << input << "\")" << std::endl;
  
  if (isLabel(input))
  {
    tokens.push_back(input);
  } else
  {

//    token += input[0];
//    tokens.push_back(token);
//    token.clear();
    for (size_t i = 0; i < input.size(); ++i)
    {
      if (input[i] == ' ' || input[i] == '\t' || input[i] == '\n')
      {
        if (token.size() > 1)
        {
          tokens.push_back(token);
          token.clear();
        }
        continue;
      }
      
      if (INSTRUCTION_SEPARATORS.count(input[i]))
      {
        if (!token.empty())
        {
          tokens.push_back(token);
        }
        token.clear();
        token += input[i];
        tokens.push_back(token);
        token.clear();
      } else
      {
        token += input[i];
      }
    }
  }
  if (!token.empty())
  {
    tokens.push_back(token);
  }
  
  // std::cout << "\"" << input << "\" to { ";
  // for (auto &tok : tokens)
  // {
  //   std::cout << "\"" << tok << "\", ";
  // }
  // std::cout << "}" << std::endl;
  
  return tokens;
}

std::vector<Lexeme> Lexer::transform(const std::vector<std::string> &rawTokens)
{
  std::vector<Lexeme> tokens;
  
  for (auto &rawToken: rawTokens)
  {
    auto type = Lexer::identify(rawToken);
    auto token = Lexeme(type, rawToken);
    tokens.push_back(token);
  }
  
  return tokens;
}

LexemeType Lexer::identify(const std::string &token)
{
  if (token.size() == 1)
  {
    char character = token[0];
    if (CHAR_MAP.count(character))
    {
      return CHAR_MAP.at(character);
    }
  }
  
  if (isLabel(token))
  {
    return LexemeType::Label;
  }
  
  if (isIdentifier(token))
  {
    return LexemeType::Identifier;
  }
  
  if (isDecimalNumber(token))
  {
    return LexemeType::DecimalNumber;
  }
  
  if (isHexNumber(token))
  {
    return LexemeType::HexNumber;
  }
  
  return LexemeType::Unknown;
}

std::string Lexer::info()
{
  std::ostringstream os;
  
  os << "VCC LEXER INFORMATION:" << std::endl;
  
  std::vector<Lexeme> allTokens;
  
  auto lineCount = readerPtr->getCodeLineCount();
  for (size_t i = 0; i < lineCount; i++)
  {
    auto line = readerPtr->getCodeLine(i);
    os << "INPUT LINE: " << line << std::endl;
    auto rawTokens = Lexer::tokenize(line);
    os << "RAW TOKENS: ";
    for (auto &t: rawTokens)
    {
      os << t << " ";
    }
    os << std::endl;
    os << "LEXEMES:" << std::endl;
    auto tokens = Lexer::transform(rawTokens);
    for (auto &token: tokens)
    {
      os << token << std::endl;
    }
    os << std::endl;
  }
  
  return os.str();
}
