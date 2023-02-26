//
// Created by Richard Marks on 2/24/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool EndInstructionSyntax::peek(const std::vector<Lexeme> &input, unsigned long start)
{
  instruction = Instruction::Unknown;
  
  if (!ParseUtils::matchInstructionToken("q", input, start))
  {
    return false;
  }
  
  instruction = Instruction::EndWithSuccess;
  
  try
  {
    if (ParseUtils::compare(input, start + 1, SyntaxPatterns::decLit()))
    {
      instruction = Instruction::EndWithDecLitExitCode;
    } else if (ParseUtils::compare(input, start + 1, SyntaxPatterns::hexLit()))
    {
      instruction = Instruction::EndWithHexLitExitCode;
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

unsigned long EndInstructionSyntax::consume(
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  switch (instruction)
  {
  case Instruction::EndWithDecLitExitCode:
  {
    return build(
      ParseUtils::matchDecLit,
      program,
      inputLexemes,
      inputOffset
    );
  }
  case Instruction::EndWithHexLitExitCode:
  {
    return build(
      ParseUtils::matchHexLit,
      program,
      inputLexemes,
      inputOffset
    );
  }
  case Instruction::EndWithSuccess:
  {
    return build(
      ParseUtils::matchNONE,
      program,
      inputLexemes,
      inputOffset
    );
  }
  default:
    return 0;
  }
}
