//
// Created by Richard Marks on 2/24/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool EndInstructionSyntax::peek(std::vector<Lexeme> &input, unsigned long start)
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
    if (compare(input, start + 1, SyntaxPatterns::decLit()))
    {
// std::cout << "assign declitref" << std::endl;
      instruction = Instruction::EndWithDecLitExitCode;
      exitCode = ParseUtils::parseULong(input[start + 2].data);
      size = 2;
    } else if (compare(input, start + 1, SyntaxPatterns::hexLit()))
    {
// std::cout << "assign hexlitref" << std::endl;
      instruction = Instruction::EndWithHexLitExitCode;
      exitCode = ParseUtils::parseULong(input[start + 2].data, 16);
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

unsigned long EndInstructionSyntax::consume()
{
  return size;
}

[[nodiscard]] std::unique_ptr<InstructionNode> EndInstructionSyntax::getInstructionNode() const
{
  if (instruction == Instruction::EndWithSuccess)
  {
    std::vector<unsigned long> operands;
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
  std::vector<unsigned long> operands{exitCode};
  return std::make_unique<InstructionNode>(instruction, std::move(operands));
}