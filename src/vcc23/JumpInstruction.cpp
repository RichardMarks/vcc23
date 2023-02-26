//
// Created by Richard Marks on 2/25/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool JumpInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("g", input, start))
  {
    return false;
  }
  
  if (ParseUtils::compare(input, start + 1, SyntaxPatterns::insDecLit()))
  {
    if (ParseUtils::matchInstructionToken("|", input, start + 1))
    {
      // absolute jump
      instruction = Instruction::AbsJumpDecLit;
    } else if (ParseUtils::matchInstructionToken("<", input, start + 1))
    {
      // relative back
      instruction = Instruction::RelJumpBackDecLit;
    } else if (ParseUtils::matchInstructionToken(">", input, start + 1))
    {
      // relative forward
      instruction = Instruction::RelJumpForwardDecLit;
    }
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::insHexLit()))
  {
    if (ParseUtils::matchInstructionToken("|", input, start + 1))
    {
      // absolute jump
      instruction = Instruction::AbsJumpHexLit;
    } else if (ParseUtils::matchInstructionToken("<", input, start + 1))
    {
      // relative back
      instruction = Instruction::RelJumpBackHexLit;
    } else if (ParseUtils::matchInstructionToken(">", input, start + 1))
    {
      // relative forward
      instruction = Instruction::RelJumpForwardHexLit;
    }
  } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::insRef()))
  {
    if (ParseUtils::matchInstructionToken("|", input, start + 1))
    {
      // absolute jump
      instruction = Instruction::AbsJumpRef;
    } else if (ParseUtils::matchInstructionToken("<", input, start + 1))
    {
      // relative back
      instruction = Instruction::RelJumpBackRef;
    } else if (ParseUtils::matchInstructionToken(">", input, start + 1))
    {
      // relative forward
      instruction = Instruction::RelJumpForwardRef;
    }
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long JumpInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::AbsJumpDecLit:
  {
    return build(
      ParseUtils::matchInsDecLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::AbsJumpHexLit:
  {
    return build(
      ParseUtils::matchInsHexLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::AbsJumpRef:
  {
    return build(
      ParseUtils::matchInsRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpBackDecLit:
  {
    return build(
      ParseUtils::matchInsDecLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpBackHexLit:
  {
    return build(
      ParseUtils::matchInsHexLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpBackRef:
  {
    return build(
      ParseUtils::matchInsRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpForwardDecLit:
  {
    return build(
      ParseUtils::matchInsDecLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpForwardHexLit:
  {
    return build(
      ParseUtils::matchInsHexLit,
      program,
      inputLexemes,
      inputOffset);
  }
  
  case Instruction::RelJumpForwardRef:
  {
    return build(
      ParseUtils::matchInsRef,
      program,
      inputLexemes,
      inputOffset);
  }
  
  default:
    return 0;
  }
}


