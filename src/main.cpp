#include <iostream>

#include "CommandLine.h"

#include "VCCReader.h"

using namespace common;
using namespace vcc23;

int main(int argc, char *argv[])
{
  CommandLine cmdLine(argc, argv);
  
  std::string messages[]{
    "source input filepath required",
    "cart output filepath required"};
  cmdLine.required(2, messages);
  
  std::string sourceFile = cmdLine.getArg(0);
  std::string cartFile = cmdLine.getArg(1);
  
  vcc23::VCCReader reader;
  reader.loadVCCFromFile(sourceFile);
  
  return 0;
}
