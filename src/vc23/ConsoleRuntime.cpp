//
// Created by Richard Marks on 2/26/23.
//

#include <iostream>
#include <iomanip>
#include <sstream>

#include <utility>
#include <vector>
#include <functional>
#include <memory>
#include <bitset>

#include "ConsoleRuntime.h"
#include "CartReader.h"

#include <cstdint>

template<typename T>
std::string hexdump(T value)
{
  std::ostringstream os;
  auto *bytes = reinterpret_cast<uint8_t *>(&value);
  
  std::size_t num_bytes = sizeof(T);
  
  os << std::setfill('0') << std::hex << std::uppercase;
  
  for (std::size_t i = 0; i < num_bytes; ++i)
  {
    os << std::setw(2) << static_cast<int>(bytes[i]) << " ";
  }
  
  os << std::dec;
  
  return os.str();
}

using namespace vc23;
using namespace common;

class MemoryCopyInputDevice : public StandardInputDevice
{
private:
  std::function<unsigned char()> readData;
  std::function<void(unsigned char)> writeData;

public:
  ~MemoryCopyInputDevice() override = default;
  
  void setRWFns(std::function<unsigned char()> readDataFn,
                std::function<void(unsigned char)> writeDataFn)
  {
    readData = std::move(readDataFn);
    writeData = std::move(writeDataFn);
  }
  
  [[nodiscard]] unsigned short getID() const override
  {
    return 1;
  }
  
  [[nodiscard]] std::string getName() const override
  {
    return "MEMORY COPY";
  }
  
  unsigned char readU8() override
  {
    auto value = readData();
    writeData(value);
    return value;
  }
};

void ConsoleRuntime::setStatus(RuntimeStatus nextStatus)
{
  status = nextStatus;
}

