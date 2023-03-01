//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_LEXER_H
#define VCC23_LEXER_H

#include <utility>
#include <vector>
#include <string>
#include <iomanip>

#include "LexemeType.h"
#include "Lexeme.h"

namespace vcc23
{
  class VCCReader;
  
  class Lexer
  {
  private:
    
    VCCReader *readerPtr;
    std::vector<Lexeme> lexemes;
    
    static std::vector<std::string> tokenize(const std::string &line);
    
    static std::vector<Lexeme> transform(const std::vector<std::string> &rawTokens);
    
    static LexemeType identify(const std::string &token);
  
  public:
    explicit Lexer(VCCReader *reader);
    
    void tokenizeAndReduceToLexemeStream();
    
    [[nodiscard]] std::vector<Lexeme> getLexemeStream() const
    {
      return {lexemes.begin(), lexemes.end()};
    };
    
    
    std::string info();
  };
}

#endif //VCC23_LEXER_H
