//
// Created by Richard Marks on 2/25/23.
//


#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool NegateInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("_", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::ref()))
  {
    instruction = Instruction::NegRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long NegateInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::NegRef:
  {
    return build(
      ParseUtils::matchRef,
      program,
      inputLexemes,
      inputOffset
    );
  }
  
  default:
    return 0;
  }
}
