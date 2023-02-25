//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_LEXEME_H
#define VC23_LEXEME_H

namespace vcc23
{
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
}

#endif //VC23_LEXEME_H
