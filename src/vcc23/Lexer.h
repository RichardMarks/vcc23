//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_LEXER_H
#define VCC23_LEXER_H

#include <utility>
#include <vector>
#include <string>
#include <iomanip>

namespace vcc23
{
  enum class LexemeType : unsigned char
  {
    Unknown,
    Instruction,
    DecimalPrefix,
    HexPrefix,
    AddressPrefix,
    DevicePrefix,
    DeviceSuffix,
    IndexPrefix,
    IndexSuffix,
    ROMSelect,
    RAMSelect,
    DecimalNumber,
    HexNumber
  };
  
  class Lexeme
  {
  public:
    LexemeType type;
    std::string data;
    
    Lexeme() : type(LexemeType::Unknown), data()
    {}
    
    Lexeme(LexemeType t, std::string d) : type(t), data(std::move(d))
    {}
    
    Lexeme(const Lexeme &other) = default;
    
    Lexeme(Lexeme &&other) noexcept: type(other.type), data(std::move(other.data))
    {}
    
    Lexeme &operator=(const Lexeme &other) = default;
    
    Lexeme &operator=(Lexeme &&other) noexcept
    {
      type = other.type;
      data = std::move(other.data);
      return *this;
    }
    
    bool operator==(const Lexeme &other) const
    {
      return type == other.type && data == other.data;
    }
    
    int operator<(const Lexeme &other) const
    {
      return type < other.type && data < other.data;
    }
    
    int operator>(const Lexeme &other) const
    {
      return type > other.type && data > other.data;
    }
    
    friend std::ostream &operator<<(std::ostream &os, const Lexeme &l)
    {
      auto desc = (l.isType(LexemeType::DecimalNumber) || l.isType(LexemeType::HexNumber)) ? "NUMBER" : l.name();
      os << std::setw(16) << desc << ": {0x" << std::hex << static_cast<unsigned>(l.type) << "} \"" << l.data << "\"";
      return os;
    }
    
    [[nodiscard]] bool isType(LexemeType t) const
    {
      return type == t;
    }
    
    [[nodiscard]] std::string name() const;
  };
  
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
  };
}

#endif //VCC23_LEXER_H
