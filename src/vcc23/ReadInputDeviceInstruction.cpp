//
// Created by Richard Marks on 2/24/23.
//

#include "Instructions.h"
#include "ParseUtils.h"
#include "SyntaxPatterns.h"

using namespace vcc23;

bool ReadInputDeviceInstructionSyntax::peek(std::vector<Lexeme> &input, unsigned long start)
{
  address = 0;
  sourceAddress = 0;
  literal = 0;
  device = 0;
  size = 0;
  instruction = Instruction::Unknown;
  
  if (!(input[start].isType(LexemeType::Instruction) && input[start].data == "r"))
  {
    std::cout << "not read input device" << std::endl;
    return false;
  }

// ReadInputDeviceDecLitRef
  std::vector<LexemeType> p1{
    LexemeType::DecimalPrefix,
    LexemeType::DecimalNumber,
    LexemeType::DevicePrefix,
    LexemeType::DecimalNumber,
    LexemeType::DeviceSuffix,
    LexemeType::AddressPrefix,
    LexemeType::DecimalNumber};

// ReadInputDeviceHexLitRef
  std::vector<LexemeType> p2{
    LexemeType::HexPrefix,
    LexemeType::HexNumber,
    LexemeType::DevicePrefix,
    LexemeType::DecimalNumber,
    LexemeType::DeviceSuffix,
    LexemeType::AddressPrefix,
    LexemeType::DecimalNumber};

// ReadInputDeviceRefRef
  std::vector<LexemeType> p3{
    LexemeType::AddressPrefix,
    LexemeType::DecimalNumber,
    LexemeType::DevicePrefix,
    LexemeType::DecimalNumber,
    LexemeType::DeviceSuffix,
    LexemeType::AddressPrefix,
    LexemeType::DecimalNumber};
  
  if (compare(input, start + 1, p1))
  {
// std::cout << "assign declitref" << std::endl;
    instruction = Instruction::ReadInputDeviceDecLitRef;
    
    literal = ParseUtils::parseULong(input[start + 2].data);
    device = ParseUtils::parseULong(input[start + 4].data);
    address = ParseUtils::translateAddressToken(input[start + 7].data);
    size = 7;
  } else if (compare(input, start + 1, p2))
  {
// std::cout << "assign hexlitref" << std::endl;
    instruction = Instruction::ReadInputDeviceHexLitRef;
    literal = ParseUtils::parseULong(input[start + 2].data, 16);
    device = ParseUtils::parseULong(input[start + 4].data);
    address = ParseUtils::translateAddressToken(input[start + 7].data);
    size = 7;
  } else if (compare(input, start + 1, p3))
  {
// std::cout << "assign refref" << std::endl;
    instruction = Instruction::ReadInputDeviceRefRef;
    sourceAddress = ParseUtils::translateAddressToken(input[start + 2].data);
    device = ParseUtils::parseULong(input[start + 4].data);
    address = ParseUtils::translateAddressToken(input[start + 7].data);
    size = 7;
  }
  
  if (instruction == Instruction::Unknown)
  {
    return false;
  }
  
  return true;
}

unsigned long ReadInputDeviceInstructionSyntax::consume()
{
  return size;
}

[[nodiscard]] std::unique_ptr<InstructionNode> ReadInputDeviceInstructionSyntax::getInstructionNode() const
{
  if (instruction == Instruction::ReadInputDeviceRefRef)
  {
    std::vector<unsigned long> operands{
      sourceAddress, device, address};
    return std::make_unique<InstructionNode>(instruction, std::move(operands));
  }
  std::vector<unsigned long> operands{
    literal, device, address};
  return std::make_unique<InstructionNode>(instruction, std::move(operands));
}