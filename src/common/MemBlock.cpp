//
// Created by Richard Marks on 2/22/23.
//

#include "MemBlock.h"

#include <iostream>
#include <iomanip>

using namespace common;

MemBlock::MemBlock()
  : data(nullptr), dataSize(0), offset(0)
{
}

MemBlock::MemBlock(unsigned long maxSize) : data(nullptr), dataSize(0), offset(0)
{
  initialize(maxSize);
}

MemBlock::~MemBlock()
{
  if (data != nullptr)
  {
    delete[] data;
    data = nullptr;
  }
}

void MemBlock::initialize(unsigned long maxSize)
{
  if (data != nullptr)
  {
    delete[] data;
    data = nullptr;
  }
  data = new unsigned char[maxSize];
  for (unsigned long i = 0; i < maxSize; i++)
  {
    data[i] = 0;
  }
  offset = 0;
  dataSize = maxSize;
}

void MemBlock::seek(unsigned long targetOffset)
{
  offset = targetOffset > dataSize ? dataSize : targetOffset;
}

void MemBlock::fill(unsigned long start, unsigned long end, unsigned char fillValue)
{
  if (start == end)
  {
    return;
  }
  
  if (start > end)
  {
    unsigned long t = start;
    start = end;
    end = t;
  }
  
  if (end > dataSize)
  {
    end = dataSize;
  }
  
  for (unsigned long i = start; i < end; i++)
  {
    data[i] = fillValue;
  }
}

unsigned char MemBlock::readU8()
{
  if (offset + sizeof(unsigned char) > dataSize)
  {
    return 0;
  }
  unsigned char val = data[offset];
  offset += sizeof(unsigned char);
  return val;
}

unsigned short MemBlock::readU16()
{
  if (offset + sizeof(unsigned short) > dataSize)
  {
    return 0;
  }
  unsigned short val = (unsigned short) data[offset] | (unsigned short) data[offset + 1] << 8;
  offset += sizeof(unsigned short);
  return val;
}

unsigned int MemBlock::readU32()
{
  if (offset + sizeof(unsigned int) > dataSize)
  {
    return 0;
  }
  unsigned int val = (unsigned int) data[offset] |
                     (unsigned int) data[offset + 1] << 8 |
                     (unsigned int) data[offset + 2] << 16 |
                     (unsigned int) data[offset + 3] << 24;
  offset += sizeof(unsigned int);
  return val;
}

unsigned long MemBlock::readU64()
{
  if (offset + sizeof(unsigned long) > dataSize)
  {
    return 0;
  }
  unsigned long val = (unsigned long) data[offset] |
                      (unsigned long) data[offset + 1] << 8 |
                      (unsigned long) data[offset + 2] << 16 |
                      (unsigned long) data[offset + 3] << 24 |
                      (unsigned long) data[offset + 4] << 32 |
                      (unsigned long) data[offset + 5] << 40 |
                      (unsigned long) data[offset + 6] << 48 |
                      (unsigned long) data[offset + 7] << 56;
  offset += sizeof(unsigned long);
  return val;
}

void MemBlock::writeU8(unsigned char value)
{
  if (offset + sizeof(unsigned char) > dataSize)
  {
    return;
  }
  data[offset] = value;
  offset += sizeof(unsigned char);
}

void MemBlock::writeU16(unsigned short value)
{
  if (offset + sizeof(unsigned short) > dataSize)
  {
    return;
  }
  data[offset] = (unsigned char) value;
  data[offset + 1] = (unsigned char) (value >> 8);
  offset += sizeof(unsigned short);
}

void MemBlock::writeU32(unsigned int value)
{
  if (offset + sizeof(unsigned int) > dataSize)
  {
    return;
  }
  data[offset] = (unsigned char) value;
  data[offset + 1] = (unsigned char) (value >> 8);
  data[offset + 2] = (unsigned char) (value >> 16);
  data[offset + 3] = (unsigned char) (value >> 24);
  offset += sizeof(unsigned int);
}

void MemBlock::writeU64(unsigned long value)
{
  if (offset + sizeof(unsigned long) > dataSize)
  {
    return;
  }
  data[offset] = (unsigned char) value;
  data[offset + 1] = (unsigned char) (value >> 8);
  data[offset + 2] = (unsigned char) (value >> 16);
  data[offset + 3] = (unsigned char) (value >> 24);
  data[offset + 4] = (unsigned char) (value >> 32);
  data[offset + 5] = (unsigned char) (value >> 40);
  data[offset + 6] = (unsigned char) (value >> 48);
  data[offset + 7] = (unsigned char) (value >> 56);
  offset += sizeof(unsigned long);
}

void MemBlock::hd() const
{
  const int rowSize = 16;      // number of bytes to print per row
  const int asciiStart = 0x20; // ASCII value for printable characters
  const int asciiEnd = 0x7E;
  const int asciiFiller = '.'; // filler character for non-printable bytes
  
  for (int i = 0; i < dataSize; i += rowSize)
  {
    // print row header
    std::cout << std::setw(8) << std::setfill('0') << std::hex << i << ": ";
    
    // print hex dump for this row
    for (int j = 0; j < rowSize; j++)
    {
      if (i + j < dataSize)
      {
        std::cout << std::setw(2) << static_cast<int>(data[i + j]) << ' ';
      } else
      {
        std::cout << "   ";
      }
    }
    
    // print ASCII representation for this row
    std::cout << "  ";
    for (int j = 0; j < rowSize; j++)
    {
      if (i + j < dataSize)
      {
        const int c = data[i + j];
        if (c >= asciiStart && c <= asciiEnd)
        {
          std::cout << static_cast<char>(c);
        } else
        {
          std::cout << static_cast<char>(asciiFiller);
        }
      } else
      {
        std::cout << ' ';
      }
    }
    
    // end of row
    std::cout << std::endl;
  }
}