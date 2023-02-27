//
// Created by Richard Marks on 2/26/23.
//

#include "Devices.h"

#include <iostream>

using namespace vc23;

unsigned short StandardOutputDevice::getID() const
{
  return 0;
}

std::string StandardOutputDevice::getName() const
{
  return "STDOUT";
}

void StandardOutputDevice::writeU8(unsigned char value)
{
  std::cout << value;
}