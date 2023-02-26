//
// Created by Richard Marks on 2/24/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool ReadInputDeviceImmediateInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("i", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::devRef()))
  {
    instruction = Instruction::ReadInputDeviceRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long ReadInputDeviceImmediateInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::ReadInputDeviceRef:
  {
    return build(
      ParseUtils::matchDevRef,
      program,
      inputLexemes,
      inputOffset
    );
  }
  default:
    return 0;
  }
}

bool ReadInputDeviceInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("r", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::decLitDevRef()))
  {
    instruction = Instruction::ReadInputDeviceDecLitRef;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::hexLitDevRef()))
  {
    instruction = Instruction::ReadInputDeviceHexLitRef;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::refDevRef()))
  {
    instruction = Instruction::ReadInputDeviceRefRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long ReadInputDeviceInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::ReadInputDeviceDecLitRef:
  {
    return build(
      ParseUtils::matchDecLitDevRef,
      program,
      inputLexemes,
      inputOffset
    );
  }
  case Instruction::ReadInputDeviceHexLitRef:
  {
    return build(
      ParseUtils::matchHexLitDevRef,
      program,
      inputLexemes,
      inputOffset
    );
  }
  case Instruction::ReadInputDeviceRefRef:
  {
    return build(
      ParseUtils::matchRefDevRef,
      program,
      inputLexemes,
      inputOffset
    );
  }
  default:
    return 0;
  }
}
