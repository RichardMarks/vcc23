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
    
    auto stopAfter = cmdLine.getSetting("stopafter");
    
    auto readerInfo = cmdLine.hasFlag("readerinfo");
    auto lexerInfo = cmdLine.hasFlag("lexerinfo");
    auto parserInfo = cmdLine.hasFlag("parserinfo");
    auto codegenInfo = cmdLine.hasFlag("codegeninfo");
    
    VCCReader reader;
    reader.loadVCCFromFile(sourceFile);
    
    if (readerInfo)
    {
      std::cout << reader.info() << std::endl;
    }
    
    if (stopAfter == "reader")
    {
      return EXIT_SUCCESS;
    }
    
    Lexer lexer(&reader);
    lexer.tokenizeAndReduceToLexemeStream();
    
    if (lexerInfo)
    {
      std::cout << lexer.info() << std::endl;
    }
    
    if (stopAfter == "lexer")
    {
      return EXIT_SUCCESS;
    }
    
    Parser parser(&lexer);
    parser.buildSyntaxTree();
    
    if (parserInfo)
    {
      std::cout << parser.info() << std::endl;
    }
    
    if (stopAfter == "parser")
    {
      return EXIT_SUCCESS;
    }
    
    CodeGenerator codeGenerator(&reader, &parser);
    codeGenerator.generateROMTable();
    codeGenerator.generateProgramCode();
    
    if (codegenInfo)
    {
      std::cout << codeGenerator.info() << std::endl;
    }
    
    if (stopAfter == "codegen")
    {
      return EXIT_SUCCESS;
    }
    
    auto romTable = codeGenerator.getROMTable();
    auto codeStream = codeGenerator.getCodeStream();
    
    CartWriter writer;
    writer.setName("12345678901234567890123456789012");
    writer.setData((romTable));
    writer.setCode(codeStream);
    writer.writeToFile(cartFile);
    
    return EXIT_SUCCESS;
  } catch (std::exception &ex)
  {
    std::cerr << ex.what() << std::endl;
    return EXIT_FAILURE;
  }
}
