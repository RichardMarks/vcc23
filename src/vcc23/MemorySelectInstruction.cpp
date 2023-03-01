//
// Created by Richard Marks on 2/28/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

/*
 * SelectRamDefault,
    SelectRamDecLit,
    SelectRamHexLit,
    SelectRomDefault,
    SelectRomDecLit,
    SelectRomHexLit,
 */
bool MemorySelectInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("`", input, start))
  {
    return false;
  }
  
  // `R
  // `R[d10]
  // `R[&fc]
  // `D
  // `D[d10]
  // `D
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::ramSelect()))
  {
    instruction = Instruction::SelectRamHexLit;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long MemorySelectInstructionSyntax::consume(
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
