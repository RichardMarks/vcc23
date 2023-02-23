//
// Created by Richard Marks on 2/21/23.
//

#include "SyntaxTree.h"

using namespace vcc23;

[[nodiscard]] bool ASTNode::isType(ASTNodeType t) const
{
  return getType() == t;
}

OperandNode::OperandNode(const OperandNode &other)
  : value(other.value)
{}

OperandNode::OperandNode(OperandNode &&other) noexcept
  : value(other.value)
{}

OperandNode &OperandNode::operator=(const OperandNode &other)
{
  if (this != &other)
  {
    value = other.value;
  }
  return *this;
}

OperandNode &OperandNode::operator=(OperandNode &&other) noexcept
{
  if (this != &other)
  {
    value = other.value;
  }
  return *this;
}

[[nodiscard]] ASTNodeType OperandNode::getType() const
{
  return ASTNodeType::Operand;
}

[[nodiscard]] std::string OperandNode::typeName() const
{
  return "Operand";
}

OperandNode::OperandNode(unsigned long num) : value(num)
{
  std::cout << "OperandNode(" << num << ")" << std::endl;
}

[[nodiscard]]  unsigned long OperandNode::getValue() const
{
  return value;
}

InstructionNode::InstructionNode(const InstructionNode &other)
  : opcode(other.opcode)
{
  for (const auto &op: other.operands)
  {
    operands.push_back(std::make_unique<OperandNode>(*op));
  }
}

InstructionNode::InstructionNode(InstructionNode &&other)
noexcept: opcode(other.opcode),
          operands(std::move(other.operands))
{
}

InstructionNode &InstructionNode::operator=(const InstructionNode &other)
{
  if (this != &other)
  {
    // clear the existing vector and perform a deep copy of the vector and its contents
    operands.clear();
    for (const auto &op: other.operands)
    {
      operands.push_back(std::make_unique<OperandNode>(*op));
    }
    
    opcode = other.opcode;
  }
  
  return *this;
}

InstructionNode &InstructionNode::operator=(InstructionNode &&other)
noexcept
{
  if (this != &other)
  {
    opcode = other.opcode;
    operands = std::move(other.operands);
  }
  return *this;
}

[[nodiscard]] ASTNodeType InstructionNode::getType() const
{
  return ASTNodeType::Instruction;
};

[[nodiscard]] std::string InstructionNode::typeName() const
{
  return "Instruction";
};

InstructionNode::InstructionNode(common::Instruction code, const std::vector<unsigned long> &operandData) : opcode(code)
{
  std::cout << "InstructionNode(code:" << std::hex << (unsigned) code << ", operands:";
  for (auto val: operandData)
  {
    std::cout << val << ",";
  }
  std::cout << ")" << std::endl;
  for (auto val: operandData)
  {
    auto op = std::make_unique<OperandNode>(val);
    operands.emplace_back(std::move(op));
  }
}

[[nodiscard]] common::Instruction InstructionNode::getOpcode() const
{
  return opcode;
}

[[nodiscard]] const std::vector<std::unique_ptr<OperandNode>> &InstructionNode::getOperands() const
{
  return operands;
}

[[nodiscard]] ASTNodeType ProgramNode::getType() const
{
return ASTNodeType::Program;
};

[[nodiscard]] std::string ProgramNode::typeName() const
{
return "Program";
};

[[nodiscard]] const std::vector<std::unique_ptr<InstructionNode>> &ProgramNode::getInstructions() const
{
  return instructions;
}

void ProgramNode::addInstructionNode(std::unique_ptr<InstructionNode> instruction)
{
  instructions.emplace_back(std::move(instruction));
}