//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_SYNTAXPATTERNS_H
#define VC23_SYNTAXPATTERNS_H

#include "LexemeType.h"

#include <vector>
#include <memory>

namespace vcc23
{
  class SyntaxPatterns
  {
  private:
    SyntaxPatterns() = default;
    
    ~SyntaxPatterns() = default;
    
    static SyntaxPatterns &instance()
    {
      static SyntaxPatterns singletonInstance;
      return singletonInstance;
    }
    
    const std::vector<LexemeType> DECLITREF
      {
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> DECLIT
      {
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> HEXLITREF
      {
        LexemeType::HexPrefix,
        LexemeType::HexNumber,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> HEXLIT
      {
        LexemeType::HexPrefix,
        LexemeType::HexNumber
      };
    
    const std::vector<LexemeType> REFREF
      {
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> REF
      {
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
  public:
    SyntaxPatterns(SyntaxPatterns const &) = delete;
    
    SyntaxPatterns &operator=(SyntaxPatterns const &) = delete;
    
    static const std::vector<LexemeType> &decLitRef()
    { return SyntaxPatterns::instance().DECLITREF; };
    
    static const std::vector<LexemeType> &hexLitRef()
    { return SyntaxPatterns::instance().HEXLITREF; };
    
    static const std::vector<LexemeType> &decLit()
    { return SyntaxPatterns::instance().DECLIT; };
    
    static const std::vector<LexemeType> &hexLit()
    { return SyntaxPatterns::instance().HEXLIT; };
    
    static const std::vector<LexemeType> &ref()
    { return SyntaxPatterns::instance().REF; };
    
    static const std::vector<LexemeType> &refRef()
    { return SyntaxPatterns::instance().REFREF; };
    
  };
}

#endif //VC23_SYNTAXPATTERNS_H
