//
// Created by Richard Marks on 2/24/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool PrintInstructionSyntax::peek(std::vector<Lexeme> &input, unsigned long start)
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
  
  if (compare(input, start + 1, SyntaxPatterns::decLitRef()))
  {
// std::cout << "assign declitref" << std::endl;
    instruction = Instruction::PrintDecLitRef;
    literal = ParseUtils::parseULong(input[start + 2].data);
    address = ParseUtils::translateAddressToken(input[start + 4].data);
    size = 4;
  } else if (compare(input, start + 1, SyntaxPatterns::hexLitRef()))
  {
// std::cout << "assign hexlitref" << std::endl;
    instruction = Instruction::PrintHexLitRef;
    literal = ParseUtils::parseULong(input[start + 2].data, 16);
    address = ParseUtils::translateAddressToken(input[start + 4].data);
    size = 4;
  } else if (compare(input, start + 1, SyntaxPatterns::refRef()))
  {
// std::cout << "assign refref" << std::endl;
    instruction = Instruction::PrintRefRef;
    sourceAddress = ParseUtils::translateAddressToken(input[start + 2].data);
    address = ParseUtils::translateAddressToken(input[start + 4].data);
    size = 4;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long PrintInstructionSyntax::consume()
{
  return size;
}

[[nodiscard]] std::unique_ptr<InstructionNode> PrintInstructionSyntax::getInstructionNode() const
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