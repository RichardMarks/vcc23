//
// Created by Richard Marks on 2/24/23.
//

#include "InstructionSyntax.h"

#include "ParserErrors.h"
#include "ParseUtils.h"

#include <iostream>

using namespace vcc23;

unsigned long InstructionSyntax::build(
  const OperandSequenceMatchFunction &matchFunction,
  ProgramNode &program,
  const std::vector<Lexeme> &inputLexemes,
  unsigned long inputOffset
)
{
  auto match = matchFunction(inputLexemes, inputOffset);
  auto node = std::make_unique<InstructionNode>(
    instruction,
    std::move(match.operands)
  );
  program.addInstructionNode(std::move(node));
  return match.size;
}