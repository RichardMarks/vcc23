//
// Created by Richard Marks on 2/22/23.
//

#ifndef VC23_MEMBLOCK_H
#define VC23_MEMBLOCK_H

#include <string>

namespace common
{
  class MemBlock
  {
  private:
    unsigned char *data;
    unsigned long dataSize;
    unsigned long offset;
  
  public:
    std::string name;
    bool debug{false};
    
    MemBlock();
    
    explicit MemBlock(unsigned long maxSize);
    
    ~MemBlock();
    
    void initialize(unsigned long maxSize);
    
    [[nodiscard]] unsigned long size() const
    { return dataSize; }
    
    [[nodiscard]] unsigned long tell() const
    { return offset; }
    
    [[nodiscard]] bool eom() const
    { return offset == dataSize; }
    
    void seek(unsigned long targetOffset);
    
    void fill(unsigned long start, unsigned long end, unsigned char fillValue = 0);
    
    unsigned char peekU8();
    
    unsigned short peekU16();
    
    unsigned int peekU32();
    
    unsigned long peekU64();
    
    unsigned char peekAtU8(unsigned long targetOffset);
    
    unsigned short peekAtU16(unsigned long targetOffset);
    
    unsigned int peekAtU32(unsigned long targetOffset);
    
    unsigned long peekAtU64(unsigned long targetOffset);
    
    unsigned char readU8();
    
    unsigned short readU16();
    
    unsigned int readU32();
    
    unsigned long readU64();
    
    void writeU8(unsigned char value);
    
    void writeU16(unsigned short value);
    
    void writeU32(unsigned int value);
    
    void writeU64(unsigned long value);
    
    void hd();
    
    std::string hdstr();
  };
}

#endif //VC23_MEMBLOCK_H
