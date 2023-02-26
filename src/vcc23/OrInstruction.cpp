//
// Created by Richard Marks on 2/25/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool OrInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("|", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::decLitRef()))
  {
    instruction = Instruction::OrDecLitRef;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::hexLitRef()))
  {
    instruction = Instruction::OrHexLitRef;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::refRef()))
  {
    instruction = Instruction::OrRefRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long OrInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::OrDecLitRef:
  {
    return build(
      ParseUtils::matchDecLitRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::OrHexLitRef:
  {
    return build(
      ParseUtils::matchHexLitRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::OrRefRef:
  {
    return build(
      ParseUtils::matchRefRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  default:
    return 0;
  }
}

