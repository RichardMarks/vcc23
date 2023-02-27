//
// Created by Richard Marks on 2/26/23.
//

#include <iostream>
#include <iomanip>
#include <sstream>

#include <vector>

#include "ConsoleRuntime.h"
#include "CartReader.h"


using namespace vc23;
using namespace common;

void ConsoleRuntime::setStatus(RuntimeStatus nextStatus)
{
  status = nextStatus;
}

void ConsoleRuntime::initialize()
{
  ram = std::make_unique<MemBlock>(constants::RUNTIME_RAM_SIZE);
  data = std::make_unique<MemBlock>();
  code = std::make_unique<MemBlock>();
  
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
    
    writeU8ToRAM(readU8FromRAM(address) - literal, address);
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
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AbsJumpHexLit, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::AbsJumpRef, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
    code->seek(offset);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackDecLit, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(code->tell() - literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackHexLit, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(code->tell() - literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpBackRef, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
    code->seek(code->tell() - offset);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardDecLit, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(code->tell() + literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardHexLit, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto literal = operands.at(0);
    
    code->seek(code->tell() + literal);
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::RelJumpForwardRef, {
    if (lastComparison != 0)
    {
      return;
    }
    
    auto address = operands.at(0);
    auto offset = readU8FromRAM(address);
    
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
    // TODO: implementation
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceDecLitRef, {
    // TODO: implementation
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceHexLitRef, {
    // TODO: implementation
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::ReadInputDeviceRefRef, {
    auto sourceAddress = operands.at(0);
    auto deviceIndex = operands.at(1);
    auto targetAddress = operands.at(2);
    
    // TODO: implement devices properly
    if (deviceIndex == 1)
    {
      // memory copy device
      auto n = readU8FromRAM(sourceAddress);
      for (auto k = 0; k < n; k++)
      {
        auto byt = readU8FromDataBlock(k);
        writeU8ToRAM(byt, targetAddress + k);
      }
    }
  });
  
  // OUTPUT DEVICES
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceDecLit, {
    // TODO: implementation
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceHexLit, {
    // TODO: implementation
  });
  
  VC23REGISTEROPERATIONMETHOD(Instruction::WriteOutputDeviceRef, {
    // TODO: implementation
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
  if (
    ((flags & RuntimeFlags::Trace) == RuntimeFlags::Trace) &&
    ((flags & RuntimeFlags::TraceDump) == RuntimeFlags::TraceDump))
  {
    std::cout << "BEFORE STEP:" << std::endl;
    dumpRAM();
  }
  // read the operation code
  unsigned short opcode = code->readU16();
  
  // read the number of operands
  unsigned char numOperands = code->readU8();
  
  // read each operand into a single vector for easy access
  std::vector<unsigned long> operandValues;
  
  for (unsigned i = 0; i < numOperands; i++)
  {
    // read each operand
    unsigned char opSize = code->readU8();
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
      executeOperationMethod(operationName, operandValues);
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
    std::cout << "AFTER STEP:" << std::endl;
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

void ConsoleRuntime::setFlag(RuntimeFlags flag)
{
  flags = flags | flag;
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