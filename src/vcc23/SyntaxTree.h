//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_SYNTAXTREE_H
#define VCC23_SYNTAXTREE_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include "Syntax.h"
#include "Lexer.h"

namespace vcc23
{
  enum class ASTNodeType
  {
    Program,
    Instruction,
    Operand
  };
  
  class ASTNode
  {
  public:
    virtual ~ASTNode() = default;
    
    [[nodiscard]] virtual ASTNodeType getType() const = 0;
    
    [[nodiscard]] virtual std::string typeName() const = 0;
    
    [[nodiscard]] bool isType(ASTNodeType t) const;
  };
  
  class ASTVisitor
  {
  public:
    virtual ~ASTVisitor() = default;
    
    virtual void visit(ASTNode *node) = 0;
  };
  
  class OperandNode : public ASTNode
  {
  private:
    unsigned long value;
  
  public:
    OperandNode(const OperandNode &other);
    OperandNode(OperandNode &&other) noexcept;
    OperandNode &operator=(const OperandNode &other);
    OperandNode &operator=(OperandNode &&other) noexcept;
    [[nodiscard]] ASTNodeType getType() const override;
    [[nodiscard]] std::string typeName() const override;
    explicit OperandNode(unsigned long num);
    [[nodiscard]] virtual unsigned long getValue() const;
  };
  
  class InstructionNode : public ASTNode
  {
  private:
    common::Instruction opcode;
    std::vector<std::unique_ptr<OperandNode>> operands;
  
  public:
    InstructionNode(const InstructionNode &other);
    InstructionNode(InstructionNode &&other) noexcept ;
    InstructionNode &operator=(const InstructionNode &other);
    InstructionNode &operator=(InstructionNode &&other) noexcept ;
  
    [[nodiscard]] ASTNodeType getType() const override;
    [[nodiscard]] std::string typeName() const override;
  
    InstructionNode(common::Instruction code, const std::vector<unsigned long> &operandData);
  
    [[nodiscard]] common::Instruction getOpcode() const;
    [[nodiscard]] const std::vector<std::unique_ptr<OperandNode>> &getOperands() const;
  };
  
  class ProgramNode : public ASTNode
  {
  private:
    std::vector<std::unique_ptr<InstructionNode>> instructions;
  
  public:
    [[nodiscard]] ASTNodeType getType() const override;
    [[nodiscard]] std::string typeName() const override;
    
    ProgramNode() = default;
    
    [[nodiscard]] const std::vector<std::unique_ptr<InstructionNode>> &getInstructions() const;
    
    void addInstructionNode(std::unique_ptr<InstructionNode> instruction);
  };
  
}

#endif //VCC23_SYNTAXTREE_H
