//
// Created by Richard Marks on 2/21/23.
//
#include "CartWriter.h"

#include <iostream>
#include <fstream>

using namespace common;

template <typename T>
void writeLE(std::ofstream &outfile, T value)
{
  size_t size_bytes = sizeof(value);
  for (int i = 0; i < size_bytes; i++)
  {
    outfile.put(value & 0xFF);
    value >>= 8;
  }
}

void CartWriter::setName(const std::string &cartName)
{
  // Truncate the string if it is longer than 32 characters
  std::string truncated = cartName.substr(0, 32);
  
  // Pad the string with spaces if it is shorter than 32 characters
  if (truncated.length() < 32) {
    truncated.append(32 - truncated.length(), ' ');
  }
  
  name = truncated;
}

void CartWriter::setData(const std::vector<unsigned char>& dataSource)
{
  data = std::vector<unsigned char>(dataSource.begin(), dataSource.end());
}

void CartWriter::setCode(const std::vector<unsigned char>& codeSource)
{
  code = std::vector<unsigned char>(codeSource.begin(), codeSource.end());
}

void CartWriter::writeToFile(const std::string &fileName)
{
  std::ofstream fp(fileName, std::ios::binary);
  
  if (!fp.is_open())
  {
    throw std::runtime_error("Unable to write - open file failed.");
  }
  
  char fourcc[] = {'v', 'c', '2', '3'};
  fp.write(reinterpret_cast<char *>(&fourcc), sizeof(char) * 4);
  
  fp.write(reinterpret_cast<char *>(const_cast<char *>(name.c_str())), sizeof(char) * 33);
  
  unsigned long reserved[] = {0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL, 0UL};
  fp.write(reinterpret_cast<char *>(&reserved), 16 * sizeof(unsigned long));
  
  unsigned long dataLength = data.size();
  writeLE<unsigned long>(fp, dataLength);
  
  unsigned long codeLength = code.size();
  writeLE<unsigned long>(fp, codeLength);
  
  for (auto x : data)
  {
    fp.write(reinterpret_cast<char *>(&x), sizeof(char));
  }
  
  for (auto x : code)
  {
    fp.write(reinterpret_cast<char *>(&x), sizeof(char));
  }
  
  fp.close();
}