//
// Created by Richard Marks on 2/22/23.
//

#include "CommandLine.h"
#include "ConsoleRuntime.h"

using namespace vc23;

using namespace common;

int main(int argc, char *argv[])
{
  VC23_SAFE_BLOCK_START
    CommandLine cmdLine(argc, argv);
    std::string messages[]{
      "cart input filepath required"};
    cmdLine.required(1, messages);
    
    std::string cartFile = cmdLine.getArg(0);
    
    ConsoleRuntime runtime;
    
    if (cmdLine.hasFlag("debug"))
    {
      runtime.setFlag(RuntimeFlags::Debug);
    }
    
    if (cmdLine.hasFlag("trace"))
    {
      runtime.setFlag(RuntimeFlags::Trace);
    }
    
    if (cmdLine.hasFlag("tracedump"))
    {
      runtime.setFlag(RuntimeFlags::TraceDump);
    }
    
    runtime.initialize();
    
    runtime.load(cartFile);
    
    if (cmdLine.hasFlag("debug"))
    {
      runtime.dumpDataBlock();
      runtime.dumpCodeBlock();
    }
    
    while (runtime.running())
    {
      runtime.step();
    }
    
    runtime.shutdown();
    
    return runtime.getExitCode();
  
  VC23_SAFE_BLOCK_END
  
  return EXIT_FAILURE;
}