void ConsoleRuntime::initialize()
{
  ram = std::make_unique<MemBlock>(constants::RUNTIME_RAM_SIZE);
  data = std::make_unique<MemBlock>();
  code = std::make_unique<MemBlock>();
  
  ram->name = "RAM";
  data->name = "DATA";
  code->name = "CODE";
  
  auto stdInDev = std::make_unique<StandardInputDevice>();
  inputDevices.emplace(stdInDev->getID(), std::move(stdInDev));
  
  auto memCpyDev = std::make_unique<MemoryCopyInputDevice>();
  memCpyDev->setRWFns([&]()
                      {
                        return readU8FromDataBlock(data->tell());
                      }, [&](unsigned char value)
                      {
                        writeU8ToRAM(value, ram->tell());
                      });
  inputDevices.emplace(memCpyDev->getID(), std::move(memCpyDev));
  
  auto stdOutDev = std::make_unique<StandardOutputDevice>();
  outputDevices.emplace(stdOutDev->getID(), std::move(stdOutDev));
  
  auto numOps = sizeof(OPERATION_NAMES) / sizeof(const char *);
  for (auto i = 0; i < numOps; i++)
  {
    auto instruction = static_cast<Instruction>(i);
    auto name = OPERATION_NAMES[i];
    operationTable.emplace(instruction, name);
  }

#define VC23REGISTEROPERATIONMETHOD(ins, body) \
  registerOperationMethod( \
  operationTable.at((ins)), \
  [&](const std::vector<unsigned long>& operands)body)
  
  VC23REGISTEROPERATIONMETHOD(Instruction::Nop, {
    // do nothing
  });
  
  // ASSIGNMENT
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AssignDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AssignHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(literal, address);
    
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AssignRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(sourceAddress), targetAddress);
  });
  
  // ADDITION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AddDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(literal + readU8FromRAM(address), address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AddHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(literal + readU8FromRAM(address), address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AddRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto sumValue = readU8FromRAM(sourceAddress) + readU8FromRAM(targetAddress);
    writeU8ToRAM(sumValue, targetAddress);
    
  });
  
  // SUBTRACTION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SubDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    auto literald = static_cast<double>(literal);
    auto diff = static_cast<double>(readU8FromRAM(address)) - literald;
    if (diff < 0)
    {
      diff = 0;
    }
    auto diffu8 = static_cast<unsigned char>(diff);
    
    writeU8ToRAM(diffu8, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SubHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) - literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SubRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto diffValue = readU8FromRAM(targetAddress) - readU8FromRAM(sourceAddress);
    writeU8ToRAM(diffValue, targetAddress);
  });
  
  // MULTIPLICATION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::MulDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) * literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::MulHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) * literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::MulRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto productValue = readU8FromRAM(targetAddress) * readU8FromRAM(sourceAddress);
    writeU8ToRAM(productValue, targetAddress);
  });
  
  // DIVISION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::DivDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) / literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::DivHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) / literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::DivRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto quotientValue = readU8FromRAM(targetAddress) / readU8FromRAM(sourceAddress);
    writeU8ToRAM(quotientValue, targetAddress);
  });
  
  // NEGATION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::NegRef, {
    auto address = operands.at(0);
    
    writeU8ToRAM(readU8FromRAM(address) * -1, address);
  });
  
  // LOGICAL
  
  VC23REGISTEROPERATIONMETHOD(Instruction::XorDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) ^ literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::XorHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) ^ literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::XorRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto value = readU8FromRAM(targetAddress) ^ readU8FromRAM(sourceAddress);
    writeU8ToRAM(value, targetAddress);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AndDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) & literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AndHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) & literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AndRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto value = readU8FromRAM(targetAddress) & readU8FromRAM(sourceAddress);
    writeU8ToRAM(value, targetAddress);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::OrDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) | literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::OrHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) | literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::OrRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto value = readU8FromRAM(targetAddress) | readU8FromRAM(sourceAddress);
    writeU8ToRAM(value, targetAddress);
  });
  
  // BIT SHIFTING
  
  VC23REGISTEROPERATIONMETHOD(Instruction::LeftShiftDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) << literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::LeftShiftHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) << literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RightShiftDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) >> literal, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RightShiftHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    writeU8ToRAM(readU8FromRAM(address) >> literal, address);
  });
  
  // COMPARISON
  
  VC23REGISTEROPERATIONMETHOD(Instruction::CmpDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    auto value = readU8FromRAM(address);
    
    if (value == literal)
    {
      lastComparison = 0;
    } else if (literal < value)
    {
      lastComparison = -1;
    } else if (literal > value)
    {
      lastComparison = 1;
    }
    
  });
  VC23REGISTEROPERATIONMETHOD(Instruction::CmpHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    auto value = readU8FromRAM(address);
    
    if (value == literal)
    {
      lastComparison = 0;
    } else if (literal < value)
    {
      lastComparison = -1;
    } else if (literal > value)
    {
      lastComparison = 1;
    }
    
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::CmpRefRef, {
    
    auto address1 = operands.at(0);
    auto address2 = operands.at(1);
    
    auto value1 = readU8FromRAM(address1);
    auto value2 = readU8FromRAM(address2);
    
    if (value1 == value2)
    {
      lastComparison = 0;
    } else if (value1 < value2)
    {
      lastComparison = -1;
    } else if (value1 > value2)
    {
      lastComparison = 1;
    }
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::CmpRef, {
    auto address = operands.at(0);
    
    auto value = readU8FromRAM(address);
    
    if (value == 0)
    {
      lastComparison = 0;
    } else if (value < 0)
    {
      lastComparison = -1;
    } else if (value > 0)
    {
      lastComparison = 1;
    }
  });
  
  // EXECUTION PATH MANIPULATION
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AbsJumpDecLit, {
    auto literal = operands.at(0);
    
    traceJump(true, code->tell(), literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AbsJumpHexLit, {
    auto literal = operands.at(0);
    
    traceJump(true, code->tell(), literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AbsJumpRef, {
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
    traceJump(true, code->tell(), offset);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(offset);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackDecLit, {
    auto literal = operands.at(0);
    
    traceJump(false, code->tell(), code->tell() - literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() - literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackHexLit, {
    auto literal = operands.at(0);
    
    traceJump(false, code->tell(), code->tell() - literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() - literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackRef, {
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
    traceJump(false, code->tell(), code->tell() - offset);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() - offset);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardDecLit, {
    auto literal = operands.at(0);
    
    traceJump(false, code->tell(), code->tell() + literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() + literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardHexLit, {
    auto literal = operands.at(0);
    
    traceJump(false, code->tell(), code->tell() + literal);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() + literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardRef, {
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
    traceJump(false, code->tell(), code->tell() + offset);
    
    if (lastComparison != 0)
    {
      return;
    }
    
    code->seek(code->tell() + offset);
  });
  
  // MEMORY SELECT
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRamDefault, {
    ram->seek(0);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRamDecLit, {
    auto literal = operands.at(0);
    ram->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRamHexLit, {
    auto literal = operands.at(0);
    ram->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRomDefault, {
    data->seek(0);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRomDecLit, {
    auto literal = operands.at(0);
    data->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::SelectRomHexLit, {
    auto literal = operands.at(0);
    data->seek(literal);
  });
  
  // INPUT DEVICES
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceRef, {
    auto deviceIndex = operands.at(0);
    auto address = operands.at(1);
    
    auto &device = inputDevices.at(deviceIndex);
    auto value = device->readU8();
    writeU8ToRAM(value, address);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceDecLitRef, {
    auto literal = operands.at(0);
    auto deviceIndex = operands.at(1);
    auto address = operands.at(2);
    
    auto &device = inputDevices.at(deviceIndex);
    for (auto k = 0; k < literal; k++)
    {
      auto value = device->readU8();
      writeU8ToRAM(value, address + k);
    }
    
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceHexLitRef, {
    auto literal = operands.at(0);
    auto deviceIndex = operands.at(1);
    auto address = operands.at(2);
    
    auto &device = inputDevices.at(deviceIndex);
    for (auto k = 0; k < literal; k++)
    {
      auto value = device->readU8();
      writeU8ToRAM(value, address + k);
    }
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceRefRef, {
    auto sourceAddress = operands.at(0);
    auto deviceIndex = operands.at(1);
    auto targetAddress = operands.at(2);
    
    auto &device = inputDevices.at(deviceIndex);
    auto n = readU8FromRAM(sourceAddress);
    for (auto k = 0; k < n; k++)
    {
      ram->seek(targetAddress + k);
      auto value = device->readU8();
      writeU8ToRAM(value, targetAddress + k);
    }

//    // TODO: implement devices properly
//    if (deviceIndex == 1)
//    {
//      // memory copy device
//      auto n = readU8FromRAM(sourceAddress);
//      for (auto k = 0; k < n; k++)
//      {
//        auto byt = readU8FromDataBlock(k);
//        writeU8ToRAM(byt, targetAddress + k);
//      }
//    }
  });
  
  // OUTPUT DEVICES
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceDecLit, {
    auto literal = operands.at(0);
    auto deviceIndex = operands.at(1);
    
    auto &device = outputDevices.at(deviceIndex);
    device->writeU8(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceHexLit, {
    auto literal = operands.at(0);
    auto deviceIndex = operands.at(1);
    
    auto &device = outputDevices.at(deviceIndex);
    device->writeU8(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceRef, {
    // o @3 {0}
    auto address = operands.at(0);
    auto deviceIndex = operands.at(1);
    auto value = readU8FromRAM(address);
    
    auto &device = outputDevices.at(deviceIndex);
    device->writeU8(value);
  });
  
  // PRINTING STRINGS
  
  VC23REGISTEROPERATIONMETHOD(Instruction::PrintDecLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    for (auto k = 0; k < literal; k++)
    {
      auto ch = readU8FromRAM(address + k);
      std::cout << ch;
    }
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::PrintHexLitRef, {
    auto literal = operands.at(0);
    auto address = operands.at(1);
    
    for (auto k = 0; k < literal; k++)
    {
      auto ch = readU8FromRAM(address + k);
      std::cout << ch;
    }
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::PrintRefRef, {
    auto sourceAddress = operands.at(0);
    auto targetAddress = operands.at(1);
    
    auto n = readU8FromRAM(sourceAddress);
    
    for (auto k = 0; k < n; k++)
    {
      auto ch = readU8FromRAM(targetAddress + k);
      std::cout << ch;
    }
  });
  
  // END PROGRAM
  
  VC23REGISTEROPERATIONMETHOD(Instruction::EndWithDecLitExitCode, {
    exitCode = operands.at(0);
    setStatus(RuntimeStatus::Halted);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::EndWithHexLitExitCode, {
    exitCode = operands.at(0);
    setStatus(RuntimeStatus::Halted);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::EndWithSuccess, {
    exitCode = 0;
    setStatus(RuntimeStatus::Halted);
  });


#undef VC23REGISTEROPERATIONMETHOD

}

bool ConsoleRuntime::running()
{
  return status != RuntimeStatus::Halted && !code->eom();
}

void ConsoleRuntime::dumpRAM() const
{
  std::cout << "RAM BLOCK" << std::endl;
  ram->hd();
}

void ConsoleRuntime::dumpDataBlock() const
{
  std::cout << "DATA BLOCK" << std::endl;
  data->hd();
}

void ConsoleRuntime::dumpCodeBlock() const
{
  std::cout << "CODE BLOCK" << std::endl;
  code->hd();
}

void ConsoleRuntime::startup()
{
  ram->seek(0);
  data->seek(0);
  code->seek(0);
}

void ConsoleRuntime::step()
{
  if (execLimit != 0)
  {
    if (++steps >= execLimit)
    {
      code->seek(code->size());
      return;
    }
  }
  
  if (
    ((flags & RuntimeFlags::Trace) == RuntimeFlags::Trace) &&
    ((flags & RuntimeFlags::TraceDump) == RuntimeFlags::TraceDump))
  {
    std::cout << "BEFORE STEP " << std::dec << steps << ":" << std::endl;
    dumpRAM();
  }
  // read the operation code
  unsigned short opcode = code->readU16();
  
  // read the number of operands
  unsigned char numOperands = code->readU8();
  
  // read each operand into a single vector for easy access
  std::vector<unsigned long> operandValues;
  
  unsigned short operationTotalSize = sizeof(unsigned short) + sizeof(unsigned char);
  
  for (unsigned i = 0; i < numOperands; i++)
  {
    // read each operand
    unsigned char opSize = code->readU8();
    operationTotalSize += opSize;
    if (opSize == 1)
    {
      unsigned char opValue = code->readU8();
      operandValues.push_back(opValue);
    } else if (opSize == 2)
    {
      unsigned short opValue = code->readU16();
      operandValues.push_back(opValue);
    } else if (opSize == 4)
    {
      unsigned int opValue = code->readU32();
      operandValues.push_back(opValue);
    } else if (opSize == 8)
    {
      unsigned long opValue = code->readU64();
      operandValues.push_back(opValue);
    } else
    {
      throw std::runtime_error("INVALID OPERAND SIZE " + std::to_string(opSize));
    }
  }
  
  // cast the operation code to the instruction enum
  auto instruction = static_cast<Instruction>(opcode);
  
  // process the instruction
  if (operationTable.count(instruction))
  {
    auto operationName = operationTable.at(instruction);
    
    if (hasOperationMethod(operationName))
    {
      
      std::ostringstream ss;
      ss << "*EXECUTE AT 0x"
         << std::setfill('0')
         << std::setw(4)
         << std::hex << code->tell()
         << " / 0x"
         << std::setfill('0')
         << std::setw(4)
         << std::hex << code->size()
         << std::dec
         << " "
         << operationName << "(";
      auto k = 0;
      for (auto &val: operandValues)
      {
        ss << std::to_string(val);
        if (k < operandValues.size() - 1)
        {
          ss << ", ";
        }
        k++;
      }
      ss << ")";
      traceMessage(ss.str());
      
      if ((flags & RuntimeFlags::ShowOpSize) == RuntimeFlags::ShowOpSize)
      {
        std::cout << "*            OPERATION: " << operationName << std::endl;
        std::cout << "*    INSTRUCTION BYTES: " << std::dec << sizeof(unsigned short) + sizeof(unsigned char)
                  << std::endl;
        std::cout << "*        OPERAND BYTES: " << std::dec
                  << operationTotalSize - (sizeof(unsigned short) + sizeof(unsigned char)) << std::endl;
        std::cout << "*TOTAL OPERATION BYTES: " << std::dec << operationTotalSize << std::endl;
      }
      
      if ((flags & RuntimeFlags::NoExec) == RuntimeFlags::NoExec)
      {
        std::cout << "*EXECUTION SKIPPED BECAUSE NOEXEC FLAG ENABLED" << std::endl;
      } else
      {
        executeOperationMethod(operationName, operandValues);
      }
    } else
    {
      std::ostringstream ss;
      ss << "Unregistered Runtime Operation Method \""
         << operationName << "\"" << std::endl;
      throw std::runtime_error(ss.str());
    }
  } else
  {
    std::ostringstream ss;
    ss << "Instruction has no Runtime Operation: "
       << std::hex << static_cast<unsigned short>(instruction)
       << std::dec << std::endl;
    throw std::runtime_error(ss.str());
  }
  
  if (
    ((flags & RuntimeFlags::Trace) == RuntimeFlags::Trace) &&
    ((flags & RuntimeFlags::TraceDump) == RuntimeFlags::TraceDump))
  {
    std::cout << "AFTER STEP " << std::dec << steps << ":" << std::endl;
    dumpRAM();
  }
}

void ConsoleRuntime::shutdown()
{

}

unsigned char ConsoleRuntime::readU8FromDataBlock(unsigned long offset)
{
  data->seek(offset);
  auto value = data->readU8();
  traceMessage("*READ FROM ROM ADDRESS " + std::to_string(offset) + ": " + std::to_string(value));
  return value;
}

unsigned char ConsoleRuntime::readU8FromRAM(unsigned long offset)
{
  ram->seek(offset);
  auto value = ram->readU8();
  traceMessage("*READ FROM RAM ADDRESS " + std::to_string(offset) + ": " + std::to_string(value));
  return value;
}

void ConsoleRuntime::writeU8ToRAM(unsigned char value, unsigned long offset)
{
  traceMessage("*WRITE TO RAM ADDRESS " + std::to_string(offset) + ": " + std::to_string(value));
  ram->seek(offset);
  ram->writeU8(value);
}

void ConsoleRuntime::load(const std::string &filePath)
{
  size_t dotPosition = filePath.find_last_of('.');
  std::string fileExt;
  if (dotPosition != std::string::npos)
  {
    fileExt = filePath.substr(dotPosition + 1);
  }
  
  if (fileExt.empty())
  {
    throw std::runtime_error("No file extension in: " + filePath);
  }
  
  if (fileExt == "cart")
  {
    // load a compiled cart
    CartReader reader;
    reader.readFromFile(filePath);
    if (reader.isValid())
    {
      auto dataBytes = reader.getData();
      auto codeBytes = reader.getCode();
      
      data->initialize(dataBytes.size());
      code->initialize(codeBytes.size());
      
      for (auto &byt: dataBytes)
      {
        data->writeU8(byt);
      }
      
      for (auto &byt: codeBytes)
      {
        code->writeU8(byt);
      }
    } else
    {
      throw std::runtime_error("cart invalid?");
    }
  } else if (fileExt == "vcc")
  {
    // FUTURE:
    // load a vcc source, parse, and compile into data and code mem blocks.
    throw std::runtime_error("Loading vcc files is not supported at this time. ");
  } else
  {
    throw std::runtime_error("Unknown file extension: " + fileExt);
  }
  
  startup();
}

bool ConsoleRuntime::hasFlag(RuntimeFlags flag)
{
  return ((flags & flag) == flag);
}

void ConsoleRuntime::setFlag(RuntimeFlags flag)
{
  flags = flags | flag;
  if (RuntimeFlags::Debug == (flags & RuntimeFlags::Debug))
  {
    ram->debug = true;
    code->debug = true;
    data->debug = true;
  } else
  {
    ram->debug = false;
    code->debug = false;
    data->debug = false;
  }
}

void ConsoleRuntime::clearFlag(RuntimeFlags flag)
{
  flags = flags & ~flag;
}

void ConsoleRuntime::setFlags(RuntimeFlags nextFlags)
{
  flags = nextFlags;
}

void ConsoleRuntime::traceMessage(const std::string &message)
{
  if ((flags & RuntimeFlags::Trace) == RuntimeFlags::Trace)
  {
    std::cout << "TRACE: " << message << std::endl;
  }
}

void ConsoleRuntime::debugMessage(const std::string &message)
{
  if ((flags & RuntimeFlags::Debug) == RuntimeFlags::Debug)
  {
    std::cout << "DEBUG: " << message << std::endl;
  }
}


void ConsoleRuntime::traceJump(bool absolute, unsigned long origin, unsigned long target) const
{
  if ((flags & RuntimeFlags::TraceJump) != RuntimeFlags::TraceJump)
  {
    return;
  }
  
  std::cout << "* LAST COMPARISON: " << std::dec << (int) lastComparison << std::endl;
  
  if (lastComparison != 0)
  {
    std::cout << "* NO JUMP" << std::endl;
    return;
  }
  
  if (absolute)
  {
    std::cout << "* ABSOLUTE JUMP FROM "
              << std::dec << origin << "(" << hexdump<unsigned short>(origin)
              << ") TO "
              << std::dec << target << "(" << hexdump<unsigned short>(target) << ")"
              << std::endl;
  } else
  {
    double diff = static_cast<double>(target) - static_cast<double>(origin);
    std::cout << "* RELATIVE JUMP "
              << std::dec << (diff >= 0 ? "+" : "") << static_cast<long>(diff)
              << " FROM "
              << std::dec << origin << "(" << hexdump<unsigned short>(origin)
              << ") TO "
              << std::dec << target << "(" << hexdump<unsigned short>(target) << ")"
              << std::endl;
  }
  std::cout << "* BYTE AT ORIGIN: " << hexdump<unsigned char>(code->peekAtU8(origin)) << std::endl;
  std::cout << "* BYTE AT TARGET: " << hexdump<unsigned char>(code->peekAtU8(target)) << std::endl;
}

void ConsoleRuntime::describeCode()
{
  std::cout << "DESCRIBING CODE BLOCK" << std::endl;
  code->hd();
  code->seek(0);
  std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
  
  while (!code->eom())
  {
    std::ostringstream os;
    
    os << std::setw(8) << std::setfill('0') << std::hex << code->tell() << ": " << std::dec;
    
    // read the operation code
    unsigned short opcode = code->readU16();
    
    // cast the operation code to the instruction enum
    auto instruction = static_cast<Instruction>(opcode);
    
    if (operationTable.count(instruction))
    {
      auto operationName = operationTable.at(instruction);
      if (hasOperationMethod(operationName))
      {
        os << std::setfill('\0') << std::setw(32) << hexdump<unsigned short>(opcode) << " ;" << operationName
           << std::endl;
      }
    } else
    {
      os << std::setfill('\0') << std::setw(32) << hexdump<unsigned short>(opcode) << " ; UNKNOWN INSTRUCTION"
         << std::endl;
    }
    
    
    os << std::setw(8) << std::setfill('0') << std::hex << code->tell() << ": " << std::dec;
    
    
    // read the number of operands
    unsigned char numOperands = code->readU8();
    os << std::setfill('\0') << std::setw(32) << hexdump<unsigned char>(numOperands) << " ; number of operands"
       << std::endl;
    
    for (unsigned i = 0; i < numOperands; i++)
    {
      
      os << std::setw(8) << std::setfill('0') << std::hex << code->tell() << ": " << std::dec;
      
      // read each operand
      unsigned char opSize = code->readU8();
      os << std::setfill('\0') << std::setw(32) << hexdump<unsigned char>(numOperands) << " ; operand size in bytes"
         << std::endl;
      
      
      os << std::setw(8) << std::setfill('0') << std::hex << code->tell() << ": " << std::dec;
      
      if (opSize == 1)
      {
        unsigned char opValue = code->readU8();
        os << std::setfill('\0') << std::setw(32) << hexdump<unsigned char>(opValue) << " ; operand value decimal: "
           << std::dec
           << ((long) opValue) << std::endl;
      } else if (opSize == 2)
      {
        unsigned short opValue = code->readU16();
        os << std::setfill('\0') << std::setw(32) << hexdump<unsigned short>(opValue) << " ; operand value decimal: "
           << std::dec
           << ((long) opValue) << std::endl;
      } else if (opSize == 4)
      {
        unsigned int opValue = code->readU32();
        os << std::setfill('\0') << std::setw(32) << hexdump<unsigned int>(opValue) << " ; operand value decimal: "
           << std::dec
           << ((long) opValue) << std::endl;
      } else if (opSize == 8)
      {
        unsigned long opValue = code->readU64();
        os << std::setfill('\0') << std::setw(32) << hexdump<unsigned long>(opValue) << " ; operand value decimal: "
           << std::dec
           << ((long) opValue) << std::endl;
      } else
      {
        throw std::runtime_error("INVALID OPERAND SIZE " + std::to_string(opSize));
      }
    }
    std::cout << os.str();
  }
  
  std::cout << std::setfill('-') << std::setw(40) << "-" << std::endl;
  
  code->seek(0);
}

void ConsoleRuntime::setExecLimit(int limit)
{
  execLimit = static_cast<unsigned long>(limit);
}
