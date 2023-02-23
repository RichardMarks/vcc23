//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_CODEGENERATOR_H
#define VCC23_CODEGENERATOR_H

#include "SyntaxTree.h"

namespace vcc23
{
  class Parser;
  
  class VCCReader;
  
  class CodeGenerator : public ASTVisitor
  {
  private:
    VCCReader *readerPtr;
    Parser *parserPtr;
    
    std::vector<unsigned char> binaryCodeStream;
    std::vector<unsigned char> romTable;
  
  public:
    CodeGenerator(VCCReader *reader, Parser *parser);
    
    void generateROMTable();
    
    void generateProgramCode();
    
    void visit(ASTNode *node) override;
    
    [[nodiscard]] std::vector<unsigned char> getCodeStream() const
    {
      return {binaryCodeStream.begin(), binaryCodeStream.end()};
    }
    
    [[nodiscard]] std::vector<unsigned char> getROMTable() const
    {
      return {romTable.begin(), romTable.end()};
    }
  };
}

#endif //VCC23_CODEGENERATOR_H
