//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_PARSEUTILS_H
#define VC23_PARSEUTILS_H

#include "LexemeType.h"
#include "Lexeme.h"

#include <string>
#include <vector>

namespace vcc23
{
  struct MatchResult
  {
    unsigned long size;
    std::vector<unsigned long> operands;
  };
  
  class ParseUtils
  {
  public:
    static bool isValidAddressToken(const std::string &token);
    
    static unsigned long parseULong(const std::string &token, int base = 10);
    
    static unsigned long translateAddressToken(const std::string &token);
    
    static bool matchInstructionToken(
      const std::string &expected,
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchInsDecLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchInsHexLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchInsRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchDecLitRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchDecLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchDecLitDevRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchHexLitRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchHexLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchHexLitDevRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRefDevRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchDevRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRefRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRef(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static bool compare(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset,
      const std::vector<LexemeType> &pattern);
    
    static MatchResult matchNONE(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchDecLitDev(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchHexLitDev(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRefDev(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRamDecLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRamHexLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRomDecLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
    
    static MatchResult matchRomHexLit(
      const std::vector<Lexeme> &inputLexemes,
      unsigned long inputOffset);
  };
  
}

#endif //VC23_PARSEUTILS_H
