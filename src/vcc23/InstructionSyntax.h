//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_INSTRUCTIONSYNTAX_H
#define VC23_INSTRUCTIONSYNTAX_H

#include "SyntaxTree.h"
#include "Syntax.h"
#include "Lexeme.h"

#include <vector>
#include <unordered_map>
#include <functional>

namespace vcc23
{
  using common::Instruction;
  struct MatchResult;
  typedef std::function<MatchResult(const std::vector<Lexeme> &inputLexemes,
                                    unsigned long inputOffset)> OperandSequenceMatchFunction;
  
  class InstructionSyntax
  {
  protected:
    Instruction instruction{Instruction::Unknown};
    
    unsigned long build(
      const OperandSequenceMatchFunction &matchFunction,
      ProgramNode &program,
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset
    );
  
  public:
    
    virtual bool peek(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset
    ) = 0;
    
    virtual unsigned long consume(
      ProgramNode &program,
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset
    ) = 0;
  };
}

#endif //VC23_INSTRUCTIONSYNTAX_H
