//
// Created by Richard Marks on 2/21/23.
//
#include "Lexer.h"
#include "Parser.h"
#include "ParserErrors.h"
#include "InstructionSyntax.h"
#include "Instructions.h"

#include <unordered_map>
#include <vector>
#include <string>

using namespace vcc23;
using namespace common;

// std::unordered_map<Instruction, std::vector<LexemeType>> SYNTAX_MAP{
//     {Instruction::Nop, {}},

//     {Instruction::AssignDecLitRef, DECLITREF},
//     {Instruction::AssignHexLitRef, HEXLITREF},
//     {Instruction::AssignRefRef, REFREF},

//     {Instruction::AddDecLitRef, DECLITREF},
//     {Instruction::AddHexLitRef, HEXLITREF},
//     {Instruction::AddRefRef, REFREF},

//     {Instruction::SubDecLitRef, DECLITREF},
//     {Instruction::SubHexLitRef, HEXLITREF},
//     {Instruction::SubRefRef, REFREF},

//     {Instruction::MulDecLitRef, DECLITREF},
//     {Instruction::MulHexLitRef, HEXLITREF},
//     {Instruction::MulRefRef, REFREF},

//     {Instruction::DivDecLitRef, DECLITREF},
//     {Instruction::DivHexLitRef, HEXLITREF},
//     {Instruction::DivRefRef, REFREF},

//     {Instruction::NegRef, REF},

//     {Instruction::XorDecLitRef, DECLITREF},
//     {Instruction::XorHexLitRef, HEXLITREF},
//     {Instruction::XorRefRef, REFREF},

//     {Instruction::AndDecLitRef, DECLITREF},
//     {Instruction::AndHexLitRef, HEXLITREF},
//     {Instruction::AndRefRef, REFREF},

//     {Instruction::OrDecLitRef, DECLITREF},
//     {Instruction::OrHexLitRef, HEXLITREF},
//     {Instruction::OrRefRef, REFREF},

//     {Instruction::LeftShiftDecLitRef, DECLITREF},
//     {Instruction::LeftShiftHexLitRef, HEXLITREF},

//     {Instruction::CmpDecLitRef, DECLITREF},
//     {Instruction::CmpHexLitRef, HEXLITREF},
//     {Instruction::CmpRefRef, REFREF},
//     {Instruction::CmpRef, REF},

//     // this might cause some problems if not careful..
//     // have to double check that the instruction is actually "|"
//     {Instruction::AbsJumpDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::AbsJumpHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::AbsJumpRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     // have to double check that the instruction is actually ">"
//     {Instruction::RelJumpForwardDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::RelJumpForwardHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::RelJumpForwardRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     // have to double check that the instruction is actually "<"
//     {Instruction::RelJumpBackDecLit, {LexemeType::Instruction, LexemeType::DecimalPrefix, LexemeType::DecimalNumber}},
//     {Instruction::RelJumpBackHexLit, {LexemeType::Instruction, LexemeType::HexPrefix, LexemeType::HexNumber}},
//     {Instruction::RelJumpBackRef, {LexemeType::Instruction, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::SelectRamDefault, {LexemeType::RAMSelect}},
//     {Instruction::SelectRamDecLit, {LexemeType::RAMSelect, LexemeType::IndexPrefix, LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::IndexSuffix}},
//     {Instruction::SelectRamHexLit, {LexemeType::RAMSelect, LexemeType::IndexPrefix, LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::IndexSuffix}},

//     {Instruction::SelectRomDefault, {LexemeType::ROMSelect}},
//     {Instruction::SelectRomDecLit, {LexemeType::ROMSelect, LexemeType::IndexPrefix, LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::IndexSuffix}},
//     {Instruction::SelectRomHexLit, {LexemeType::ROMSelect, LexemeType::IndexPrefix, LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::IndexSuffix}},

//     {Instruction::ReadInputDeviceRef, {LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::ReadInputDeviceDecLitRef, {LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},
//     {Instruction::ReadInputDeviceHexLitRef, {LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::ReadInputDeviceRefRef, {LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix, LexemeType::AddressPrefix, LexemeType::DecimalNumber}},

