//
// Created by Richard Marks on 2/25/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool WriteOutputDeviceInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("o", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::decLitDev()))
  {
    instruction = Instruction::WriteOutputDeviceDecLit;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::hexLitDev()))
  {
    instruction = Instruction::WriteOutputDeviceHexLit;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::refDev()))
  {
    instruction = Instruction::WriteOutputDeviceRef;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long WriteOutputDeviceInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::WriteOutputDeviceDecLit:
  {
    return build(
      ParseUtils::matchDecLitDev,
      program,
      inputLexemes,
      inputOffset
    );
  }
  
  case Instruction::WriteOutputDeviceHexLit:
  {
    return build(
      ParseUtils::matchHexLitDev,
      program,
      inputLexemes,
      inputOffset
    );
  }
  
  case Instruction::WriteOutputDeviceRef:
  {
    return build(
      ParseUtils::matchRefDev,
      program,
      inputLexemes,
      inputOffset
    );
  }
  default:
    return 0;
  }
}
