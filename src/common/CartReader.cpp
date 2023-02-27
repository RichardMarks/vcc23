//
// Created by Richard Marks on 2/22/23.
//

#include "CartReader.h"

#include <iostream>
#include <fstream>

using namespace common;

CartReader::CartReader() : valid(false)
{}

void CartReader::readFromFile(const std::string &fileName)
{
  valid = false;
  std::ifstream fp(fileName, std::ios::binary);
  
  if (!fp.is_open())
  {
    throw std::runtime_error("Unable to load - open file failed.");
  }
  
  // identify file
  char fourcc[4] = {' '};
  fp.read(reinterpret_cast<char *>(&fourcc), sizeof(char) * 4);
  
  if (
    fourcc[0] != 'v' ||
    fourcc[1] != 'c' ||
    fourcc[2] != '2' ||
    fourcc[3] != '3')
  {
    fp.close();
    throw std::runtime_error("Unable to load - Invalid vc23 cart file.");
  }
  
  // read name
  char cartName[33] = {'\0'};
  fp.read(reinterpret_cast<char *>(&cartName), sizeof(char) * 33);
  name = cartName;
  
  // skip reserved block
  fp.seekg(16 * sizeof(unsigned long), std::ios_base::cur);
  
  // read block sizes
  unsigned long dataLength = 0;
  unsigned long codeLength = 0;
  fp.read(reinterpret_cast<char *>(&dataLength), sizeof(unsigned long));
  fp.read(reinterpret_cast<char *>(&codeLength), sizeof(unsigned long));
  
  data.reserve(dataLength);
  for (unsigned long i = 0; i < dataLength; i++)
  {
    unsigned char byt = 0;
    fp.read(reinterpret_cast<char *>(&byt), sizeof(unsigned char));
    data.push_back(byt);
  }
  
  code.reserve(codeLength);
  for (unsigned long i = 0; i < codeLength; i++)
  {
    unsigned char byt = 0;
    fp.read(reinterpret_cast<char *>(&byt), sizeof(unsigned char));
    code.push_back(byt);
  }
  
  fp.close();
  valid = true;
}

std::vector<unsigned char> CartReader::getData() const
{
  return std::vector<unsigned char>(data.begin(), data.end());
}

std::vector<unsigned char> CartReader::getCode() const
{
  return std::vector<unsigned char>(code.begin(), code.end());
}

std::string CartReader::getName() const
{
  return name;
}