//     {Instruction::WriteOutputDeviceDecLit, {LexemeType::DecimalPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},
//     {Instruction::WriteOutputDeviceHexLit, {LexemeType::HexPrefix, LexemeType::HexNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},

//     {Instruction::WriteOutputDeviceRef, {LexemeType::AddressPrefix, LexemeType::DecimalNumber, LexemeType::DevicePrefix, LexemeType::DecimalNumber, LexemeType::DeviceSuffix}},

//     {Instruction::PrintDecLitRef, DECLITREF},
//     {Instruction::PrintHexLitRef, HEXLITREF},
//     {Instruction::PrintRefRef, REFREF},

//     {Instruction::EndWithDecLitExitCode, DECLIT},
//     {Instruction::EndWithHexLitExitCode, HEXLIT},

//     {Instruction::EndWithSuccess, {}}
//     //
// };

// std::unordered_map<char, Instruction> INSTRUCTION_MAP{
//     {'~', Instruction::Nop},
//     //
// };

// all instruction matchers

//

//

//

//

Parser::Parser(Lexer *lexer) : lexerPtr(lexer)
{
}

void Parser::buildSyntaxTree()
{
  
  NopInstructionSyntax nopSyntax;
  AssignInstructionSyntax assignSyntax;
  AddInstructionSyntax addSyntax;
  SubtractInstructionSyntax subSyntax;
  MultiplyInstructionSyntax mulSyntax;
  DivideInstructionSyntax divSyntax;
  NegateInstructionSyntax negSyntax;
  XorInstructionSyntax xorSyntax;
  AndInstructionSyntax andSyntax;
  OrInstructionSyntax orSyntax;
  LeftShiftInstructionSyntax leftShiftSyntax;
  RightShiftInstructionSyntax rightShiftSyntax;
  CompareInstructionSyntax cmpSyntax;
  ZCompareInstructionSyntax zcmpSyntax;
  JumpInstructionSyntax jmpSyntax;
  ReadInputDeviceImmediateInstructionSyntax ridSyntax;
  ReadInputDeviceInstructionSyntax readInputDeviceSyntax;
  PrintInstructionSyntax printSyntax;
  EndInstructionSyntax endSyntax;
  
  std::unordered_map<char, InstructionSyntax *> syntaxTable{
    {'~', &nopSyntax},
    {'.', &assignSyntax},
    {'+', &addSyntax},
    {'-', &subSyntax},
    {'*', &mulSyntax},
    {'/', &divSyntax},
    {'_', &negSyntax},
    {'^', &xorSyntax},
    {'&', &andSyntax},
    {'|', &orSyntax},
    {'<', &leftShiftSyntax},
    {'>', &rightShiftSyntax},
    {'=', &cmpSyntax},
    {'z', &zcmpSyntax},
    {'g', &jmpSyntax},
    {'i', &ridSyntax},
    {'r', &readInputDeviceSyntax},
    {'p', &printSyntax},
    {'q', &endSyntax}};
  
  std::vector<Lexeme> lexemes = lexerPtr->getLexemeStream();
  
  currentIndex = 0;
  
  syntaxTree = std::make_unique<ProgramNode>();
  auto *program = reinterpret_cast<ProgramNode *>(syntaxTree.get());
  
  while (currentIndex < lexemes.size())
  {
    auto token = lexemes.at(currentIndex);
    std::cout << "[" << std::setw(4) << currentIndex << "] " << token << std::endl;
    if (token.isType(LexemeType::Instruction))
    {
      if (token.data.size() == 1 && syntaxTable.count(token.data[0]))
      {
        auto syntax = syntaxTable.at(token.data[0]);
        if (syntax->peek(lexemes, currentIndex))
        {
          auto advanced = syntax->consume(*program, lexemes, currentIndex);
          // std::cout << "advancing " << advanced << std::endl;
          currentIndex += advanced;
        }
      } else
      {
        std::cerr << "unknown token: " << token.data << std::endl;
      }
    }
    currentIndex++;
  }
}
