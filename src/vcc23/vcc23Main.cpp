#include <iostream>
#include <stdexcept>

#include "CommandLine.h"

#include "VCCReader.h"
#include "Lexer.h"
#include "Parser.h"
#include "CodeGenerator.h"
#include "CartWriter.h"

using namespace common;
using namespace vcc23;

int main(int argc, char *argv[])
{
  try
  {
    CommandLine cmdLine(argc, argv);
  
    std::string messages[]{
      "source input filepath required",
      "cart output filepath required"};
    cmdLine.required(2, messages);
  
    std::string sourceFile = cmdLine.getArg(0);
    std::string cartFile = cmdLine.getArg(1);
  
    VCCReader reader;
    reader.loadVCCFromFile(sourceFile);
  
    Lexer lexer(&reader);
    lexer.tokenizeAndReduceToLexemeStream();
  
    Parser parser(&lexer);
    parser.buildSyntaxTree();
  
    CodeGenerator codeGenerator(&reader, &parser);
    codeGenerator.generateROMTable();
    codeGenerator.generateProgramCode();
  
    auto romTable = codeGenerator.getROMTable();
    auto codeStream = codeGenerator.getCodeStream();
  
    CartWriter writer;
    writer.setName("12345678901234567890123456789012");
    writer.setData((romTable));
    writer.setCode(codeStream);
    writer.writeToFile(cartFile);
  
    return EXIT_SUCCESS;
  } catch (std::exception& ex) {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
