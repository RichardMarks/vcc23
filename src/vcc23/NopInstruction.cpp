//
// Created by Richard Marks on 2/25/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool NopInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  if (ParseUtils::matchInstructionToken("~", input, start))
  {
    instruction = Instruction::Nop;
    return true;
  }
  
  instruction = Instruction::Unknown;
  return false;
}

unsigned long NopInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::Nop:
  {
    return build(
      ParseUtils::matchNONE,
      program,
      inputLexemes,
      inputOffset);
  }
  
  default:
    return 0;
  }
}
