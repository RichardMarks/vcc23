//
// Created by Richard Marks on 2/21/23.
//

#ifndef VCC23_BYTEORDER_H
#define VCC23_BYTEORDER_H

#include <vector>
#include <cstring>

namespace common
{
  template <typename T>
  class ByteOrder
  {
  public:
    /// @brief writes the given value to the buffer in little-endian byte order
    /// @param buffer buffer to write to
    /// @param value value to be written
    static void write(std::vector<unsigned char> &buffer, const T &value)
    {
      static_assert(std::is_trivial<T>::value, "write: T must be a trivial type");
      T leValue = value;
      auto isle = *reinterpret_cast<const unsigned short *>("\0\x01") == 0x0100;
      if (!isle)
      {
        auto *p = reinterpret_cast<unsigned char *>(&leValue);
        for (int i = 0; i < sizeof(T) / 2; ++i)
        {
          std::swap(p[i], p[sizeof(T) - 1 - i]);
        }
      }
      buffer.resize(buffer.size() + sizeof(leValue));
      std::memcpy(&buffer[buffer.size() - sizeof(leValue)], &leValue, sizeof(leValue));
    }
    
    /// @brief reads a little-endian byte order value from the buffer
    /// @param buffer buffer to read from
    /// @param readPosition where to read from the buffer (will be updated to after the read value)
    /// @return
    static T read(std::vector<unsigned char> &buffer, size_t &readPosition)
    {
      static_assert(std::is_trivial<T>::value, "read: T must be a trivial type");
      T leValue;
      std::memcpy(&leValue, &buffer[readPosition], sizeof(leValue));
      readPosition += sizeof(leValue);
      T value = leValue;
      auto isle = *reinterpret_cast<const unsigned short *>("\0\x01") == 0x0100;
      if (!isle)
      {
        auto *p = reinterpret_cast<unsigned char *>(&value);
        for (int i = 0; i < sizeof(T) / 2; ++i)
        {
          std::swap(p[i], p[sizeof(T) - 1 - i]);
        }
      }
      return value;
    }
  };
}

#endif //VCC23_BYTEORDER_H
