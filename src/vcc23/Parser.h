//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_PARSER_H
#define VCC23_PARSER_H

#include "Syntax.h"
#include "SyntaxTree.h"

namespace vcc23
{
  class Lexer;
  class Lexeme;
  
  class Parser
  {
  private:
    Lexer *lexerPtr;
  
    std::unique_ptr<ASTNode> syntaxTree;
  
    unsigned long currentIndex;
  public:
    explicit Parser(Lexer *lexer);
    void buildSyntaxTree();
    [[nodiscard]] const std::unique_ptr<ASTNode> &getSyntaxTree() const
    {
      return syntaxTree;
    }
  
  };
}

#endif //VCC23_PARSER_H
