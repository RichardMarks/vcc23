//
// Created by Richard Marks on 2/26/23.
//

#ifndef VC23_DEVICES_H
#define VC23_DEVICES_H

#include <string>

namespace vc23
{
  class Device
  {
  public:
    virtual ~Device() = default;
    
    [[nodiscard]] virtual unsigned short getID() const = 0;
    
    [[nodiscard]] virtual std::string getName() const = 0;
  };
  
  class InputDevice : public Device
  {
  public:
    virtual ~InputDevice() = default;
    
    [[nodiscard]] unsigned short getID() const override = 0;
    
    [[nodiscard]] std::string getName() const override = 0;
    
    virtual unsigned char readU8() = 0;
  };
  
  class OutputDevice : public Device
  {
  public:
    virtual ~OutputDevice() = default;
    
    [[nodiscard]] unsigned short getID() const override = 0;
    
    [[nodiscard]] std::string getName() const override = 0;
    
    virtual void writeU8(unsigned char) = 0;
  };
  
  class StandardInputDevice : public InputDevice
  {
  public:
    [[nodiscard]] unsigned short getID() const override;
    
    [[nodiscard]] std::string getName() const override;
    
    unsigned char readU8() override;
  };
  
  class StandardOutputDevice : public OutputDevice
  {
  public:
    [[nodiscard]] unsigned short getID() const override;
    
    [[nodiscard]] std::string getName() const override;
    
    void writeU8(unsigned char) override;
  };
}

#endif //VC23_DEVICES_H
