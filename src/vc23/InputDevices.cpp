//
// Created by Richard Marks on 2/26/23.
//

#include "Devices.h"

#include <cstdio>

using namespace vc23;

unsigned short StandardInputDevice::getID() const
{
  return 0;
}

std::string StandardInputDevice::getName() const
{
  return "STDIN";
}

unsigned char StandardInputDevice::readU8()
{
  return std::getchar();
}