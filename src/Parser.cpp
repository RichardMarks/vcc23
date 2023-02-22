//
// Created by Richard Marks on 2/21/23.
//
#include "Lexer.h"
#include "Parser.h"
#include "ParserErrors.h"

#include <unordered_map>
#include <vector>
#include <string>

using namespace vcc23;

std::vector<LexemeType> DECLITREF{LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::AddressPrefix, LexemeType::DecimalNumber};
std::vector<LexemeType> DECLIT{LexemeType::DecimalPrefix, LexemeType::DecimalNumber};
std::vector<LexemeType> HEXLITREF{LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::AddressPrefix, LexemeType::DecimalNumber};
std::vector<LexemeType> HEXLIT{LexemeType::HexPrefix, LexemeType::HexNumber};
std::vector<LexemeType> REFREF{LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::AddressPrefix, LexemeType::DecimalNumber};
std::vector<LexemeType> REF{LexemeType::AddressPrefix, LexemeType::DecimalNumber};

// std::unordered_map<Instruction, std::vector<LexemeType>> SYNTAX_MAP{
//     {Instruction::Nop, {}},

//     {Instruction::AssignDecLitRef, DECLITREF},
//     {Instruction::AssignHexLitRef, HEXLITREF},
//     {Instruction::AssignRefRef, REFREF},

//     {Instruction::AddDecLitRef, DECLITREF},
//     {Instruction::AddHexLitRef, HEXLITREF},
//     {Instruction::AddRefRef, REFREF},

//     {Instruction::SubDecLitRef, DECLITREF},
//     {Instruction::SubHexLitRef, HEXLITREF},
//     {Instruction::SubRefRef, REFREF},

//     {Instruction::MulDecLitRef, DECLITREF},
//     {Instruction::MulHexLitRef, HEXLITREF},
//     {Instruction::MulRefRef, REFREF},

//     {Instruction::DivDecLitRef, DECLITREF},
//     {Instruction::DivHexLitRef, HEXLITREF},
//     {Instruction::DivRefRef, REFREF},

//     {Instruction::NegRef, REF},

//     {Instruction::XorDecLitRef, DECLITREF},
//     {Instruction::XorHexLitRef, HEXLITREF},
//     {Instruction::XorRefRef, REFREF},

//     {Instruction::AndDecLitRef, DECLITREF},
//     {Instruction::AndHexLitRef, HEXLITREF},
//     {Instruction::AndRefRef, REFREF},

//     {Instruction::OrDecLitRef, DECLITREF},
//     {Instruction::OrHexLitRef, HEXLITREF},
//     {Instruction::OrRefRef, REFREF},

//     {Instruction::LeftShiftDecLitRef, DECLITREF},
//     {Instruction::LeftShiftHexLitRef, HEXLITREF},

//     {Instruction::CmpDecLitRef, DECLITREF},
//     {Instruction::CmpHexLitRef, HEXLITREF},
//     {Instruction::CmpRefRef, REFREF},
//     {Instruction::CmpRef, REF},

//     // this might cause some problems if not careful..
//     // have to double check that the instruction is actually "|"
//     {Instruction::AbsJumpDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::AbsJumpHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::AbsJumpRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     // have to double check that the instruction is actually ">"
//     {Instruction::RelJumpForwardDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::RelJumpForwardHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::RelJumpForwardRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     // have to double check that the instruction is actually "<"
//     {Instruction::RelJumpBackDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::RelJumpBackHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::RelJumpBackRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::SelectRamDefault, {LexemeType::RAMSelect}},
//     {Instruction::SelectRamDecLit, {LexemeType::RAMSelect, LexemeType::IndexPrefix, LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::IndexSuffix}},
//     {Instruction::SelectRamHexLit, {LexemeType::RAMSelect, LexemeType::IndexPrefix, LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::IndexSuffix}},

//     {Instruction::SelectRomDefault, {LexemeType::ROMSelect}},
//     {Instruction::SelectRomDecLit, {LexemeType::ROMSelect, LexemeType::IndexPrefix, LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::IndexSuffix}},
//     {Instruction::SelectRomHexLit, {LexemeType::ROMSelect, LexemeType::IndexPrefix, LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::IndexSuffix}},

