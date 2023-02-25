//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_INSTRUCTIONS_H
#define VC23_INSTRUCTIONS_H

#include "InstructionSyntax.h"

namespace vcc23
{
  class AssignmentInstructionSyntax : public InstructionSyntax
  {
  private:
    unsigned long address{};
    unsigned long sourceAddress{};
    unsigned long literal{};
    unsigned long size{};
  
  public:
    bool peek(std::vector<Lexeme> &input, unsigned long start) override;
    
    unsigned long consume() override;
    
    [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override;
  };
  
  class ReadInputDeviceInstructionSyntax : public InstructionSyntax
  {
  private:
    unsigned long address{};
    unsigned long sourceAddress{};
    unsigned long literal{};
    unsigned long device{};
    unsigned long size{};
  
  public:
    bool peek(std::vector<Lexeme> &input, unsigned long start) override;
    
    unsigned long consume() override;
    
    [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override;
  };
  
  class PrintInstructionSyntax : public InstructionSyntax
  {
  private:
    unsigned long address{};
    unsigned long sourceAddress{};
    unsigned long literal{};
    unsigned long size{};
  
  public:
    bool peek(std::vector<Lexeme> &input, unsigned long start) override;
    
    unsigned long consume() override;
    
    [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override;
  };
  
  class EndInstructionSyntax : public InstructionSyntax
  {
  private:
    unsigned long exitCode{};
    unsigned long size{};
  
  public:
    bool peek(std::vector<Lexeme> &input, unsigned long start) override;
    
    unsigned long consume() override;
    
    [[nodiscard]] std::unique_ptr<InstructionNode> getInstructionNode() const override;
  };
}

#endif //VC23_INSTRUCTIONS_H
