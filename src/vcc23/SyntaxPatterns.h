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
    
    const std::vector<LexemeType> DECLITDEVREF
      {
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
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
    
    const std::vector<LexemeType> HEXLITDEVREF
      {
        LexemeType::HexPrefix,
        LexemeType::HexNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix,
        LexemeType::AddressPrefix,
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
    
    const std::vector<LexemeType> DEVREF
      {
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> REFDEVREF
      {
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
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
    
    const std::vector<LexemeType> INSDECLIT
      {
        LexemeType::Instruction,
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> INSHEXLIT
      {
        LexemeType::Instruction,
        LexemeType::HexPrefix,
        LexemeType::HexNumber
      };
    
    const std::vector<LexemeType> INSREF
      {
        LexemeType::Instruction,
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber
      };
    
    const std::vector<LexemeType> REFDEV
      {
        LexemeType::AddressPrefix,
        LexemeType::DecimalNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix
      };
    
    const std::vector<LexemeType> DECLITDEV
      {
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix
      };
    
    const std::vector<LexemeType> HEXLITDEV
      {
        LexemeType::HexPrefix,
        LexemeType::HexNumber,
        LexemeType::DevicePrefix,
        LexemeType::DecimalNumber,
        LexemeType::DeviceSuffix
      };
    
    /*
     *          SELECT: {0x1} "`"
             RAM: {0xa} "R"
    INDEX_PREFIX: {0x7} "["
          NUMBER: {0xb} "1024"
    INDEX_SUFFIX: {0x8} "]"
     * */
    
    
    const std::vector<LexemeType> RAMSELECTDECLIT
      {
        LexemeType::RAMSelect,
        LexemeType::IndexPrefix,
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber,
        LexemeType::IndexSuffix
      };
    
    const std::vector<LexemeType> RAMSELECTHEXLIT
      {
        LexemeType::RAMSelect,
        LexemeType::IndexPrefix,
        LexemeType::HexPrefix,
        LexemeType::HexNumber,
        LexemeType::IndexSuffix
      };
    
    const std::vector<LexemeType> ROMSELECTDECLIT
      {
        LexemeType::ROMSelect,
        LexemeType::IndexPrefix,
        LexemeType::DecimalPrefix,
        LexemeType::DecimalNumber,
        LexemeType::IndexSuffix
      };
    
    const std::vector<LexemeType> ROMSELECTHEXLIT
      {
        LexemeType::RAMSelect,
        LexemeType::IndexPrefix,
        LexemeType::HexPrefix,
        LexemeType::HexNumber,
        LexemeType::IndexSuffix
      };
  
  public:
    SyntaxPatterns(SyntaxPatterns const &) = delete;
    
    SyntaxPatterns &operator=(SyntaxPatterns const &) = delete;
    
    static const std::vector<LexemeType> &ramDecLit()
    { return SyntaxPatterns::instance().RAMSELECTDECLIT; };
    
    static const std::vector<LexemeType> &romDecLit()
    { return SyntaxPatterns::instance().ROMSELECTDECLIT; };
    
    static const std::vector<LexemeType> &ramHexLit()
    { return SyntaxPatterns::instance().RAMSELECTHEXLIT; };
    
    static const std::vector<LexemeType> &romHexLit()
    { return SyntaxPatterns::instance().ROMSELECTHEXLIT; };
    
    static const std::vector<LexemeType> &insDecLit()
    { return SyntaxPatterns::instance().INSDECLIT; };
    
    static const std::vector<LexemeType> &insHexLit()
    { return SyntaxPatterns::instance().INSHEXLIT; };
    
    static const std::vector<LexemeType> &insRef()
    { return SyntaxPatterns::instance().INSREF; };
    
    static const std::vector<LexemeType> &decLitDevRef()
    { return SyntaxPatterns::instance().DECLITDEVREF; };
    
    static const std::vector<LexemeType> &decLitRef()
    { return SyntaxPatterns::instance().DECLITREF; };
    
    static const std::vector<LexemeType> &hexLitDevRef()
    { return SyntaxPatterns::instance().HEXLITDEVREF; };
    
    static const std::vector<LexemeType> &hexLitRef()
    { return SyntaxPatterns::instance().HEXLITREF; };
    
    static const std::vector<LexemeType> &decLit()
    { return SyntaxPatterns::instance().DECLIT; };
    
    static const std::vector<LexemeType> &hexLit()
    { return SyntaxPatterns::instance().HEXLIT; };
    
    static const std::vector<LexemeType> &ref()
    { return SyntaxPatterns::instance().REF; };
    
    static const std::vector<LexemeType> &refDevRef()
    { return SyntaxPatterns::instance().REFDEVREF; };
    
    static const std::vector<LexemeType> &devRef()
    { return SyntaxPatterns::instance().DEVREF; };
    
    static const std::vector<LexemeType> &refDev()
    { return SyntaxPatterns::instance().REFDEV; };
    
    static const std::vector<LexemeType> &decLitDev()
    { return SyntaxPatterns::instance().DECLITDEV; };
    
    static const std::vector<LexemeType> &hexLitDev()
    { return SyntaxPatterns::instance().HEXLITDEV; };
    
    static const std::vector<LexemeType> &refRef()
    { return SyntaxPatterns::instance().REFREF; };
    
  };
}

#endif //VC23_SYNTAXPATTERNS_H
