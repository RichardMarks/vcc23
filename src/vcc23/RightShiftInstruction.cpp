//
// Created by Richard Marks on 2/25/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool RightShiftInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken(">", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::decLitRef()))
  {
    instruction = Instruction::RightShiftDecLitRef;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::hexLitRef()))
  {
    instruction = Instruction::RightShiftHexLitRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long RightShiftInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::RightShiftDecLitRef:
  {
    return build(
      ParseUtils::matchDecLitRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RightShiftHexLitRef:
  {
    return build(
      ParseUtils::matchHexLitRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  default:
    return 0;
  }
}
