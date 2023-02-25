//
// Created by Richard Marks on 2/24/23.
//

#ifndef VC23_INSTRUCTIONS_H
#define VC23_INSTRUCTIONS_H

#include "InstructionSyntax.h"
#include "ParseUtils.h"

namespace vcc23
{
#define VCC23DECLAREINSTRUCTIONCLASS(CLASSNAME) \
  class CLASSNAME##InstructionSyntax : public InstructionSyntax \
  { \
  public: \
  bool peek( \
    const std::vector<Lexeme> &inputLexemes, \
    unsigned long inputOffset \
  ) override; \
  unsigned long consume( \
    ProgramNode &program, \
    const std::vector<Lexeme> &inputLexemes, \
    unsigned long inputOffset \
  ) override; }
  
  // declare all the instruction classes
  
  VCC23DECLAREINSTRUCTIONCLASS(Nop);
  
  VCC23DECLAREINSTRUCTIONCLASS(Assign);
  
  VCC23DECLAREINSTRUCTIONCLASS(Add);
  
  VCC23DECLAREINSTRUCTIONCLASS(Subtract);
  
  VCC23DECLAREINSTRUCTIONCLASS(Multiply);
  
  VCC23DECLAREINSTRUCTIONCLASS(Divide);
  
  VCC23DECLAREINSTRUCTIONCLASS(Negate);
  
  VCC23DECLAREINSTRUCTIONCLASS(Xor);
  
  VCC23DECLAREINSTRUCTIONCLASS(And);
  
  VCC23DECLAREINSTRUCTIONCLASS(Or);
  
  VCC23DECLAREINSTRUCTIONCLASS(LeftShift);
  
  VCC23DECLAREINSTRUCTIONCLASS(RightShift);
  
  VCC23DECLAREINSTRUCTIONCLASS(Compare);
  
  VCC23DECLAREINSTRUCTIONCLASS(Jump);
  
  VCC23DECLAREINSTRUCTIONCLASS(MemorySelect);
  
  VCC23DECLAREINSTRUCTIONCLASS(ReadInputDeviceImmediate);
  
  VCC23DECLAREINSTRUCTIONCLASS(ReadInputDevice);
  
  VCC23DECLAREINSTRUCTIONCLASS(WriteOutputDevice);
  
  VCC23DECLAREINSTRUCTIONCLASS(Print);
  
  VCC23DECLAREINSTRUCTIONCLASS(End);

#undef VCC23DECLAREINSTRUCTIONCLASS

}

#endif //VC23_INSTRUCTIONS_H
