//
// Created by Richard Marks on 2/28/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool MemorySelectInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("`", input, start))
  {
    return false;
  }
  
  if (input.size() < start + 1)
  {
    // oh no, we're out of tokens O_o
    return false;
  }
  
  if (!((input.at(start + 1).isType(LexemeType::RAMSelect)) || (input.at(start + 1).isType(LexemeType::ROMSelect))))
  {
    // unexpected memory selector token
    return false;
  }
  
  if (input.size() < start + 2)
  {
    // oh no, we're out of tokens O_o
    return false;
  }
  
  if (!(input.at(start + 2).isType(LexemeType::DevicePrefix)))
  {
    // we are going to be in default select mode
    if (input.at(start + 1).isType(LexemeType::RAMSelect))
    {
      instruction = Instruction::SelectRamDefault;
    } else if (input.at(start + 1).isType(LexemeType::ROMSelect))
    {
      instruction = Instruction::SelectRomDefault;
    }
    return true;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::ramDecLit()))
  {
    instruction = Instruction::SelectRamDecLit;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::ramHexLit()))
  {
    instruction = Instruction::SelectRamHexLit;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::romDecLit()))
  {
    instruction = Instruction::SelectRomDecLit;
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::romHexLit()))
  {
    instruction = Instruction::SelectRomHexLit;
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
  if (instruction == Instruction::SelectRamDefault || instruction == Instruction::SelectRomDefault)
  {
    return build(ParseUtils::matchNONE, program, inputLexemes, inputOffset);
  }
  
  switch (instruction)
  {
  case Instruction::SelectRamDecLit:
  {
    return build(ParseUtils::matchRamDecLit, program, inputLexemes, inputOffset);
  }
  case Instruction::SelectRomDecLit:
  {
    return build(ParseUtils::matchRomDecLit, program, inputLexemes, inputOffset);
  }
  case Instruction::SelectRamHexLit:
  {
    return build(ParseUtils::matchRamHexLit, program, inputLexemes, inputOffset);
  }
  case Instruction::SelectRomHexLit:
  {
    return build(ParseUtils::matchRomHexLit, program, inputLexemes, inputOffset);
  }
  
  default:
    return 0;
  }
}
