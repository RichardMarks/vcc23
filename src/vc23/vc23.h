//
// Created by Richard Marks on 2/22/23.
//

#ifndef VC23_VC23_H
#define VC23_VC23_H

#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <fstream>
#include <iomanip>
#include <functional>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <unordered_set>

#include "Syntax.h"
#include "MemBlock.h"


namespace common
{
  struct cart_header_t
  {
    char fourcc[4];
    char name[33];
    [[maybe_unused]] unsigned long reserved[16];
    unsigned long dataLength;
    unsigned long codeLength;
  };
  
  struct cart_blocks_t
  {
    unsigned char *data;
    unsigned char *code;
  };
  
  struct cart_t
  {
    struct cart_header_t header;
    struct cart_blocks_t blocks;
  };
}

namespace vc23
{
  using common::MemBlock;
  
  class Cart
  {
  private:
    bool loaded;
    struct common::cart_t cart{};
    
    MemBlock dataBlock;
    MemBlock codeBlock;
  
  public:
    MemBlock &data()
    { return dataBlock; }
    
    MemBlock &code()
    { return codeBlock; }
    
    Cart()
    {
      cart.header.codeLength = 0;
      cart.header.dataLength = 0;
      cart.blocks.data = nullptr;
      cart.blocks.code = nullptr;
      loaded = false;
    }
    
    ~Cart()
    {
      if (cart.blocks.data != nullptr)
      {
        std::cout << "~ Free Data" << std::endl;
        delete[] cart.blocks.data;
      }
      
      if (cart.blocks.code != nullptr)
      {
        std::cout << "~ Free Code" << std::endl;
        delete[] cart.blocks.code;
      }
    }
    
    void load(const std::string &fileName)
    {
      std::cout << "Loading Cart: " << fileName << std::endl;
      
      loaded = false;
      std::cout << "Cart::loadFromFile(\"" << fileName << "\");" << std::endl;
      
      std::ifstream fp(fileName, std::ios::binary);
      
      if (!fp.is_open())
      {
        throw std::runtime_error("Unable to load - open file failed.");
      }
      
      std::cout << "Loading Header..." << std::endl;
      
      fp.read(reinterpret_cast<char *>(&cart.header.fourcc), sizeof(char) * 4);
      
      if (
        cart.header.fourcc[0] != 'v' ||
        cart.header.fourcc[1] != 'c' ||
        cart.header.fourcc[2] != '2' ||
        cart.header.fourcc[3] != '3')
      {
        fp.close();
        throw std::runtime_error("Unable to load - Invalid vc23 cart file.");
      }
      
      fp.read(reinterpret_cast<char *>(&cart.header.name), sizeof(char) * 33);
      // skip reserved block
      fp.seekg(16 * sizeof(unsigned long), std::ios_base::cur);
      fp.read(reinterpret_cast<char *>(&cart.header.dataLength), sizeof(unsigned long));
      fp.read(reinterpret_cast<char *>(&cart.header.codeLength), sizeof(unsigned long));
      
      std::cout << "vc23 cart file: " << cart.header.name << std::endl;
      std::cout << std::setw(16) << std::to_string(cart.header.dataLength) << " (0x" << std::setfill('0')
                << std::setw(sizeof(unsigned long) * 2) << std::hex << cart.header.dataLength << ") bytes in data."
                << std::endl;
      std::cout << std::setw(16) << std::to_string(cart.header.codeLength) << " (0x" << std::setfill('0')
                << std::setw(sizeof(unsigned long) * 2) << std::hex << cart.header.codeLength << ") bytes in code."
                << std::endl;
      
      std::cout << "Loading Data...";
      cart.blocks.data = new unsigned char[cart.header.dataLength];
      
      fp.read(reinterpret_cast<char *>(cart.blocks.data),
              static_cast<std::streamsize>(sizeof(unsigned char) * cart.header.dataLength));
      std::cout << "OK." << std::endl;
      
      std::cout << "Loading Code...";
      cart.blocks.code = new unsigned char[cart.header.codeLength];
      fp.read(reinterpret_cast<char *>(cart.blocks.code),
              static_cast<std::streamsize>(sizeof(unsigned char) * cart.header.codeLength));
      std::cout << "OK." << std::endl;
      
      // copy data block
      dataBlock.initialize(cart.header.dataLength);
      for (unsigned long i = 0; i < cart.header.dataLength; i++)
      {
        dataBlock.writeU8(cart.blocks.data[i]);
      }
      dataBlock.seek(0);
      
      // copy code block
      codeBlock.initialize(cart.header.codeLength);
      for (unsigned long i = 0; i < cart.header.codeLength; i++)
      {
        codeBlock.writeU8(cart.blocks.code[i]);
      }
      codeBlock.seek(0);
      
      fp.close();
      loaded = true;
      
      // test
      unsigned datRows = cart.header.dataLength / 8;
      for (unsigned i = 0; i < datRows; i++)
      {
        std::cout << "DATA ";
        for (unsigned j = 0; j < 8; j++)
        {
          std::cout << std::setw(2) << std::hex << (unsigned) cart.blocks.data[j + i * 8] << " ";
        }
        std::cout << std::dec << std::endl;
      }
      
      for (unsigned i = 0; i < cart.header.codeLength; i++)
      {
        std::cout << std::setw(2) << std::hex << (unsigned) cart.blocks.code[i] << " ";
      }
      std::cout << std::dec << std::endl;
    }
    
    [[nodiscard]] bool isLoaded() const
    {
      return loaded;
    }
  };
  
