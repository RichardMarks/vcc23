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

namespace vcc23
{
  using common::Instruction;
  
  constexpr const char *SYNTAX_DECIMAL_LITERAL_REF = "DCR";
  
  class InstructionSyntax
  {
  protected:
    static std::unordered_map<std::string, std::vector<LexemeType>> syntaxPatterns;
    
    Instruction instruction{Instruction::Unknown};
    
    static bool compare(std::vector<Lexeme> &input, unsigned long start, const std::vector<LexemeType> &pattern);
  
  public:
    
    virtual bool peek(std::vector<Lexeme> &input, unsigned long start) = 0;
    
    virtual unsigned long consume() = 0;
    
    [[nodiscard]] virtual std::unique_ptr<InstructionNode> getInstructionNode() const = 0;
    
  };
}

#endif //VC23_INSTRUCTIONSYNTAX_H