//     {Instruction::ReadInputDeviceRef, {LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::ReadInputDeviceDecLitRef, {LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},
//     {Instruction::ReadInputDeviceHexLitRef, {LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::ReadInputDeviceRefRef, {LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::WriteOutputDeviceDecLit, {LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},
//     {Instruction::WriteOutputDeviceHexLit, {LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},

//     {Instruction::WriteOutputDeviceRef, {LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},

//     {Instruction::PrintDecLitRef, DECLITREF},
//     {Instruction::PrintHexLitRef, HEXLITREF},
//     {Instruction::PrintRefRef, REFREF},

//     {Instruction::EndWithDecLitExitCode, DECLIT},
//     {Instruction::EndWithHexLitExitCode, HEXLIT},

//     {Instruction::EndWithSuccess, {}}
//     //
// };

// std::unordered_map<char, Instruction> INSTRUCTION_MAP{
//     {'~', Instruction::Nop},
//     //
// };

// all instruction matchers

bool isValidAddressToken(const std::string &token)
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

unsigned long parseULong(const std::string &token, int base = 10)
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

unsigned long translateAddressToken(const std::string &token)
{
  // std::cout << "translateAddressToken(\"" << token << "\")" << std::endl;
  if (token.size() == 1)
  {
    if (token[0] == 'x')
    {
      return 0;
    }
    else if (token[0] == 'y')
    {
      return 1;
    }
    else if (token[0] == 'z')
    {
      return 2;
    }
  }
  
  return parseULong(token);
}

class InstructionSyntax
{
protected:
  Instruction instruction{Instruction::Unknown};
  
  static bool compare(std::vector<Lexeme> &input, unsigned long start, std::vector<LexemeType> &pattern)
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
      
