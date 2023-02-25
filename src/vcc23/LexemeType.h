//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_LEXEMETYPE_H
#define VC23_LEXEMETYPE_H

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
}

#endif //VC23_LEXEMETYPE_H
