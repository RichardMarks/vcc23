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
#include <sstream>

using namespace vcc23;
using namespace common;

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
  WriteOutputDeviceInstructionSyntax writeOutputDeviceSyntax;
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
    {'o', &writeOutputDeviceSyntax},
    {'p', &printSyntax},
    {'q', &endSyntax}};
  
  std::vector<Lexeme> lexemes = lexerPtr->getLexemeStream();
  
  currentIndex = 0;
  
  syntaxTree = std::make_unique<ProgramNode>();
  auto *program = reinterpret_cast<ProgramNode *>(syntaxTree.get());
  
  while (currentIndex < lexemes.size())
  {
    auto token = lexemes.at(currentIndex);
    std::cout << "PARSER: [" << std::setw(4) << std::dec << currentIndex << "] " << token << std::endl;
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
        } else
        {
          std::cerr << "peek failed" << std::endl;
        }
      } else
      {
        std::cerr << "unknown token: " << token.data << std::endl;
      }
    } else
    {
      std::cerr << "unexpected token type: 0x" << std::hex << static_cast<int>(token.type) << std::dec << std::endl;
    }
    currentIndex++;
  }
}

std::string Parser::info()
{
  std::ostringstream os;
  
  os << "VCC PARSER INFORMATION:" << std::endl;
  
  auto *program = reinterpret_cast<ProgramNode *>(syntaxTree.get());
  auto &instructions = program->getInstructions();
  auto numInstructions = instructions.size();
  
  os << "INSTRUCTION COUNT: " << std::dec << numInstructions << std::endl;
  os << "INSTRUCTIONS: " << std::endl;
  for (auto &inode: instructions)
  {
    auto *instruction = reinterpret_cast<InstructionNode *>(inode.get());
    auto &operands = instruction->getOperands();
    auto numOperands = operands.size();
    os << "0x" << std::setfill('0') << std::setw(4) << std::hex
       << static_cast<unsigned short>(instruction->getOpcode())
       << std::dec << std::setfill('\0')
       << " " << instruction->typeName() << "(";
    for (auto i = 0; i < numOperands; i++)
    {
      os << operands.at(i);
      if (i < operands.size() - 1)
      {
        os << ", ";
      }
    }
    os << ")" << std::endl;
  }
  
  return os.str();
}