      std::cout << "comparing input \"" << inputToken.data << "\" " << std::hex << (unsigned)inputToken.type << std::dec << " and expected " << std::hex << (unsigned)expectedType << std::dec << std::endl;
      if (typeFailed)
      {
        // edge case - addressing via x,y,z
        if (i > 0 && pattern[i - 1] == LexemeType::AddressPrefix)
        {
          if (expectedType == LexemeType::DecimalNumber)
          {
            if (inputToken.isType(LexemeType::Unknown))
            {
              if (!(isValidAddressToken(inputToken.data)))
              {
                std::cout << "address token edge case compare failed at " << (start + i) << std::endl;
                return false;
              }
              else
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

public:
  virtual bool
  peek(std::vector<Lexeme> &input, unsigned long start) = 0;
  virtual unsigned long consume() = 0;
  [[nodiscard]] virtual std::unique_ptr<InstructionNode> getInstructionNode() const = 0;
};

class AssignmentInstructionSyntax : public InstructionSyntax
{
private:
  unsigned long address{};
  unsigned long sourceAddress{};
  unsigned long literal{};
  unsigned long size{};

public:
  bool peek(std::vector<Lexeme> &input, unsigned long start) override
  {
    address = 0;
    sourceAddress = 0;
    literal = 0;
    size = 0;
    instruction = Instruction::Unknown;
    
    if (!(input[start].isType(LexemeType::Instruction) && input[start].data == "."))
    {
      std::cout << "not assignment" << std::endl;
      return false;
    }
    
    if (compare(input, start + 1, DECLITREF))
    {
      // std::cout << "assign declitref" << std::endl;
      instruction = Instruction::AssignDecLitRef;
      literal = parseULong(input[start + 2].data);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    else if (compare(input, start + 1, HEXLITREF))
    {
      // std::cout << "assign hexlitref" << std::endl;
      instruction = Instruction::AssignHexLitRef;
      literal = parseULong(input[start + 2].data, 16);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    else if (compare(input, start + 1, REFREF))
    {
      // std::cout << "assign refref" << std::endl;
      instruction = Instruction::AssignRefRef;
      sourceAddress = translateAddressToken(input[start + 2].data);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    
    if (instruction == Instruction::Unknown)
    {
      return false;
    }
    
    return true;
  }
  
  unsigned long consume() override
  {
    return size;
  }
  
  [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override
  {
    if (instruction == Instruction::AssignRefRef)
    {
      std::vector<unsigned long> operands{
        sourceAddress, address};
      return std::make_unique<InstructionNode>(instruction, std::move(operands));
    }
    std::vector<unsigned long> operands{
      literal, address};
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
};

//

class ReadInputDeviceInstructionSyntax : public InstructionSyntax
{
private:
  unsigned long address{};
  unsigned long sourceAddress{};
  unsigned long literal{};
  unsigned long device{};
  unsigned long size{};

public:
  bool peek(std::vector<Lexeme> &input, unsigned long start) override
  {
    address = 0;
    sourceAddress = 0;
    literal = 0;
    device = 0;
    size = 0;
    instruction = Instruction::Unknown;
    
    if (!(input[start].isType(LexemeType::Instruction) && input[start].data == "r"))
    {
      std::cout << "not read input device" << std::endl;
      return false;
    }
    
    // ReadInputDeviceDecLitRef
    std::vector<LexemeType> p1{
      LexemeType::DecimalPrefix,
      LexemeType::DecimalNumber,
      LexemeType::DevicePrefix,
      LexemeType::DecimalNumber,
      LexemeType::DeviceSuffix,
      LexemeType::AddressPrefix,
      LexemeType::DecimalNumber};
    
    // ReadInputDeviceHexLitRef
    std::vector<LexemeType> p2{
      LexemeType::HexPrefix,
      LexemeType::HexNumber,
      LexemeType::DevicePrefix,
      LexemeType::DecimalNumber,
      LexemeType::DeviceSuffix,
      LexemeType::AddressPrefix,
      LexemeType::DecimalNumber};
    
    // ReadInputDeviceRefRef
    std::vector<LexemeType> p3{
      LexemeType::AddressPrefix,
      LexemeType::DecimalNumber,
      LexemeType::DevicePrefix,
      LexemeType::DecimalNumber,
      LexemeType::DeviceSuffix,
      LexemeType::AddressPrefix,
      LexemeType::DecimalNumber};
    
    if (compare(input, start + 1, p1))
    {
      // std::cout << "assign declitref" << std::endl;
      instruction = Instruction::ReadInputDeviceDecLitRef;
      
      literal = parseULong(input[start + 2].data);
      device = parseULong(input[start + 4].data);
      address = translateAddressToken(input[start + 7].data);
      size = 7;
    }
    else if (compare(input, start + 1, p2))
    {
      // std::cout << "assign hexlitref" << std::endl;
      instruction = Instruction::ReadInputDeviceHexLitRef;
      literal = parseULong(input[start + 2].data, 16);
      device = parseULong(input[start + 4].data);
      address = translateAddressToken(input[start + 7].data);
      size = 7;
    }
    else if (compare(input, start + 1, p3))
    {
      // std::cout << "assign refref" << std::endl;
      instruction = Instruction::ReadInputDeviceRefRef;
      sourceAddress = translateAddressToken(input[start + 2].data);
      device = parseULong(input[start + 4].data);
      address = translateAddressToken(input[start + 7].data);
      size = 7;
    }
    
    if (instruction == Instruction::Unknown)
    {
      return false;
    }
    
    return true;
  }
  
  unsigned long consume() override
  {
    return size;
  }
  
  [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override
  {
    if (instruction == Instruction::ReadInputDeviceRefRef)
    {
      std::vector<unsigned long> operands{
        sourceAddress, device, address};
      return std::make_unique<InstructionNode>(instruction, std::move(operands));
    }
    std::vector<unsigned long> operands{
      literal, device, address};
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
};

//

class PrintInstructionSyntax : public InstructionSyntax
{
private:
  unsigned long address{};
  unsigned long sourceAddress{};
  unsigned long literal{};
  unsigned long size{};

public:
  bool peek(std::vector<Lexeme> &input, unsigned long start) override
  {
    address = 0;
    sourceAddress = 0;
    literal = 0;
    size = 0;
    instruction = Instruction::Unknown;
    
    if (!(input[start].isType(LexemeType::Instruction) && input[start].data == "p"))
    {
      std::cout << "not print string" << std::endl;
      return false;
    }
    
    if (compare(input, start + 1, DECLITREF))
    {
      // std::cout << "assign declitref" << std::endl;
      instruction = Instruction::PrintDecLitRef;
      literal = parseULong(input[start + 2].data);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    else if (compare(input, start + 1, HEXLITREF))
    {
      // std::cout << "assign hexlitref" << std::endl;
      instruction = Instruction::PrintHexLitRef;
      literal = parseULong(input[start + 2].data, 16);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    else if (compare(input, start + 1, REFREF))
    {
      // std::cout << "assign refref" << std::endl;
      instruction = Instruction::PrintRefRef;
      sourceAddress = translateAddressToken(input[start + 2].data);
      address = translateAddressToken(input[start + 4].data);
      size = 4;
    }
    
    if (instruction == Instruction::Unknown)
    {
      return false;
    }
    
    return true;
  }
  
  unsigned long consume() override
  {
    return size;
  }
  
  [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override
  {
    if (instruction == Instruction::PrintRefRef)
    {
      std::vector<unsigned long> operands{
        sourceAddress, address};
      return std::make_unique<InstructionNode>(instruction, std::move(operands));
    }
    std::vector<unsigned long> operands{
      literal, address};
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
};

//

class EndInstructionSyntax : public InstructionSyntax
{
private:
  unsigned long exitCode{};
  unsigned long size{};

public:
  bool peek(std::vector<Lexeme> &input, unsigned long start) override
  {
    exitCode = 0;
    size = 0;
    instruction = Instruction::Unknown;
    
    if (!(input[start].isType(LexemeType::Instruction) && input[start].data == "q"))
    {
      std::cout << "not end program" << std::endl;
      return false;
    }
    
    instruction = Instruction::EndWithSuccess;
    
    try
    {
      if (compare(input, start + 1, DECLIT))
      {
        // std::cout << "assign declitref" << std::endl;
        instruction = Instruction::EndWithDecLitExitCode;
        exitCode = parseULong(input[start + 2].data);
        size = 2;
      }
      else if (compare(input, start + 1, HEXLIT))
      {
        // std::cout << "assign hexlitref" << std::endl;
        instruction = Instruction::EndWithHexLitExitCode;
        exitCode = parseULong(input[start + 2].data, 16);
        size = 2;
      }
    }
    catch (const std::exception &ex)
    {
      std::cerr << ex.what() << std::endl;
      // we accept that no tokens follow the end token
    }
    
    if (instruction == Instruction::Unknown)
    {
      return false;
    }
    
    return true;
  }
  
  unsigned long consume() override
  {
    return size;
  }
  
  [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override
  {
    if (instruction == Instruction::EndWithSuccess)
    {
      std::vector<unsigned long> operands;
      return std::make_unique<InstructionNode>(instruction, std::move(operands));
    }
    std::vector<unsigned long> operands{exitCode};
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
};

//

Parser::Parser(Lexer *lexer) : lexerPtr(lexer)
{
}

void Parser::buildSyntaxTree()
{
  
  AssignmentInstructionSyntax assignSyntax;
  ReadInputDeviceInstructionSyntax readInputDeviceSyntax;
  PrintInstructionSyntax printSyntax;
  EndInstructionSyntax endSyntax;
  
  std::unordered_map<char, InstructionSyntax *> syntaxTable{
    {'.', &assignSyntax},
    {'r', &readInputDeviceSyntax},
    {'p', &printSyntax},
    {'q', &endSyntax}};
  
  std::vector<Lexeme> lexemes = lexerPtr->getLexemeStream();
  
  currentIndex = 0;
  
  syntaxTree = std::make_unique<ProgramNode>();
  auto *program = reinterpret_cast<ProgramNode *>(syntaxTree.get());
  
  while (currentIndex < lexemes.size())
  {
    auto token = lexemes.at(currentIndex);
    std::cout << "[" << std::setw(4) << currentIndex << "] " << token << std::endl;
    if (token.isType(LexemeType::Instruction))
    {
      if (token.data.size() == 1 && syntaxTable.count(token.data[0]))
      {
        auto syntax = syntaxTable.at(token.data[0]);
        if (syntax->peek(lexemes, currentIndex))
        {
          auto advanced = syntax->consume();
          // std::cout << "advancing " << advanced << std::endl;
          currentIndex += advanced;
          program->addInstructionNode(std::move(syntax->getInstructionNode()));
        }
      }
      else
      {
        std::cerr << "unknown token: " << token.data << std::endl;
      }
    }
    currentIndex++;
  }
}