  enum class CpuFlags : unsigned char
  {
    Default = 0,
    EnableDebug = 1,
    EnableStdoutDevice = 2
  };
  
  CpuFlags operator|(CpuFlags a, CpuFlags b)
  {
    return (CpuFlags) ((unsigned char) a | (unsigned char) b);
  }
  
  class CPU
  {
  private:
    std::vector<unsigned char> ram;
    CpuFlags cpuFlags;
    Cart *cartPtr{};
    bool halted;
    [[maybe_unused]] unsigned char exitCode{};
  
  public:
    CPU()
    {
      cpuFlags = CpuFlags::Default;
      // zero out ram
      ram.reserve(8192);
      for (unsigned i = 0; i < 8192; i++)
      {
        ram[i] = 0;
      }
      
      halted = false;
    }
    
    void setFlags(CpuFlags flags)
    {
      cpuFlags = flags;
    }
    
    bool running()
    {
      if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
      {
        std::cout << "DEBUG: halted: "
                  << (halted ? "T" : "F") << " eom: "
                  << (cartPtr->code().eom() ? "T" : "F")
                  << std::endl;
      }
      return !halted && !cartPtr->code().eom();
    }
    
    void boot(const Cart &cart)
    {
      // if cart is loaded
      if (!cart.isLoaded())
      {
        throw std::runtime_error("Cart is Not Loaded!");
      }
      
      cartPtr = const_cast<Cart *>(&cart);
      
      std::cout << "cart data size: " << cartPtr->data().size() << " bytes" << std::endl;
      std::cout << "cart code size: " << cartPtr->code().size() << " bytes" << std::endl;
      
      cartPtr->code().hd();
    }
    
    void step()
    {
      // read an instruction from the code block
      MemBlock &code = cartPtr->code();
      MemBlock &data = cartPtr->data();
      unsigned short opcode = code.readU16();
      
      if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
      {
        std::cout << "DEBUG: opcode: " << std::hex << (int) opcode << std::dec << std::endl;
      }
      
      // read the number of operands
      unsigned char numOperands = code.readU8();
      
      if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
      {
        std::cout << "DEBUG: operand count: " << (int) numOperands << std::endl;
      }
      
      std::vector<unsigned long> operandValues;
      
      for (unsigned i = 0; i < numOperands; i++)
      {
        // read each operand
        unsigned char opSize = code.readU8();
        if (opSize == 1)
        {
          unsigned char opValue = code.readU8();
          operandValues.push_back(opValue);
        } else if (opSize == 2)
        {
          unsigned short opValue = code.readU16();
          operandValues.push_back(opValue);
        } else if (opSize == 4)
        {
          unsigned int opValue = code.readU32();
          operandValues.push_back(opValue);
        } else if (opSize == 8)
        {
          unsigned long opValue = code.readU64();
          operandValues.push_back(opValue);
        } else
        {
          throw std::runtime_error("INVALID OPERAND SIZE " + std::to_string(opSize));
        }
      }
      
      if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
      {
        std::cout << "DEBUG: operand values: " << std::endl;
        for (unsigned k = 0; k < operandValues.size(); k++)
        {
          std::cout << "[" << k << "] " << operandValues.at(0) << std::endl;
        }
      }
      
      // process the instruction
      switch (opcode)
      {
        case 0x2:
        {
          if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
          {
            std::cout << "DEBUG: AssignDecLitRef " << std::endl;
          }
          // assign dec lit ref
          unsigned long literal = operandValues.at(0);
          unsigned long address = operandValues.at(1);
          
          ram[address] = static_cast<unsigned char>(literal);
        }
          break;
        case 0x35:
        {
          if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
          {
            std::cout << "DEBUG: ReadInputDeviceRefRef " << std::endl;
          }
          // read input device ref ref
          unsigned long sourceAddress = operandValues.at(0);
          unsigned long deviceIndex = operandValues.at(1);
          unsigned long targetAddress = operandValues.at(2);
          if (deviceIndex == 1)
          {
            unsigned char n = ram[sourceAddress];
            // memory copy device
            // read from data block
            // write to ram
            for (unsigned k = 0; k < n; k++)
            {
              unsigned char byt = data.readU8();
              ram[targetAddress + k] = byt;
            }
          }
        }
          break;
        case 0x3b:
        {
          if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
          {
            std::cout << "DEBUG: PrintRefRef " << std::endl;
          }
          /*
          ; read value of from memory address 3 as n
          ; print n characters from memory addresses
          ; 4+0, 4+.., 4+n-1 to output device #0
          */
          // print string ref ref
          unsigned long sourceAddress = operandValues.at(0);
          unsigned long targetAddress = operandValues.at(1);
          
          unsigned char n = ram[sourceAddress];
          for (unsigned k = 0; k < n; k++)
          {
            unsigned char ch = ram[targetAddress + k];
            std::cout << ch;
          }
        }
          break;
        case 0x3e:
        {
          if ((unsigned char) cpuFlags & (unsigned char) CpuFlags::EnableDebug)
          {
            std::cout << "DEBUG: EndWithSuccess " << std::endl;
          }
          // end program with implicit exit code 0
          halted = true;
          exitCode = 0;
        }
          break;
        default:
          break;
      }
    }
  };
}

#define VC23_SAFE_BLOCK_START \
  try                         \
  {
#define VC23_SAFE_BLOCK_END                                           \
  }                                                                   \
  catch (std::exception & ex)                                         \
  {                                                                   \
    std::cerr << "VC23 SAFE BLOCK ERROR: " << ex.what() << std::endl; \
  }

#endif //VC23_VC23_H
