//
// Created by Richard Marks on 2/26/23.
//

#ifndef VC23_CONSOLERUNTIME_H
#define VC23_CONSOLERUNTIME_H

#include "Runtime.h"
#include "MemBlock.h"
#include "Syntax.h"
#include "Devices.h"

#include <memory>

namespace vc23
{
  namespace constants
  {
//    constexpr size_t RUNTIME_RAM_SIZE = 0x2000;
    constexpr size_t RUNTIME_RAM_SIZE = 0x80;
  }
  
  enum class RuntimeFlags : unsigned char
  {
    Default = 0x00,
    Debug = 0x01,
    Trace = 0x02,
    TraceDump = 0x04,
  };
  
  static RuntimeFlags operator|(RuntimeFlags a, RuntimeFlags b)
  {
    return static_cast<RuntimeFlags>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
  }
  
  static RuntimeFlags operator&(RuntimeFlags a, RuntimeFlags b)
  {
    return static_cast<RuntimeFlags>(static_cast<unsigned char>(a) & static_cast<unsigned char>(b));
  }
  
  static RuntimeFlags operator~(RuntimeFlags a)
  {
    return static_cast<RuntimeFlags>(~static_cast<unsigned char>(a));
  }
  
  enum class RuntimeStatus : unsigned char
  {
    Default,
    Halted
  };
  
  using common::MemBlock;
  using common::Instruction;
  
  class ConsoleRuntime : public Runtime
  {
  private:
    int exitCode{0};
    int lastComparison{0};
    RuntimeStatus status{RuntimeStatus::Default};
    RuntimeFlags flags{RuntimeFlags::Default};
    
    std::unique_ptr<MemBlock> ram;
    std::unique_ptr<MemBlock> data;
    std::unique_ptr<MemBlock> code;
    
    std::unordered_map<unsigned short, std::unique_ptr<InputDevice>> inputDevices;
    std::unordered_map<unsigned short, std::unique_ptr<OutputDevice>> outputDevices;
    
    std::unordered_map<Instruction, std::string> operationTable;
    
    void setStatus(RuntimeStatus nextStatus);
    
    void traceMessage(const std::string &message);
    
    void debugMessage(const std::string &message);
  
  public:
    ConsoleRuntime() = default;
    
    void setFlag(RuntimeFlags flag);
    
    void clearFlag(RuntimeFlags flag);
    
    void setFlags(RuntimeFlags nextFlags);
    
    void dumpRAM() const;
    
    void dumpDataBlock() const;
    
    void dumpCodeBlock() const;
    
    [[nodiscard]] int getExitCode() const
    {
      return exitCode;
    }
    
    [[nodiscard]] RuntimeStatus getStatus() const
    { return status; }
    
    void initialize();
    
    void load(const std::string &fileName);
    
    bool running();
    
    void startup() override;
    
    void step() override;
    
    void shutdown() override;
    
    unsigned char readU8FromDataBlock(unsigned long offset) override;
    
    unsigned char readU8FromRAM(unsigned long offset) override;
    
    void writeU8ToRAM(unsigned char value, unsigned long offset) override;
  };
}

#endif //VC23_CONSOLERUNTIME_H
