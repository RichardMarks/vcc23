//
// Created by Richard Marks on 2/21/23.
//
#include "VCCReader.h"
#include "Parser.h"
#include "CodeGenerator.h"
#include "ByteOrder.h"
#include "MemBlock.h"

#include <regex>
#include <sstream>

using namespace vcc23;

CodeGenerator::CodeGenerator(VCCReader *reader, Parser *parser)
{
  readerPtr = reader;
  parserPtr = parser;
}

void CodeGenerator::generateROMTable()
{
  std::regex IS_DATA_DECLARATION_REGEX("^([dD]\\s+)([0-9a-fA-F]{2}\\s*){8}$");
  
  size_t dataLineCount = readerPtr->getDataLineCount();
  for (size_t i = 0; i < dataLineCount; i++)
  {
    auto line = readerPtr->getDataLine(i);
    bool isDataDeclaration = std::regex_search(line, IS_DATA_DECLARATION_REGEX);
    if (isDataDeclaration)
    {
      std::regex HEX_PAIRS("[0-9a-fA-F]{2}");
      auto start = std::sregex_iterator(line.begin(), line.end(), HEX_PAIRS);
      auto end = std::sregex_iterator();
      for (auto it = start; it != end; ++it)
      {
        auto value = static_cast<unsigned char>(std::stoi(it->str(), nullptr, 16));
        common::ByteOrder<unsigned char>::write(romTable, value);
      }
    }
  }
}

void CodeGenerator::generateProgramCode()
{
  visit(parserPtr->getSyntaxTree().get());
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"

void CodeGenerator::visit(ASTNode *node)
{
  if (!node)
  {
    return;
  }
  
  switch (node->getType())
  {
  case ASTNodeType::Program:
  {
    auto *program = reinterpret_cast<ProgramNode *>(node);
    auto &instructionNodes = program->getInstructions();
    for (auto &instructionNode: instructionNodes)
    {
      visit(instructionNode.get());
    }
  }
    break;
  case ASTNodeType::Instruction:
  {
    // instruction opcodes are written as 16-bit unsigned integers
    // next is an 8-bit unsigned integers specifying the number of
    // operands that follow
    auto *instruction = reinterpret_cast<InstructionNode *>(node);
    auto opcode = static_cast<unsigned short>(instruction->getOpcode());
    common::ByteOrder<unsigned short>::write(binaryCodeStream, opcode);
    auto &operandNodes = instruction->getOperands();
    auto numOperands = static_cast<unsigned char>(operandNodes.size());
    common::ByteOrder<unsigned char>::write(binaryCodeStream, numOperands);
    for (auto &operandNode: operandNodes)
    {
      visit(operandNode.get());
    }
  }
    break;
  case vcc23::ASTNodeType::Operand:
  {
    // operands are written as an 8-bit unsigned integer specifying the
    // size of the operand in number of bytes, so 1, 2, 4 or 8
    // followed by the operand value in either of
    // 8-bit unsigned integer
    // 16-bit unsigned integer
    // 32-bit unsigned integer
    // 64-bit unsigned integer
    auto *operand = reinterpret_cast<OperandNode *>(node);
    unsigned long opValue = operand->getValue();
    unsigned char opSize = 1;
    if (opValue <= 0xFF)
    {
      common::ByteOrder<unsigned char>::write(binaryCodeStream, opSize);
      auto u8Value = static_cast<unsigned char>(opValue);
      common::ByteOrder<unsigned char>::write(binaryCodeStream, u8Value);
    } else if (opValue > 0xFF)
    {
      opSize = 2;
      common::ByteOrder<unsigned char>::write(binaryCodeStream, opSize);
      auto u16Value = static_cast<unsigned short>(opValue);
      common::ByteOrder<unsigned short>::write(binaryCodeStream, u16Value);
    } else if (opValue > 0xFFFF)
    {
      opSize = 4;
      common::ByteOrder<unsigned char>::write(binaryCodeStream, opSize);
      auto u32Value = static_cast<unsigned int>(opValue);
      common::ByteOrder<unsigned char>::write(binaryCodeStream, u32Value);
    } else if (opValue > 0xFFFFFFFF)
    {
      opSize = 8;
      common::ByteOrder<unsigned char>::write(binaryCodeStream, opSize);
      common::ByteOrder<unsigned long>::write(binaryCodeStream, opValue);
    }
  }
    break;
  default:
    break;
  }
}

std::string CodeGenerator::info() const
{
  std::ostringstream os;
  
  os << "VCC CODE GENERATOR INFORMATION:" << std::endl;
  
  auto dataBytes = getROMTable();
  auto codeBytes = getCodeStream();
  
  common::MemBlock romBlock;
  common::MemBlock codeBlock;
  romBlock.initialize(dataBytes.size());
  codeBlock.initialize(codeBytes.size());
  
  for (auto &byt: dataBytes)
  {
    romBlock.writeU8(byt);
  }
  
  for (auto &byt: codeBytes)
  {
    codeBlock.writeU8(byt);
  }
  
  os << "ROM TABLE:" << std::endl;
  os << romBlock.hdstr();
  os << "CODE STREAM:" << std::endl;
  os << codeBlock.hdstr();
  
  return os.str();
}

#pragma clang diagnostic pop
