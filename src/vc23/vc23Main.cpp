//
// Created by Richard Marks on 2/22/23.
//

#include "vc23.h"
#include "CommandLine.h"

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
    
    Cart cart;
    CPU cpu;
    
    cart.load(cartFile);
    
    if (cmdLine.hasFlag("debug"))
    {
      cpu.setFlags(CpuFlags::EnableDebug | CpuFlags::EnableStdoutDevice);
    }
    cpu.boot(cart);
    
    while (cpu.running())
    {
      cpu.step();
    }
  
  VC23_SAFE_BLOCK_END
  
  return 0;
}
